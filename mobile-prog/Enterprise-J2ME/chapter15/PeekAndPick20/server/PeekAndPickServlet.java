import java.io.*;
import java.text.*;
import java.util.*;

import javax.mail.*;
import javax.mail.internet.*;
import javax.servlet.http.*;
import javax.servlet.*;

public class PeekAndPickServlet extends HttpServlet {
  private DateFormat mDateFormat;
  private String mMailServer;
  
  public void init() {
    mDateFormat = new SimpleDateFormat("yyyy.MM.dd HH:mm:ss");
    mMailServer = getInitParameter("PeekAndPick.mailServer");
    timeLog("init() completed");
  }
  
  public void doPost(HttpServletRequest request,
      HttpServletResponse response)
      throws ServletException, IOException {
    
    // Open up the posted content and read it.
    BufferedReader in = request.getReader();
    
    // Get the server and destination address.
    String log = in.readLine();
    String address = in.readLine();
    
    StringBuffer content = new StringBuffer();
    String line = null;
    while ((line = in.readLine()) != null) {
      content.append(line);
      content.append('\n');
    }

    // Default response message.
    String message = "100 ok";

    try {    
      sendMail(mMailServer, address, content.toString());
    }
    catch (Throwable t) {
      message = "200 " + t.toString();
    }
    
    timeLog(address + ": " + message);
    
    // Send back a response.
    response.setContentType("text/plain");
    response.setContentLength(message.length());
    PrintWriter out = response.getWriter();
    out.println(message);
    out.flush();
  }
  
  public void destroy() {
    timeLog("destroy() completed");
  }

  private void sendMail(String server, String address,
      String content) throws Exception {
    // Send the e-mail.
    Properties p = new Properties();
    p.put("mail.smtp.host", server);
    
    Session s = Session.getDefaultInstance(p, null);
    
    InternetAddress from = new InternetAddress("PeekAndPick <" + address + ">");
    InternetAddress to = new InternetAddress(address);
    
    MimeMessage m = new MimeMessage(s);
    m.setFrom(from);
    m.addRecipient(Message.RecipientType.TO, to);
    String now = mDateFormat.format(new java.util.Date());
    m.setSubject("Mail from PeekAndPick [" + now + "]");
    m.setText(content.toString());
    
    Transport.send(m);
  }
  
  private void timeLog(String s) {
    Date now = new Date();
    log("[" + mDateFormat.format(now) + "] " + s);
  }
}
