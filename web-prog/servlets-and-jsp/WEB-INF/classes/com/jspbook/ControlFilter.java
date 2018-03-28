package com.jspbook;

import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;

/**
 * Changelog
 *  (1.1) The default control class was 'default' which is a reserved word in Java. It was changed to 'index', which works better as index.jsp is usually the default page of a web applicaiton.
 * 
 */
public class ControlFilter implements Filter {
  protected FilterConfig config = null;

  public void init(FilterConfig filterConfig) {
    config = filterConfig;
  }

  public void doFilter ( ServletRequest req, ServletResponse res,
                         FilterChain chain ) throws IOException, ServletException {
    if (!(req instanceof HttpServletRequest)) {
      throw new ServletException("Filter requires a HTTP request.");
    }

    // determine name of implicit control component.
    HttpServletRequest request = (HttpServletRequest)req;
    HttpServletResponse response = (HttpServletResponse)res;
    String uri = request.getRequestURI();
    
    // redirect directory requests -- fix, this is slightly inefficient
    if (uri==null||uri.equals("")||uri.equals("/")) {
      response.sendRedirect("http://www.jspbook.com/index.jsp");
      return;
    }
    
    int start = uri.lastIndexOf("/")+1;
    int stop = uri.lastIndexOf(".");
    String name = "index"; // 1.1 'index' is now the default logic class
//    String name = "default"; // 1.0 'default' is a reserved Java word
    if (start < stop) {
      name = uri.substring(start, stop);
    }
    boolean doFilter = true;
    
    // try to load and run an implicit MVC control component.
    try {
      Object o = Class.forName("com.jspbook."+name).newInstance();
      if (!(o instanceof Control)) {
        throw new ServletException("Class com.jspbook."+name+" does not implement com.jspbook.Control");
      }
      
      Control control = (Control)o;
      doFilter = control.doLogic(request, response);
    }
    catch (ClassNotFoundException e) {
      //ignore
    }
    catch (InstantiationException e) {
      throw new ServletException(e);
    }
    catch (IllegalAccessException e) {
      throw new ServletException(e);
    }
    
    // do whatever is next
    if (doFilter) {
      chain.doFilter(request, response);
    }
  }
  
  public void destroy() {
    // noop
  }
}
