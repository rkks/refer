package com.jspbook;

import java.io.*;
import java.util.logging.*;
import javax.servlet.*;
import javax.servlet.http.*;

class AuditRequestWrapper extends HttpServletRequestWrapper {
  Logger logger;
  public AuditRequestWrapper(Logger logger,
      HttpServletRequest request) {
    super(request);
    this.logger = logger;
  }

  public String getContentType() {
    String contentType = super.getContentType();
    logger.fine("ContentType[ " + contentType + "]");
    return contentType;
  }

  public int getContentLength() {
    int contentLength = super.getContentLength();
    logger.fine("getContentLength[" + contentLength + "]");
    return contentLength;
  }

  public long getDateHeader(String s) {
    long date =  super.getDateHeader(s);
    logger.fine("getDateHeader[" + s + ": " + date + "]");
    return date;
  }

  public String getHeader(String s) {
    String header = super.getHeader(s);
    logger.fine("getHeader[" + s + ": " + header + "]");
    return header;
  }

  public int getIntHeader(String s) {
    int header = super.getIntHeader(s);
    logger.fine("getIntHeader[" + s + ": " + header + "]");
    return header;
  }

  public String getQueryString() {
    String queryString = super.getQueryString();
    logger.fine("getQueryString[" + queryString + "]");
    return queryString;
  }
  // other methods left out for clarity
}
