package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;

public class feedback extends AbstractContent {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
                      throws ServletException, IOException {
    return doLogic(request, response, "feedback");
  }
}
