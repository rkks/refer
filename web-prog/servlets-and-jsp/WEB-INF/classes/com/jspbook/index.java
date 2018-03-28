package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;

public class index extends AbstractContent {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
                      throws ServletException, IOException {
    doLogic(request, response, "news");
    return true;
  }
}
