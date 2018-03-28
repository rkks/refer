package com.enterprisej2me.iFeedBack.surveyserver;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.util.*;

import javax.naming.*;
import java.sql.*;
import javax.sql.DataSource;

public class Server extends HttpServlet {

  protected static final String DBName =
                                "jdbc/Cloudscape";
  private DataSource dataSource;

  private String userid;

  public void init () throws ServletException {
    try {
      InitialContext ic = new InitialContext();
      dataSource = (DataSource) ic.lookup(DBName);
    } catch (Exception e) {
      e.printStackTrace();
      throw new ServletException("init error");
    }
  }

  public void doPost(HttpServletRequest request,
                     HttpServletResponse response)
      throws ServletException, IOException {

    response.setContentType("application/binary");
    try {
      InputStream in =
           (InputStream) request.getInputStream();
      OutputStream out =
           (OutputStream) response.getOutputStream();
      DataInputStream din = new DataInputStream(in);
      DataOutputStream dout =
                          new DataOutputStream(out);

      int opcode = din.readInt();
      String securityToken = din.readUTF();
      if ( !Auth.authenticate(securityToken) ) {
        // dout.writeUTF("Status: Invalid Token");
        dout.writeBoolean(false);
        dout.writeUTF("");
        dout.flush();
        dout.close();
        din.close();
        return;
      }
      userid = securityToken.substring(0, securityToken.indexOf(":"));

      if ( opcode == 0 ) {
        getQuestion(din, dout);
      } else if ( opcode == 1 ) {
        submitAnswer(din, dout);
      }

    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  public void doGet(HttpServletRequest request,
                       HttpServletResponse response)
      throws ServletException, IOException {

    response.setContentType("text/html");
    try {
      ServletOutputStream out =
                          response.getOutputStream();
      out.print("<html><body>");

      Connection c = dataSource.getConnection();
      PreparedStatement ps = c.prepareStatement(
                        "SELECT *"
                        + " FROM iFeedBackAs ORDER BY ts");
      ResultSet rs = ps.executeQuery();

      java.sql.Timestamp ts;
      String userid, qid, answer, comments;
      while (rs.next()) {
        userid = rs.getString(1);
        qid = rs.getString(2);
        answer = rs.getString(3);
        comments = rs.getString(4);
        ts = rs.getTimestamp(5);
        Calendar cal = Calendar.getInstance();
        cal.setTime(ts);
        out.print("<p><b>" +
            cal.get(Calendar.DAY_OF_MONTH) + "-" +
            cal.get(Calendar.MONTH) + "-" +
            cal.get(Calendar.YEAR) + "- " +
            cal.get(Calendar.HOUR_OF_DAY) + ":" +
            cal.get(Calendar.MINUTE) + ":" +
            cal.get(Calendar.SECOND) + "</b><br>" +
            userid + " : " + qid + " : " + answer + " : " + comments);
      }
      rs.close();
      ps.close();
      c.close();
      out.print("</body></html>");
      out.close();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }



  private void getQuestion( DataInputStream din, DataOutputStream dout ) throws Exception {
    Connection c = dataSource.getConnection();
    PreparedStatement ps = c.prepareStatement(
                             "SELECT * FROM iFeedBackQs"
                             + " WHERE visible=true");
    ResultSet rs = ps.executeQuery();
    // Only use the first one
    rs.next();
    int qid = rs.getInt(1);
    String question = rs.getString(2);
    String choices = rs.getString(3);
    ps.close();
    c.close();

    // Auth is valid
    dout.writeBoolean(true);
    dout.writeInt(qid);
    dout.writeUTF(question);
    StringTokenizer st = new StringTokenizer(choices, ":");
    // Number of choices
    dout.writeInt( st.countTokens() );
    // Choices
    while ( st.hasMoreTokens() ) {
      dout.writeUTF( st.nextToken() );
    }
    dout.flush();
  }

  private void submitAnswer( DataInputStream din, DataOutputStream dout ) throws Exception {
    long timestamp = din.readLong();
    Timestamp ts = new Timestamp(timestamp);
    int qid = din.readInt();
    String answer = din.readUTF();
    String comments = din.readUTF();

    Connection c = dataSource.getConnection();
    PreparedStatement ps = c.prepareStatement(
                             "INSERT into iFeedBackAs"
                             + " (userid, qid, answer, comments, ts)"
                             + " VALUES (?, ?, ?, ?, ?)");
    ps.setString(1, userid);
    ps.setInt(2, qid);
    ps.setString(3, answer);
    ps.setString(4, comments);
    ps.setTimestamp(5, ts);

    // Auth is valid
    dout.writeBoolean(true);
    if ( ps.executeUpdate() != 1 ) {
      ps.close();
      c.close();
      dout.writeBoolean(false);
      dout.writeUTF("");
      dout.flush();
      dout.close();
      return;
    }
    ps.close();
    c.close();
    dout.writeBoolean(true);
    dout.writeUTF("");
    dout.flush();
  }
}

