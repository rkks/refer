import javax.microedition.rms.*;
import java.util.Enumeration;
import java.io.*;

public class StockDB {
   RecordStore recordStore = null;
   public StockDB() {}

   // Open a record store with the given name
   public StockDB(String fileName) {
      try {
         recordStore = RecordStore.openRecordStore(fileName, true);
      } catch(RecordStoreException rse) {
         rse.printStackTrace();
      }
   }

   // Close the record store
   public void close() throws RecordStoreNotOpenException,RecordStoreException {
      if (recordStore.getNumRecords() == 0) {
         String fileName = recordStore.getName();
         recordStore.closeRecordStore();
         recordStore.deleteRecordStore(fileName);
      } else {
         recordStore.closeRecordStore();
      }
   }

   // Add a new record (stock) 
   // to the record store
   public synchronized void addNewStock(String record) {
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      DataOutputStream outputStream = new DataOutputStream(baos);
      try {
         outputStream.writeUTF(record);
      } catch (IOException ioe) {
         System.out.println(ioe);
         ioe.printStackTrace();
      }
      byte[] b = baos.toByteArray();
      try {
         recordStore.addRecord(b, 0, b.length);
      } catch (RecordStoreException rse) {
         System.out.println(rse);
         rse.printStackTrace();
      }
   }

   // Enumerate through the records.
   public synchronized RecordEnumeration enumerate() throws
                       RecordStoreNotOpenException {
      return recordStore.enumerateRecords(null, null, false);
   }
}
