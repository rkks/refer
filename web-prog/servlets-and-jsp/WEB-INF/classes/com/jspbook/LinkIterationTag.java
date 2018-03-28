package com.jspbook;

import javax.servlet.jsp.tagext.TagSupport;
import javax.servlet.jsp.JspException;
import javax.servlet.ServletRequest;

public class LinkIterationTag extends TagSupport {
  String[] links;
  int count;

  public int doStartTag() throws JspException {
    // get the array of links
    ServletRequest request = pageContext.getRequest();
    links = (String[])request.getAttribute("links");
    // reset count
    count = 0;

    // set current link
    pageContext.setAttribute("link", links[count]);
    count++;
    return EVAL_BODY_INCLUDE;
  }

  public int doAfterBody() throws JspException {
    if(count < links.length) {
      try {
        pageContext.setAttribute("link", links[count]);
        count++;
      }
      catch (Exception e) {
        throw new JspException();
      }
      return EVAL_BODY_AGAIN;
    }
    else {
      return SKIP_BODY;
    }
  }
}
