package com.jspbook;

import java.io.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;

public class LinkTrackerFilter implements Filter {
  Calendar startDate = Calendar.getInstance();
  static int count = 0;
  public static Hashtable requests = new Hashtable();
  public static Hashtable responses = new Hashtable();
  public static Hashtable referers = new Hashtable();
  FilterConfig fc = null;

  public void doFilter(ServletRequest req, ServletResponse res,
                       FilterChain chain) throws IOException, ServletException {
    HttpServletRequest request = (HttpServletRequest) req;
    HttpServletResponse response = (HttpServletResponse) res;

    String uri = request.getRequestURI();
    String path = request.getContextPath();
//    if (path.length() != 0) {
      String turi = uri.substring(path.length(), uri.length());
//    }

    if (turi.startsWith("/redirect")) {
      String url = request.getParameter("url");
      // error check!
      if (url == null || url.equals("")) {
        response.sendRedirect(path);
        return;
      }
      Link l = new Link();
      l.url = url;
      l.count = 1;
      l.lastVisited = Calendar.getInstance().getTimeInMillis();
      if (responses.get(l.url)!=null){
        l = (Link)responses.get(l.url);
        l.count++;
      }
      else {
        responses.put(l.url, l);
      }

      response.sendRedirect(url);
      return;
    }

    if (uri.endsWith(".js") || uri.endsWith(".css") ||
        uri.endsWith(".gif") || uri.endsWith(".png") ||
        uri.endsWith(".jpg")|| uri.endsWith(".jpeg")) {
      chain.doFilter(req, res);
      return;
    }

    {
      // Log request
      Link l = new Link();
      l.url = uri;
      l.count = 1;
      l.lastVisited = Calendar.getInstance().getTimeInMillis();
      if (requests.get(l.url)!=null){
        l = (Link)requests.get(l.url);
        l.count++;
      }
      else {
        requests.put(l.url, l);
      }
    }

    // log referer
    String referer = request.getHeader("referer");
    if (referer != null && !referer.equals("")) {
      Link l = new Link();
      l.url = referer;
      l.count = 1;
      l.lastVisited = Calendar.getInstance().getTimeInMillis();
      if (referers.get(l.url)!=null){
        l = (Link)referers.get(l.url);
        l.count++;
      }
      else {
        referers.put(l.url, l);
      }
    }

    //log the hit
    count++;
    chain.doFilter(req, res);
  }

  public void init(FilterConfig filterConfig) {
    this.fc = filterConfig;
  }

  public void destroy() {
    this.fc = null;
  }

  // total visitors
  public int getCount() {
    return count;
  }

  public static Link[] getRequests() {
    Collection c = requests.values();
    Vector v = new Vector(c);
    Collections.sort(v, new LinkComparator());
    return (Link[])v.toArray(new Link[0]);
  }

  public static Link[] getResponses() {
    Collection c = responses.values();
    Vector v = new Vector(c);
    Collections.sort(v, new LinkComparator());
    return (Link[])v.toArray(new Link[0]);
  }

  public static Link[] getReferers() {
    Collection c = referers.values();
    Vector v = new Vector(c);
    Collections.sort(v, new LinkComparator());
    return (Link[])v.toArray(new Link[0]);
  }

  public long getDays() {
    Calendar now = Calendar.getInstance();
    long a = startDate.getTimeInMillis();
    long b = now.getTimeInMillis();
    long between = b-a;

    long days = (between/(1000*60*60*24));
    if (days < 1)
      days = 1;
    return days;
  }
}
