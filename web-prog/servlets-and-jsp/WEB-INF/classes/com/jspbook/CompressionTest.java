package com.jspbook;

import java.io.*;
import java.net.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class CompressionTest extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
  throws IOException, ServletException {

    response.setContentType("text/html");
    PrintWriter out = response.getWriter();
    out.println("<html>");
    out.println("<head>");
    out.println("<title>Compression Test</title>");
    out.println("</head>");
    out.println("<body>");
    out.println("<h1>Compression Test</h1>");
    out.println("<form>");
    String url = request.getParameter("url");
    if (url != null) {
      out.print("<input size=\"50\" name=\"url\" ");
      out.println("value=\""+url+"\">");
    } else {
      out.println("<input size=\"50\" name=\"url\">");
    }
    out.print("<input type=\"submit\" value=\"Check\">");
    out.println("</form>");
    out.println("URL: "+ url);
    if (url != null) {
      URL noCompress = new URL(url);
      HttpURLConnection huc =
        (HttpURLConnection)noCompress.openConnection();
      huc.setRequestProperty("user-agent","Mozilla(MSIE)");
      huc.connect();
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      InputStream is = huc.getInputStream();
      while(is.read() != -1) {
        baos.write((byte)is.read());
      }
      byte[] b1 = baos.toByteArray();

      URL compress = new URL(url);
      HttpURLConnection hucCompress =
        (HttpURLConnection)noCompress.openConnection();
      hucCompress.setRequestProperty("accept-encoding","gzip");
      hucCompress.setRequestProperty("user-agent","Mozilla(MSIE)");
      hucCompress.connect();
      ByteArrayOutputStream baosCompress =
        new ByteArrayOutputStream();
      InputStream isCompress = hucCompress.getInputStream();
      while(isCompress.read() != -1) {
        baosCompress.write((byte)isCompress.read());
      }
      byte[] b2 = baosCompress.toByteArray();

      out.print("<pre>");
      out.println("Uncompressed: " + b1.length);
      out.println("Compressed: " + b2.length);
      out.print("Space saved: "+(b1.length-b2.length)+", or ");
      out.println((((b1.length - b2.length)*100)/b1.length)+"%");
      out.println("Downstream(2kbps)");
      out.println(" No GZIP: "+(float)b1.length/2000+"seconds");
      out.println(" GZIP:    "+(float)b2.length/2000+"seconds");
      out.println("Downstream(5kbps)");
      out.println(" No GZIP: "+(float)b1.length/5000+"seconds");
      out.println(" GZIP:    "+(float)b2.length/5000+"seconds");
      out.println("Downstream(10kbps)");
      out.println(" No GZIP: "+(float)b1.length/10000+"seconds");
      out.println(" GZIP:    "+(float)b2.length/10000+"seconds");
      out.println("</pre>");
    }
    out.println("</body>");
    out.println("</html>");
  }
}
