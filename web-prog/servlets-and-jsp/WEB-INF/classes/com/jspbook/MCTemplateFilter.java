package com.jspbook;

import java.io.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class MCTemplateFilter implements Filter {
  FilterConfig fc = null;

  public void doFilter(ServletRequest req, ServletResponse res,
    FilterChain chain) throws IOException, ServletException {

    HttpServletRequest request = (HttpServletRequest) req;
    String uri = request.getRequestURI();
    String client = "HTML";
    if (uri.endsWith("xhtml")) {
      client = "XHTML";
    }
    
    // business logic...
    HashMap user = new HashMap();
    user.put("name", "Bruce");
    request.setAttribute("user", user);

    ServletContext sc = fc.getServletContext();
    sc.getRequestDispatcher("/mc/MCexample"+client+".jsp").forward(request, res);
    
  }

  public void init(FilterConfig filterConfig) {
    fc = filterConfig;
  }

  public void destroy() {
    fc = null;
  }
}
