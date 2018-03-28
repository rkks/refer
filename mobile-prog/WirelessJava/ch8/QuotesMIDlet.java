import javax.microedition.rms.*;
import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import javax.microedition.io.*;
import java.io.*;

public class QuotesMIDlet extends MIDlet implements CommandListener {
   Display display = null;
   List menu = null; // main menu
   List choose = null;
   TextBox input = null;
   Ticker ticker = new Ticker("Database Application");
   String quoteServer = 
   "http://quote.yahoo.com/d/quotes.csv?s=";
   String quoteFormat = "&f=slc1wop"; // The only quote format supported

   static final Command backCommand = new Command("Back", Command.BACK, 0);
   static final Command mainMenuCommand = new Command("Main", Command.SCREEN, 1);
   static final Command saveCommand = new Command("Save", Command.OK, 2);
   static final Command exitCommand = new Command("Exit", Command.STOP, 3);
   String currentMenu = null;

   // Stock data
   String name, date, price;
    
   // record store
   StockDB db = null;

   public QuotesMIDlet() { // constructor
   }

   // start the MIDlet
   public void startApp() throws MIDletStateChangeException {
      display = Display.getDisplay(this);
      // open a db stock file
      try {
         db = new StockDB("mystocks");
      } catch(Exception e) {}
         menu = new List("Stocks Database", Choice.IMPLICIT);
         menu.append("List Stocks", null);
         menu.append("Add A New Stock", null);
         menu.addCommand(exitCommand);
         menu.setCommandListener(this);
         menu.setTicker(ticker);

         mainMenu();
   }

   public void pauseApp() {
      display = null;
      choose = null;
      menu = null;
      ticker = null;
       
      try {
         db.close();
         db = null;
      } catch(Exception e) {}
   }

   public void destroyApp(boolean unconditional) {
      try {
         db.close();
      } catch(Exception e) {}
      notifyDestroyed();
   }

   void mainMenu() {
      display.setCurrent(menu);
      currentMenu = "Main"; 
   }

   // Construct a running ticker 
   // with stock names and prices
   public String tickerString() {
      StringBuffer ticks = null;
      try {
         RecordEnumeration enum = db.enumerate();
         ticks = new StringBuffer();
         while(enum.hasNextElement()) {
            String stock1 = new String(enum.nextRecord());
            ticks.append(Stock.getName(stock1));
            ticks.append(" @ ");
            ticks.append(Stock.getPrice(stock1));
            ticks.append("    ");
         }
      } catch(Exception ex) {}
         return (ticks.toString());
   }

   // Add a new stock to the record store 
   // by calling StockDB.addNewStock()
   public void addStock() {
      input = new TextBox("Enter a Stock Name:", "", 5, TextField.ANY);
      input.setTicker(ticker);
      input.addCommand(saveCommand);
      input.addCommand(backCommand);
      input.setCommandListener(this);
      input.setString("");
      display.setCurrent(input);
      currentMenu = "Add";
   }

   // Connect to quote.yahoo.com and 
   // retrieve the data for a given 
   // stock symbol.
   public String getQuote(String input) throws IOException, NumberFormatException {
      String url = quoteServer + input + quoteFormat;
      StreamConnection c = (StreamConnection)Connector.open(
                            url, Connector.READ_WRITE);
      InputStream is = c.openInputStream();
      StringBuffer sb = new StringBuffer();
      int ch;
      while((ch = is.read()) != -1) {
        sb.append((char)ch);
      }
      return(sb.toString());
   }
   
   // List the stocks in the record store
   public void listStocks() {
      choose = new List("Choose Stocks", Choice.MULTIPLE);
      choose.setTicker(new Ticker(tickerString()));
      choose.addCommand(backCommand);
      choose.setCommandListener(this);
      try {
         RecordEnumeration re = db.enumerate();
         while(re.hasNextElement()) {
            String theStock = new String(re.nextRecord());
            choose.append(Stock.getName(theStock)+" @ " +
                          Stock.getPrice(theStock),null);
         }
      } catch(Exception ex) {}
      display.setCurrent(choose);
      currentMenu = "List"; 
   }
 
   // Handle command events 
   public void commandAction(Command c, Displayable d) {
      String label = c.getLabel();
      if (label.equals("Exit")) {
         destroyApp(true);
      } else if (label.equals("Save")) {
         if(currentMenu.equals("Add")) {
            // add it to database
            try {
               String userInput = input.getString();
              String pr = getQuote(userInput);
              db.addNewStock(pr);
              ticker.setString(tickerString()); 
            } catch(IOException e) {
            } catch(NumberFormatException se) {
            }
            mainMenu();
          } 
      } else if (label.equals("Back")) {
         if(currentMenu.equals("List")) {
            // go back to menu
            mainMenu();
         } else if(currentMenu.equals("Add")) {
            // go back to menu
            mainMenu();
         }
      } else {
         List down = (List)display.getCurrent();
         switch(down.getSelectedIndex()) {
            case 0: listStocks();break;
            case 1: addStock();break;
         }
      }
   }
}
