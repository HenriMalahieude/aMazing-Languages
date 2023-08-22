import java.lang.Math;

public class Maze
{
    private int s = 5;
    private Cell[] mD; //useful maze Data
    private String[][] mV; //maze Visual (used in printing)
    
    private String wall = "# "; //should probably be constants, oh well
    private String empt = "  ";
    
    private int[][] dirs = {{1,0},{-1,0},{0,1},{0,-1}}; //multiply directions by 2 to get adjacent cell, default version is for walls
    
    public Maze(int size) //Size must be odd, will fix any evens.
    {
        if (size % 2 == 0){
            size++;
            if (size < 5){ //minimum size of 5 or 3x3 cells
                size = 5;
            }
            //System.out.println(size);
        }
        s = size;
    }
    
    public void printMaze(){
        System.out.print("\n");
        for (int x=0; x<(s); x++){
            for (int y=0; y<(s); y++){
                System.out.print(mV[x][y]);
            }
            System.out.print("\n");
        }
    }
    
    public void newGeneration(){
        clearMaze();
        while (seedsIn() > 1){
            int nPos[] = randCellPos();
            int rDir[] = getNextDir(nPos);
            if (rDir[0] != -1){
                delWall(rDir, nPos);
                int ePos[] = {nPos[0]+(rDir[0]*2), nPos[1]+(rDir[1]*2)};
                int sd1 = getCellSeedFromPos(nPos);
                int sd2 = getCellSeedFromPos(ePos);
                groupSeedChange(sd1, sd2);
            }
        }
        mV[1 + (int)(Math.random()*((s-1)/2))*2][0] = "  "; //random positioning for an opening
        mV[1 + (int)(Math.random()*((s-1)/2))*2][(s-1)] = "  "; //random positioning for an ending
    }
    
    private void clearMaze(){ //creates a new ungenerated maze
        Cell mazeData[] = new Cell[((s-1)/2)*((s-1)/2)];
        String mazeVis[][] = new String[s][s];
        int index = 0;
        for (int x=0; x<(s); x++){
            for (int y=0; y<(s); y++){
                int p[] = {x, y};
                if ((x+1)%2==0 && (y+1)%2==0){
                    mazeVis[x][y] = empt;
                    mazeData[index] = new Cell(pairing(x, y), p);
                    index++;
                }else{
                    mazeVis[x][y] = wall;
                }
            }
        }
        mD = mazeData; //this is for accessing cells that we can use for generation
        mV = mazeVis;
    }
    
    private int[] getNextDir(int nPos[]){ //gets a new direction from an inputed position of a cell
        int dirsPossible[][] = new int[4][2];
        int error[] = {-1, -1};
        int sd = mD[getCellIndexFromPos(nPos)].getSeed();
        int xDIndex = 0;
        boolean check1 = false;
        for (int i = 0; i < 4; i++){
            int nPW[] = {clamp(nPos[0] + dirs[i][0]), clamp(nPos[1] + dirs[i][1])};
            if (nPW[0] != 0 && nPW[0] != (s-1)){
                if (nPW[1] != 0 && nPW[1] != (s-1)){
                    check1 = true;
                }
            }
            int nPC[] = {clamp(nPos[0] + (dirs[i][0]*2)), clamp(nPos[1] + (dirs[i][1]*2))};
            if (check1 && nPC[0] != 0 && nPC[0] != (s-1)){
                if (getCellSeedFromPos(nPC) != sd && nPC[1] != 0 && nPC[1] != (s-1)){
                    dirsPossible[xDIndex][0] = dirs[i][0];
                    dirsPossible[xDIndex++][1] = dirs[i][1];
                }
            }
        }
        if (xDIndex == 0){
            mD[getCellIndexFromPos(nPos)].useable = false;
            return error;
        }
        return dirsPossible[(int)(Math.random()*xDIndex)];
    }
    
    private void groupSeedChange(int masterSeed, int inferiorSeed){ //changes all cells with inferior seed to the master seed
        for (int i=0; i<(mD.length); i++){
            if (mD[i].getSeed() == inferiorSeed){
                mD[i].setSeed(masterSeed);
            }
        }
    }
    
    private int seedsIn(){ //returns the amount of unique seeds that still exist, can be considered if seedsIn() > 1 then algorithm not finished
        int seedList[] = new int[s*s];
        int ind = 0;
        for (int i=0; i<mD.length; i++){
            int cSeed = mD[i].getSeed();
            boolean add = true;
            for (int j=0; j<seedList.length; j++){
                if (seedList[j] == cSeed){
                    add = false;
                    break;
                }
            }
            if (add){
                seedList[ind++] = cSeed;
            }
        }
        return ind;
    }
    
    private int getCellIndexFromPos(int p[]){ //returns the index at which the cell is in the maze data array that has the position given
        int point = s+1;
        for (int i=0; i<(mD.length); i++){
            int cPos[] = mD[i].getPos();
            if (cPos[0] == p[0] && cPos[1] == p[1]){//x==x and y==y
                point = i;
            }
        }
        return point;
    }
    
    private int getCellSeedFromPos(int p[]){
        int cellIndex = getCellIndexFromPos(p);
        return mD[cellIndex].getSeed();
    }
    
    private void delWall(int dir[], int p[]){ //makes the maze visible/navigable
        int nP[] = {clamp(p[0] + dir[0]), clamp(p[1] + dir[1])};
        if (nP[0] != 0 && nP[0] != (s-1)){
            if (nP[1] != 0 && nP[1] != (s-1)){
                mV[nP[0]][nP[1]] = empt;
            }
        }
    }
    
    private int clamp(int var){
        int n = Math.max(0, Math.min((s-1), var)); //clamps between 0 and the "size" of the maze
        return n;
    }
    
    private int[] randCellPos(){
        int xInd = 1 + (int)(Math.random()*((s-1)/2))*2;
        int yInd = 1 + (int)(Math.random()*((s-1)/2))*2;
        int rPos[] = {xInd, yInd};
        int cellIndForCheck = getCellIndexFromPos(rPos);
        if (mD[cellIndForCheck].useable != true){
            for (int i=0; i<mD.length; i++){
                if (mD[i].useable = true){
                    rPos[0] = mD[0].getPos()[0];
                    rPos[1] = mD[1].getPos()[1];
                    break;
                }
            }
        }
        if (getNextDir(rPos)[0] == -1){
            return randCellPos();
        }
        return rPos;
    }
    
    private int pairing(int x, int y){ //outputs a unique int from 2 ints, useful for seed application for kruskal
        double cantor = (0.5)*(x+y)*(x+y+1)+y;
        return (int)cantor; //cantor is the function name (thanks wikipedia)
    }
}
