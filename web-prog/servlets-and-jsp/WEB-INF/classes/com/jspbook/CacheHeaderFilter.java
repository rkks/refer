package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.util.*;

public class CacheHeaderFilter implements Filter {
  ServletContext sc;
  FilterConfig fc;
  String cacheControl = null;

  public void doFilter(ServletRequest req, ServletResponse res,
                       FilterChain chain)
                       throws IOException, ServletException {
    HttpServletRequest request = (HttpServletRequest) req;
    HttpServletResponse response = (HttpServletResponse) res;

    // Allow for selective non-caching, e.g. if you have a dynamically generated JPG
    String r = sc.getRealPath("");
    String path = fc.getInitParameter(request.getRequestURI());
    if (path!= null && path.equals("nocache")) {
      chain.doFilter(request, response);
      return;
    }
    path = r+path;

    // set appropriate HTTP Cache-Control parameters
    if (cacheControl != null) {
      response.addHeader("Cache-Control", cacheControl);
    }

    // pass the request/response on
    chain.doFilter(request, response);
  }

  public void init(FilterConfig filterConfig) {
    this.fc = filterConfig;

    // set reference to servlet context
    this.sc = filterConfig.getServletContext();

    // set client-side caching properties - http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.9
    cacheControl = fc.getInitParameter("Cache-Control");
  }

  public void destroy() {
    this.sc = null;
    this.fc = null;
  }
}
