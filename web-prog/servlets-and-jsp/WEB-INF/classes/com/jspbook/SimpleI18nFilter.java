package com.jspbook;

import java.io.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class SimpleI18nFilter implements Filter {
  protected static FilterConfig config = null;

  public void init(FilterConfig filterConfig) {
    config = filterConfig;
  }

  public void destroy() {
    config = null;
  }

  public void doFilter(ServletRequest req,
                       ServletResponse res,
                       FilterChain chain)
  throws IOException, ServletException {
    if (res instanceof HttpServletResponse) {
      HttpServletRequest request = (HttpServletRequest)req;
      HttpServletResponse response = (HttpServletResponse)res;

      String uri = request.getRequestURI();
      if (uri.endsWith("/")) {
        chain.doFilter(request, response);
        return;
      }
      String path = request.getContextPath();
      uri = uri.substring(path.length(), uri.length());
      uri = getLanguage(uri, request);
      ServletContext sc = config.getServletContext();
      sc.getRequestDispatcher(uri).forward(request, response);
    }
  }

  public static String getLanguage(String uri,
                                   HttpServletRequest request) {
    ServletContext sc = config.getServletContext();
    Enumeration enum = request.getLocales();
    String postfix = 
      uri.substring(uri.lastIndexOf("."), uri.length());
    String temp = uri.substring(0,uri.length()-postfix.length());
    while(enum.hasMoreElements()) {
      Locale l = (Locale)enum.nextElement();
      String lang = l.getISO3Language();
      String file = sc.getRealPath(temp+"-"+lang+postfix);
      System.out.println("Trying: "+file);
      File f = new File(file);
      if (f.exists()) {
        return temp+"-"+lang+postfix;
      }
    }
    return uri;
  }
}
