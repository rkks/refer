package com.jspbook;

import com.jspbook.*;
import javax.servlet.jsp.tagext.*;
import javax.servlet.jsp.*;
import java.util.*;
import java.io.*;

public class DateTag extends TagSupport {

  public int doStartTag() throws JspException {
    JspWriter out = pageContext.getOut();
    try {
      Date date = new Date();
      out.println(date.toString());
    }
    catch (IOException e) {
      throw new JspException(e.getMessage());
    }

    return SKIP_BODY;
  }
}
