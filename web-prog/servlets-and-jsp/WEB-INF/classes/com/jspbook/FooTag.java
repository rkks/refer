package com.jspbook;

import com.jspbook.*;
import javax.servlet.jsp.tagext.*;
import javax.servlet.jsp.*;
import java.io.*;

public class FooTag extends TagSupport {

  public int doStartTag() throws JspException {
    JspWriter out = pageContext.getOut();
    try {
      out.println("foo");
    }
    catch (IOException e) {
      throw new JspException(e.getMessage());
    }

    return SKIP_BODY;
  }
}
