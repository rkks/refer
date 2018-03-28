public class Stock {
   private static String name, time, price;
   // Given a quote from the server, 
   // retrieve the name, 
   //price, and date of the stock
   public static void parse(String data) {
      int index = data.indexOf('"'); 
      name = data.substring(++index,(index = data.indexOf('"', index)));
      index +=3;
      time = data.substring(index, (index = data.indexOf('-', index))-1);
      index +=5;
      price = data.substring(index, (index = data.indexOf('<', index)));
   }

   // Get the name of the stock from 
   // the record store
   public static String getName(String record) {
      parse(record);
      return(name);
   }
  
   // Get the price of the stock from 
   // the record store
   public static String getPrice(String record) {
      parse(record);
      return(price);
   }
}
