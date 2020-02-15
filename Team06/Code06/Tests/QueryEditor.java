import java.util.Scanner;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.FileNotFoundException;

class QueryEditor {

  private static int queryNo = 0;
  private static final String template = "Select ";
  private static final String FILE_NAME = "pattern-0-queries.txt";

  public static void main(String[] args) {
    File file = new File(FILE_NAME);
    try {
      file.createNewFile();
      Scanner scanner = new Scanner(System.in);
      System.out.println("Works");
      boolean exit = false;
      
      // use in append mode
      FileWriter fw = new FileWriter(file, true);

      // each string is a line in the 5-line query
      String[] query = new String[]{"# comment", "# decl", "# select", "# answer", "# time limit"};

      fw.flush();

      // Initialize:
      System.out.println("Welcome to Query Editor!\n"
        + "Type 'exit' to quit.");
      String userInput;
      String selectCl;

      while (!exit) {
        // user keeps entering query inputs line by line

        // starts at Query Number 1;
        queryNo++;

        System.out.println("Now entering query #" + queryNo + ":");
        // System.out.println("Using template ")

        System.out.println("Select <enter result-cl>:");
        userInput = scanner.nextLine();

        if (userInput.equalsIgnoreCase("exit")) {
          exit = true;
          queryNo--;
          System.out.println("Last query entered: #" + queryNo);
          break;
        }

        boolean done = false;
        selectCl = "Select " + userInput + " ";
        
        // Start new query
        while (!done) {
          System.out.println("\nCurrently: "+ selectCl);
          System.out.println(selectCl + "<enter ['st' = suchthat-cl] | ['p' = pattern-cl] | 'done'>");
          userInput = scanner.nextLine();
  
          if (userInput.equals("st")) {
            selectCl += "such that ";
            System.out.println(selectCl + "<enter relRef> ( <argument 1> , <argument 2> )");
            String[] split = scanner.nextLine().split(" ");
            
            if (split.length != 3) {
              System.out.println("enter 3 words only");
              continue;
            }

            selectCl += split[0] + "(" + split[1] + ", " + split[2] + ") ";

          } else if (userInput.equals("p")) {
            selectCl += "pattern ";
            System.out.println(selectCl + "<enter assign variable> ( <argument 1> ,  <argument 2> )");
            String[] split = scanner.nextLine().split(" ");
            
            if (split.length != 3) {
              System.out.println("enter 3 words only");
              continue;
            }

            selectCl += split[0] + "(" + split[1] + ", " + split[2] + ") ";

          } else if (userInput.equals("done")) {
             done = true;
          } else {
            System.out.println("Input does not match query grammar / spelling error");
          }
        }
        System.out.println("==> " + selectCl);
        query[2] = selectCl;

        // Declaration Input
        System.out.println("Enter declaration-cl (in 1 line):");
        query[1] = scanner.nextLine();
        // Answer Input
        System.out.println("Enter all answers (1 per line):\n"
          + "'none' for None"
          + "'done' for done");
        String answer = "";

        while (true) {
          userInput = scanner.nextLine();
          if (userInput.equals("done") && !answer.equals("")) {
            answer = answer.substring(0, answer.length() - 1);
            System.out.println("Answer is: " + answer);
            query[3] = answer;
            break;
          }
          if (userInput.equals("none")) {
            System.out.println("Answer is: none");
            query[3] = "None";
            break;
          }
          answer += userInput + ", ";
        }
          
        query[4] = "5000";
        System.out.println("Enter Query comments (in 1 line): ");
        query[0] = queryNo + " - " + scanner.nextLine();

        System.out.println("Query generated:");

        for (int i = 0; i < 5; i++) {
          System.out.println(query[i]);
          fw.write(query[i] + "\n");
        }

        // TODO: User can delete queries by number

        // TODO: User can display queries by number

        // TODO: User can re-number all queries
      }

      fw.flush();

    } catch (FileNotFoundException e) {

    } catch (IOException e) {
      System.out.println("IO Excepetion");
    }

  }
}
