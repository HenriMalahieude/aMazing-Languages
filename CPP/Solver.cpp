//Programmed by Henri Malahieude in 2021, looks like I just copy pasted Version2
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

//Challenge: Don't use Classes or Objects. Actually probably makes this easier.

const unsigned int mSize = 21; //must be ODD

//Global Functions
void clamp(int &value, int min, int max){
    if (min > max){
        return;
    }

    if (value > max){
        value = max;
    }else if(value < min){
        value = min;
    }
}

int generateRandomInt(int low, int high){
    return low + rand() % (high - low + 1);
}

bool checkVectorForPos(vector<int> pos, const vector<vector<int>> &v){
    for (unsigned i = 0; i < v.size(); i++){
        if (pos[0] == v[i][0] && pos[1] == v[i][1]){
            return true;
        }
    }
    return false;
}

//Maze Generation (using back tracker method)
void emptyMaze(char (&maze)[mSize][mSize]){
    for (unsigned iX = 0; iX < mSize; iX++){
        for (unsigned iY = 0; iY < mSize; iY++){
            if (iY % 2 == 0 || iX % 2 == 0){
                maze[iX][iY] = '#';
            }else{
                maze[iX][iY] = ' ';
            }
        }
    }
}

void printMaze(char (&maze)[mSize][mSize]){
    for (unsigned iY = 0; iY < mSize; iY++){
        for (unsigned iX = 0; iX < mSize; iX++){
            cout << maze[iX][iY] << ' ';
        }
        cout << endl;
    }
}

void getRandomPos(vector<int> &pos){
    vector<int> randomized = {generateRandomInt(1, mSize-2), generateRandomInt(1, mSize-2)};

    if (randomized[0] % 2 == 0){
        int r = rand() % 2;
        if(r == 0){
            randomized[0]++;
        }else{
            randomized[0]--;
        }
    }

    if (randomized[1] % 2 == 0){
        int r = rand() % 2;
        if(r == 0){
            randomized[1]++;
        }else{
            randomized[1]--;
        }
    }

    clamp(randomized[0], 1, mSize-2);
    clamp(randomized[1], 1, mSize-2);

    pos = randomized;
}

    //randomly create entrances, but eh
void createRandomEntrances(char (&maze)[mSize][mSize]){
    vector<int> pos;

    getRandomPos(pos);
    maze[0][pos[1]] = ' ';

    getRandomPos(pos);
    maze[mSize-1][pos[1]] = ' ';
}

    //just on opposite ends
void createStandardEntrances(char (&maze)[mSize][mSize]){
    maze[0][1] = ' ';
    maze[mSize-1][mSize-2] = ' ';
}

void removeSelectedIndices(vector<int> selectedIndices, vector<vector<int>> &from){
    for (unsigned i = 0; i < selectedIndices.size(); i++){
        from.erase(from.begin() + selectedIndices[i] - i);
    }
}

void getPossibleDirections(vector<int> pos, vector<vector<int>> &directions, const vector<vector<int>> &stack, const vector<vector<int>> &visited){
    vector<vector<int>> possibleDirections = {
        {2, 0},
        {0, 2},
        {-2, 0},
        {0, -2},
    };
    vector<int> removeIndices;

    for (int d = 0; d < 4; d++){
        vector<int> newPos = {pos[0] + possibleDirections[d][0], pos[1] + possibleDirections[d][1]};

        if(newPos[0] < 1 || newPos[0] > mSize-2){
            removeIndices.push_back(d);
            continue;
        }
        if(newPos[1] < 1 || newPos[1] > mSize-2){
            removeIndices.push_back(d);
            continue;
        }

        if (checkVectorForPos(newPos, stack) || checkVectorForPos(newPos, visited)){
            removeIndices.push_back(d);
            continue;
        }
    }

    removeSelectedIndices(removeIndices, possibleDirections);

    directions = possibleDirections;
}

void generateMaze(char (&maze)[mSize][mSize]){
    int numOfCells = (mSize-1) / 2;
    numOfCells *= numOfCells;

    vector<vector<int>> stack;
    vector<vector<int>> visited;

    emptyMaze(maze);
    createStandardEntrances(maze);
    
    vector<int> currentCell;
    getRandomPos(currentCell);

    while(visited.size() + stack.size() < (numOfCells - 1)){
        vector<vector<int>> possibleDirections;
        getPossibleDirections(currentCell, possibleDirections, stack, visited);

        //cout << "Visited: " << visited.size() << " | In Stack: " << stack.size() << " | Directions: " << possibleDirections.size() << endl;

        if (possibleDirections.size() < 1){ //backtrack
            //cout << "Backtracking!" << endl;
            visited.push_back(currentCell);

            currentCell = stack.back();
            stack.pop_back();
        }else{ //move forward (dig out walls)
            //cout << "Moving forward!" << endl;
            vector<int> chosenDir = possibleDirections[rand() % possibleDirections.size()];
            vector<int> wallPos = {currentCell[0] + (chosenDir[0]/2), currentCell[1] + (chosenDir[1]/2)};
            //Remove the wall
            maze[wallPos[0]][wallPos[1]] = ' ';

            stack.push_back(currentCell);
            currentCell = {currentCell[0] + chosenDir[0], currentCell[1] + chosenDir[1]};
        }
    }
}

//Pathfinding functions
void copyArrayIntoOther(char (&receiving)[mSize][mSize], const char (&giving)[mSize][mSize]){ //like saying receiving = giving, but since arrays can't be set to each other :T
    for(unsigned iX = 0; iX < mSize; iX++){
        for(unsigned iY = 0; iY < mSize; iY++){
            receiving[iX][iY] = giving[iX][iY];
        }
    }
}

void getConnections(vector<int> pos, vector<vector<int>> &possibleDirections, vector<vector<int>> &stack, vector<vector<int>> &visited, char (&solving)[mSize][mSize]){
    vector<int> removeIndices;
    getPossibleDirections(pos, possibleDirections, stack, visited);

    for (int d = 0; d < possibleDirections.size(); d++){
        vector<int> boundaryPos = {pos[0] + possibleDirections[d][0]/2, pos[1] + possibleDirections[d][1]/2};

        if(solving[boundaryPos[0]][boundaryPos[1]] == '#'){
            removeIndices.push_back(d);
            continue;
        }
    }

    removeSelectedIndices(removeIndices, possibleDirections);
}

void solveMaze(char (&maze)[mSize][mSize], char (&solvedMaze)[mSize][mSize]){
    copyArrayIntoOther(solvedMaze, maze);

    int numOfCells = (mSize-1) / 2;
    numOfCells *= numOfCells;

    vector<int> startPos = {1, 1};
    vector<int> endPos = {mSize-2, mSize-2};
    //TODO: Determine where start and end are based on empty space on maze boundary

    vector<vector<int>> positionalStack;
    vector<vector<int>> visited;

    vector<int> currentPos = startPos;

    for (int iterator = 0; iterator < (numOfCells*2); iterator++){ //capping the limit to how far the pathfinder can search to twice the amount of cells there are which should be worse case scenario
        vector<int> distance = {currentPos[0] - endPos[0], currentPos[1] - endPos[1]};
        double distanceNumber = distance[0] * distance[0] + distance[1] * distance[1];
        if (distanceNumber <= 1){
            positionalStack.push_back(currentPos);
            break; //we are within one array unit from the end, so we've effectively finished
        }

        vector<vector<int>> possibleDirections;
        getConnections(currentPos, possibleDirections, positionalStack, visited, maze);

        if (possibleDirections.size() < 1){
            visited.push_back(currentPos);

            currentPos = positionalStack.back();
            positionalStack.pop_back();
        }else{
            vector<int> randomDirection = possibleDirections[rand() % possibleDirections.size()];

            positionalStack.push_back(currentPos);
            currentPos = {currentPos[0] + randomDirection[0], currentPos[1] + randomDirection[1]};
        }
    }

    vector<int> lastPathPosition;
    vector<int> pathPosition;
    for (unsigned pathIndex = 0; pathIndex < positionalStack.size(); pathIndex++){
        pathPosition = positionalStack[pathIndex];

        if(pathIndex > 0){ //also mark the walls
            vector<int> direction = {pathPosition[0] - lastPathPosition[0], pathPosition[1] - lastPathPosition[1]};

            vector<int> boundaryPosition = {lastPathPosition[0] + (direction[0]/2), lastPathPosition[1] + (direction[1]/2)};

            if (abs(direction[0]) > 1){
                solvedMaze[pathPosition[0]][pathPosition[1]] = '-';
                solvedMaze[boundaryPosition[0]][boundaryPosition[1]] = '-';
            }else{
                solvedMaze[pathPosition[0]][pathPosition[1]] = '|';
                solvedMaze[boundaryPosition[0]][boundaryPosition[1]] = '|';
            }
        }
        lastPathPosition = pathPosition;
    }

    solvedMaze[startPos[0]][startPos[1]] = '-';
    solvedMaze[endPos[0]][endPos[1]] = '-';

    solvedMaze[0][1] = 'O';
    solvedMaze[mSize-1][mSize-2] = 'X';
}


void outputX(char (&chosen)[mSize][mSize], int iY){
    for(unsigned iX = 0; iX < mSize; iX++){
        cout << chosen[iX][iY] << ' ';
    }
}

int main() {
    srand(time(0));
    char maze[mSize][mSize];
    char solvedMaze[mSize][mSize];

    if (mSize % 2 == 0){
        cout << "mSize must be odd!" << endl;
        return 1;
    }

    generateMaze(maze);

    //cout << "Generated Maze!" << endl;

    solveMaze(maze, solvedMaze);

    //cout << "Solved Maze!" << endl;

    //printMaze(maze);

    for(unsigned iY = 0; iY < mSize; iY++){
        //First the normal maze
        outputX(maze, iY);

        cout << "       ";

        outputX(solvedMaze, iY);

        cout << endl;
    }

    return 0;
}