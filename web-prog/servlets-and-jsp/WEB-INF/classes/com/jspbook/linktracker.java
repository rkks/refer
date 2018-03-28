package com.jspbook;

import javax.servlet.http.*;
import javax.servlet.*;
import java.io.*;

public class linktracker implements Control {
  public boolean doLogic(HttpServletRequest request,
                      HttpServletResponse response)
  throws ServletException, IOException {

    Link[] requests = LinkTrackerFilter.getRequests();
    request.setAttribute("requests", requests);
    Link[] responses = LinkTrackerFilter.getResponses();
    request.setAttribute("responses", responses);
    Link[] referers = LinkTrackerFilter.getReferers();
    request.setAttribute("referers", referers);

    return true;
  }
}
