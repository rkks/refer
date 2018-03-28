package com.jspbook;

import javax.servlet.jsp.tagext.SimpleTagSupport;
import javax.servlet.jsp.*;
import java.io.IOException;

public class HelloSimpleTag extends SimpleTagSupport {

  public void doTag() throws JspException, IOException {
    JspWriter out = jspContext.getOut();
    out.println("Hello World!");
  }
}
