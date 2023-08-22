//Designed originally in 2019
class Main {
  public static void main(String[] args) {
    Maze generator = new Maze(35); //input the size here
    generator.newGeneration();
    generator.printMaze();
  }
}