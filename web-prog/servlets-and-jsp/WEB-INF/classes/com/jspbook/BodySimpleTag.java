package com.jspbook;

import javax.servlet.jsp.tagext.SimpleTagSupport;
import java.io.IOException;
import javax.servlet.jsp.*;

public class BodySimpleTag extends SimpleTagSupport {

  public void doTag() throws JspException, IOException {
    // null means 'send the output to JspContext.getOut()'
    jspBody.invoke(null);
  }
}
