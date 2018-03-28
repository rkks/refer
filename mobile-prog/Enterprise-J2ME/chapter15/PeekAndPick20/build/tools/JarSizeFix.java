import java.io.*;
import java.util.*;

import org.apache.tools.ant.*;

public final class JarSizeFix extends Task {
  private File mJar, mJad;
  
  private List mLines;

  public JarSizeFix() {}
  
  public void setJad(File jad) { mJad = jad; }
  public void setJar(File jar) { mJar = jar; }
  
  public void execute() throws BuildException {
    try {
      long size = mJar.length();
      long jadSize = 0;
      
      mLines = new ArrayList();
      
      // Read every line of the Jad.
      BufferedReader in =
          new BufferedReader(new FileReader(mJad));
      String line;
      while ((line = in.readLine()) != null) {
        mLines.add(line);
        // Parse the JAR size.
        if (line.startsWith("MIDlet-Jar-Size:")) {
          String sizeString = line.substring(17);
          jadSize = Long.parseLong(sizeString);
        }
      }
      in.close();

      // If the actual size and reported size are the
      // same, we don't have to do anything.
      if (size == jadSize) return;
      
      System.out.println("MIDlet-Jar-Size: " + size);
      
      // Write out all the same lines.
      PrintWriter out =
          new PrintWriter(new FileWriter(mJad));
      Iterator i = mLines.iterator();
      while (i.hasNext()) {
        line = (String)i.next();
        // Modify the JAR size.
        if (line.startsWith("MIDlet-Jar-Size:")) {
          out.print("MIDlet-Jar-Size: ");
          out.println(size);
        }
        else
          out.println(line);
      }
      out.close();
    }
    catch (NumberFormatException nfe) {
      throw new BuildException(nfe);
    }
    catch (IOException ioe) {
      throw new BuildException(ioe);
    }
  }
}
