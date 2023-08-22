public class Cell
{
    private int seed;
    private int pos[] = new int[2];
    public boolean useable;
    
    public Cell(int sd, int p[])
    {
        seed = sd; //Seed Instanciation for kruskal algorithm, can use this.seed for easier readability but oh well
        pos[0] = p[0];  //Position Instanciation
        pos[1] = p[1];  //For easier getting
        useable = true;
    }
    
    public int getSeed()
    {
        return seed;
    }
    
    public int[] getPos(){
        return pos;
    }
    
    public void setSeed(int nSeed)
    {
        seed = nSeed;
    }
}