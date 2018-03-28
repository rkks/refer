package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class FileUpload extends HttpServlet {
  public void doPost(HttpServletRequest request,
                     HttpServletResponse response)
  throws IOException, ServletException {

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    
    out.println("<html>");
    out.print("File upload success. <a href=\"/jspbook/files/");
    out.print("\">Click here to browse through all uploaded ");
    out.println("files.</a><br>");

    ServletInputStream sis = request.getInputStream();
    StringWriter sw = new StringWriter();
    int i = sis.read();
    for (;i!=-1&&i!='\r';i=sis.read()) {
      sw.write(i);
    }
    sis.read(); // ditch '\n'
    String delimiter = sw.toString();

    int count = 0;
    while(true) {
      StringWriter h = new StringWriter();
      int[] temp = new int[4];
      temp[0] = (byte)sis.read();
      temp[1] = (byte)sis.read();
      temp[2] = (byte)sis.read();
      h.write(temp[0]);
      h.write(temp[1]);
      h.write(temp[2]);
      // read header
      for (temp[3]=sis.read();temp[3]!=-1;temp[3]=sis.read()) {
        if (temp[0] == '\r' &&
            temp[1] == '\n' &&
            temp[2] == '\r' &&
            temp[3] == '\n') {
          break;
        }
        h.write(temp[3]);
        temp[0] = temp[1];
        temp[1] = temp[2];
        temp[2] = temp[3];
      }
      String header = h.toString();

      int startName = header.indexOf("name=\"");
      int endName = header.indexOf("\"",startName+6);
      if (startName == -1 || endName == -1) {
        break;
      }
      String name = header.substring(startName+6, endName);
      if (name.equals("file")) {
        startName = header.indexOf("filename=\"");
        endName = header.indexOf("\"",startName+10);
        String filename = header.substring(startName+10,endName);
        // trim out full path info if it is included in filename (IE includes it)
        int slash = filename.lastIndexOf("\\");
	if (slash != -1) {
	  filename = filename.substring(slash + 1);
        }
        // get a reference to ServletContext so we can translate URIs
        ServletContext sc = request.getSession().getServletContext();
        // get a reference to the 'files' directory so we can save the file.
        File file = new File(sc.getRealPath("/files"));
        file.mkdirs();
        FileOutputStream fos =
          new FileOutputStream(sc.getRealPath("/files")+"/"+filename);

        // write whole file to disk
        int length = 0;
        delimiter = "\r\n"+delimiter;
        byte[] body = new byte[delimiter.length()];
        for (int j=0;j<body.length;j++) {
          body[j] = (byte)sis.read();
        }
        // check it wasn't a 0 length file
        if (!delimiter.equals(new String(body))) {
          int e = body.length-1;
          i=sis.read();
          for (;i!=-1;i=sis.read()) {
            fos.write(body[0]);
            for (int l=0;l<body.length-1;l++) {
              body[l]=body[l+1];
            }
            body[e] = (byte)i;
            if (delimiter.equals(new String(body))) {
              break;
            }
            length++;
          }
        }

        fos.flush();
        fos.close();
      }
      if (sis.read() == '-' && sis.read() == '-') {
        break;
      }
    }
    out.println("</html>");
  }
  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
  throws IOException, ServletException {
    doPost(request, response);
  }
}
