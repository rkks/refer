package com.jspbook;

import java.util.*;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class ShowForm extends HttpServlet {

  public void doPost(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {

    response.setContentType("text/plain");
    PrintWriter out = response.getWriter();

    ServletInputStream sis = request.getInputStream();
    for (int i = sis.read(); i != -1; i = sis.read()) {
        out.print((char)i);
    }
  }
  public void doGet(HttpServletRequest request,
                    HttpServletResponse response)
    throws IOException, ServletException {
    doPost(request, response);
  }
}
