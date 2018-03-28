package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class I18nHelloWorld extends HttpServlet {

  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
      response.setContentType("text/html; charset=UTF-8");
      ServletOutputStream sos = response.getOutputStream();
      PrintWriter out =
        new PrintWriter(new OutputStreamWriter(sos,"UTF-8"), true);
      out.println("<html>");
      out.println("<head>");
      out.println("<title>i18n Hello World!</title>");
      out.println("</head>");
      out.println("<body>");
      out.println("<h1>\u4f60\u597d\u4e16\u754c!</h1>"); // Chinese
      out.println("<h1>Hello World!</h1>"); // English
      out.println("<h1>Bonjour Monde!</h1>"); // French
      out.println("<h1>Hallo Welt!</h1>"); // German
      out.println("<h1>Ciao Mondo!</h1>"); // Italian
      out.println("<h1>\u3053\u3093\u306b\u3061\u306f\u4e16\u754c!</h1>"); //Japanese
      out.println("<h1>\uc5ec\ubcf4\uc138\uc694 \uc138\uacc4 !</h1>"); // Korean
      out.println("<h1>\u00a1Hola Mundo!</h1>");// Spanish
      out.println("</body>");
      out.println("</html>");
  }
}
