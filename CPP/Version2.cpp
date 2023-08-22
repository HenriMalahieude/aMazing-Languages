//Programmed by Henri Malahieude in 2021
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

//Challenge: Don't use Classes or Objects. Actually probably makes this easier.

const unsigned int mSize = 21; //must be ODD

char maze[mSize][mSize];
vector<vector<int>> stack;
vector<vector<int>> visited;

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

void emptyMaze(){
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

void printMaze(){
    for (unsigned iY = 0; iY < mSize; iY++){
        for (unsigned iX = 0; iX < mSize; iX++){
            cout << maze[iX][iY] << ' ';
        }
        cout << endl;
    }
}

int generateRandomInt(int low, int high){
    return low + rand() % (high - low + 1);
}

void getRandomPos(vector<int> &pos){
    //cout << 1 << endl;
    vector<int> randomized = {generateRandomInt(1, mSize-2), generateRandomInt(1, mSize-2)};

    if (randomized[0] % 2 == 0){
        int r = rand() % 2;
        if(r == 0){
            randomized[0]++;
        }else{
            randomized[0]--;
        }
    }
    
    //cout << 2 << endl;

    if (randomized[1] % 2 == 0){
        int r = rand() % 2;
        if(r == 0){
            randomized[1]++;
        }else{
            randomized[1]--;
        }
    }

    //cout << 3 << endl;
    clamp(randomized[0], 1, mSize-2);
    clamp(randomized[1], 1, mSize-2);

    //cout << 4 << endl;
    pos = randomized;

    //cout << 5 << endl;
}

//randomly create entrances, but eh
void createRandomEntrances(){
    vector<int> pos;

    getRandomPos(pos);
    maze[0][pos[1]] = ' ';

    getRandomPos(pos);
    maze[mSize-1][pos[1]] = ' ';
}

//just on opposite ends
void createStandardEntrances(){
    maze[0][1] = ' ';
    maze[mSize-1][mSize-2] = ' ';
}

bool checkVectorForPos(vector<int> pos, const vector<vector<int>> &v){
    for (unsigned i = 0; i < v.size(); i++){
        if (pos[0] == v[i][0] && pos[1] == v[i][1]){
            return true;
        }
    }
    return false;
}

void getPossibleDirections(vector<int> pos, vector<vector<int>> &directions){
    vector<vector<int>> possibleDirections = {
        {2, 0},
        {0, 2},
        {-2, 0},
        {0, -2},
    };
    vector<int> removeIndices;

    //cout << "Start Position: " << pos[0] << ',' << pos[1] << endl;

    for (int d = 0; d < 4; d++){
        vector<int> newPos = {pos[0] + possibleDirections[d][0], pos[1] + possibleDirections[d][1]};

        //cout << "New Position: " << newPos[0] << ',' << newPos[1] << endl;
        if(newPos[0] < 1 || newPos[0] > mSize-2){
            removeIndices.push_back(d);
            //cout << "Out of X bounds" << endl;
            continue;
        }
        if(newPos[1] < 1 || newPos[1] > mSize-2){
            removeIndices.push_back(d);
            //cout << "Out of Y bounds" << endl;
            continue;
        }

        if (checkVectorForPos(newPos, stack) || checkVectorForPos(newPos, visited)){
            removeIndices.push_back(d);
            //cout << "Already visited!" << endl;
            continue;
        }
    }

    //cout << removeIndices.size() << endl;

    for (unsigned i = 0; i < removeIndices.size(); i++){
        possibleDirections.erase(possibleDirections.begin() + removeIndices[i] - i);
    }

    directions = possibleDirections;
}

void generateMaze(){
    int numOfCells = (mSize-1) / 2;
    numOfCells *= numOfCells;

    //cout << "rbuh" << endl;
    
    vector<int> currentCell;
    getRandomPos(currentCell);

    //cout << visited.size() << endl;
    //cout << stack.size() << endl;
    //cout << numOfCells << endl;

    while(visited.size() + stack.size() < (numOfCells - 1)){
        vector<vector<int>> possibleDirections;
        getPossibleDirections(currentCell, possibleDirections);

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

    //cout << "End!" << endl;
}

int main() {
    srand(time(0));//seed for the pseudo-random thingy

    if (mSize % 2 == 0){
        cout << "mSize must be odd!" << endl;
        return 1;
    }

    emptyMaze();
    createStandardEntrances();

    //Test getRandomPosition()
    /*int pos[2];
    getRandomPos(pos);
    cout << pos[0] << ',' << pos[1] << endl;
    maze[pos[0]][pos[1]] = ' ';*/

    //Test getPossibleDirections()
    /*vector<vector<int>> directions;
    getPossibleDirections((vector<int>){9, 9}, directions);
    cout << directions.size() << endl;
    for (unsigned i = 0; i < directions.size(); i++){
        cout << directions.at(i)[0] << directions.at(i)[1] << endl;
    }*/
    //cout << maze[10][10] << endl;

    generateMaze();
    printMaze();

    return 0;
}


