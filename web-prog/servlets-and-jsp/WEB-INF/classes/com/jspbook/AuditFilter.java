package com.jspbook;

import java.io.*;
import java.util.logging.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class AuditFilter implements javax.servlet.Filter {

  public void doFilter(ServletRequest req, ServletResponse res,
      FilterChain chain) throws IOException, ServletException {
    Logger logger = SiteLogger.getLogger();
    AuditRequestWrapper wrappedRequest =
      new AuditRequestWrapper(logger, (HttpServletRequest)req);
    chain.doFilter(wrappedRequest, res);
  }

  public void init(FilterConfig filterConfig) {
    // noop
  }

  public void destroy() {
    // noop
  }
}
