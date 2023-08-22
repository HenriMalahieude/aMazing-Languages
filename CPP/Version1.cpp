//Designed in 2017
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cmath>

//Challenge: Don't use Classes or Objects. Actually probably makes this easier.
/*
Starter Cell = S;
Open = O;
Visited = V;
Used = U;
Destroyed Wall = X;

maze[y][x];
cell[2] = {y, x};

*/

const unsigned int mSize = 25; //must be ODD

std::vector<std::vector<char> > maze(mSize, std::vector<char>(mSize, '#')); //also fills everything with wall cell text

void setUp(){ //create the proper cell openings
  for (int x = 0; x < mSize; x++){
    for (int y = 0; y < mSize; y++){
      if ((x+1)%2==0 && (y+1)%2==0){//when both x and y even
        maze[y][x] = 'O';
      }
    }
  }
}

void cleanUp(){ //cleanup the cell markings used to avoid classes use
  for (int y = 0; y < mSize; y++){
    for (int x = 0; x < mSize; x++){
      char cell = maze[y][x];
      if (cell == 'O'){
        maze[y][x] = ' ';
      }else if (cell == 'V'){
        maze[y][x] = ' ';
      }else if (cell == 'U'){
        maze[y][x] = ' ';
      }else if (cell == 'X'){
        maze[y][x] = ' ';
      }
    }
  }
}

void output(){ //output maze to console
  for (int y = 0; y < mSize; y++){
    for (int x = 0; x < mSize; x++){
      std::cout << maze[y][x] << ' ';
    }
    std::cout << std::endl;
  }
}

std::string cellType(int cell[2]){ //identify cell type to be able to determine course of action
  int x = cell[1];
  int y = cell[0];
  char cData = maze[y][x];
  if (cData == 'O'){
    return "Open";
  }else if (cData == 'V'){
    return "Visited";
  }else if (cData == 'U'){
    return "Used";
  }else if (cData == 'X'){
    return "Destroyed Wall";
  }else if (cData == '#'){
    return "Wall";
  }
  return "Could not classify"; //man do I hate monotonous typing
}

int OpenNum(){ //returns how many cells are unused/open for use.
  int num = 0;
  for (int x = 0; x < mSize; x++){
    for (int y = 0; y < mSize; y++){
      int cell[2] = {y, x};
      std::string cTyp = cellType(cell);
      if (cTyp == "Open"){
        num++;
      }
    }
  }
  return num;
}

void destWall(int cCell[2], std::string dir){ //destroy wall (this is what makes the maze visible)
  int y = cCell[0];
  int x = cCell[1];
  if (dir == "Up"){
    if (y > 1){
      maze[y - 1][x] = 'X';
    }
  }else if (dir == "Down"){
    if (y < (mSize - 1)){
      maze[y + 1][x] = 'X';
    }
  }else if (dir == "Left"){
    if (x > 1){
      maze[y][x - 1] = 'X';
    }
  }else if (dir == "Right"){
    if (x < (mSize - 1)){
      maze[y][x + 1] = 'X';
    }
  }
}

void getSCell(int sCell[2]){ //the maze has to start some where, gets a starter cell. And makes an entrance/opening
  int pPos[99] = {1};
  int dex = 0;
  for (int i=1; i<mSize; i=i+2){
      pPos[dex+1] = i;
      dex = dex + 1;
  };
  int r = rand() % 4;
  int index = pPos[(rand() % 6)];
  if (r == 0){//up
    sCell[0] = 1;
    sCell[1] = index;
    maze[0][index] = 'X';
  }else if (r == 1){ //down
    sCell[0] = mSize-2;
    sCell[1] = index;
    maze[mSize-1][index] = 'X';
  }else if (r == 2){ //left
    sCell[0] = index;
    sCell[1] = 1;
    maze[index][0] = 'X';
  }else if (r == 3){ //right
    sCell[0] = index;
    sCell[1] = mSize-2;
    maze[index][mSize-1] = 'X';
  }
} //this function is used twice to make an entrance and exit.

int newPosX(int cCell[2], std::string dir){//functions cannot output arrays so i have to make two functions: One for X and One for Y
  int x = cCell[1];
  if (dir == "Left"){
    if (x > 1){
      return (x - 2);
    }
  }else if (dir == "Right"){
    if (x < (mSize-2)){
      return (x + 2);
    }
  } //X only handles Left and right
  return x;
}

int newPosY(int cCell[2], std::string dir){
  int y = cCell[0];
  if (dir == "Up"){
    if (y > 1){
      return (y - 2);
    }
  }else if (dir == "Down"){
      if (y < (mSize-2)){
        return (y + 2);
      }
    }//Y only handles up or down
  return y;
}

std::string nextDirOpen(int cCell[2]){//to locate a new open cell we can dig to
  int y = cCell[0];
  int x = cCell[1];
  std::string dirs[4] = {"Up", "Down", "Left", "Right"};
  std::string pDirs[4];
  int index = 0;
  for (int i = 0; i < 4; i++){
    int newCell[2] = {newPosY(cCell, dirs[i]), newPosX(cCell, dirs[i])}; //get the new cell
    if (newCell[0] != cCell[0] || newCell[1] != cCell[1]){ //if it isn't the same
      std::string cType = cellType(newCell); //get the cell type
      if (cType == "Open"){ //is it an Open cell?
        pDirs[index] = dirs[i]; // add it to the array
        index++;
      }
    }
  }
  if (index > 0){
    int r = rand() % index;
    return pDirs[r]; //returns a random direction that is possible
  }else{
    return "None";
  }
}

std::string nextDirVisited(int cCell[2]){//if there isnt an open cell then we just backtrack back, and put our stamp of "Used"
  int y = cCell[0];
  int x = cCell[1];
  std::string dirs[4] = {"Up", "Down", "Left", "Right"};
  std::string pDirs[4];
  int index = 0;
  for (int i = 0; i < 4; i++){
    int newCell[2] = {newPosY(cCell, dirs[i]), newPosX(cCell, dirs[i])}; //get the new cell
    if (newCell[0] != cCell[0] || newCell[1] != cCell[1]){ //if it isn't the same
      std::string cType = cellType(newCell); //get the cell type
      if (cType == "Visited"){ //is it an Open cell?
        pDirs[index] = dirs[i]; // add it to the array
        index++;
      }
    }
  }
  if (index > 0){
    int r = rand() % index;
    return pDirs[r]; //returns a random direction that is possible
  }else{
    return "None";
  }
}

//this function is used because I decided to omit the use of a Stack, so this teleports the digger to another spot when it is surrounded by used cells
int VisitedGet(int cCell[2]){ //for when we get stuck in a hole
  std::vector<std::vector<int> > posCells(36, std::vector<int>(2, 0)); //first two vectors determine type, then after the name: (amount in first vector, and the second vector's stuff) initializing
  int index = 0;
  for (int x = 0; x < mSize; x++){
    for (int y = 0; y < mSize; y++){
      int cell[2] = {y, x};
      std::string cTyp = cellType(cell);
      std::string pDir = nextDirOpen(cell);
      std::string vDir = nextDirVisited(cell);
      if (cTyp == "Visited" && pDir != "None" && vDir != "None"){
        posCells[index][0] = y;
        posCells[index][1] = x;
        index++;
        /*std::cout << pDir << " " << vDir << std::endl;
        cCell[0] = y;
        cCell[1] = x; */
        //break;
      }
    }
  }
  //std::cout << "Remaining possible visited cells: " << index << std::endl;
  if (index > 0){
    int r = (rand() % index);
    cCell[0] = posCells[r][0];
    cCell[1] = posCells[r][1];
  }
  return index;
}

void generate(){ //the heart of this program
  setUp();
  //starter cell
  int sCell[2];
  getSCell(sCell);
  //current cell
  int cCell[2] = {sCell[1], sCell[0]}; //remember it is y , x
  int numOpenCells = OpenNum(); //gets us the number of open cells
  //std::cout << "Starting Cell: (" << cCell[1] << "," << cCell[0] << ")" << std::endl;
  int lastIndex = 0;
  while (numOpenCells > 0){ //loop until there aren't any more open cells
  //getting the new cell
    int newCell[2];
    int mX = cCell[1];
    int mY = cCell[0];
    std::string newDir = nextDirOpen(cCell); //next unused cell
    //std::cout << newDir << std::endl;
    if (newDir == "None"){ //This was the most frustrating part of the program
      newDir = nextDirVisited(cCell);
      if (newDir == "None"){ //just in case we back track where we shouldn't
        int index = VisitedGet(newCell);
        if (lastIndex == index){
          break;
        }else{
          lastIndex = index;
        }
      }else{
        //std::cout << "Backtracking: " << newDir << std::endl;
        newCell[0] = newPosY(cCell, newDir);
        newCell[1] = newPosX(cCell, newDir);
        maze[mY][mX] = 'U';
      }
    }else{
      //std::cout << "Digging: " << newDir << std::endl;
      newCell[0] = newPosY(cCell, newDir);
      newCell[1] = newPosX(cCell, newDir);
      maze[mY][mX] = 'V';
      destWall(cCell, newDir); //destroy the wall
    }
    //proper actions with new cell
    cCell[0] = newCell[0];
    cCell[1] = newCell[1];
    numOpenCells = OpenNum();
    //std::cout << numOpenCells << std::endl;
  }
  getSCell(cCell);
  //output();
}

int main() {
  srand(time(NULL));//seed for the pseudo-random thingy
  generate();
  cleanUp();
  output();
  //std::cout << "We good!" << std::endl;
  return 0;
}
