import java.util.Scanner;
import java.io.File;
import java.io.IOException;
import java.io.FileNotFoundException;

class QueryEditor {
  public static void main(String[] args) {
    File file = new File("C:\\Users\\User\\Source\\Repos\\team06-win-spa-19s2\\Team06\\Code06\\Tests\\given-3-queries.txt");
    
    try {
      Scanner scanner = new Scanner(file);
      System.out.println("Works");

    } catch (FileNotFoundException e) {

    }

  }
}
