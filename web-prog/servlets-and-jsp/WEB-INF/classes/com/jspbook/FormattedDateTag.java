package com.jspbook;

import javax.servlet.jsp.tagext.*;
import javax.servlet.jsp.*;
import java.util.*;
import java.text.*;
import java.io.*;

public class FormattedDateTag extends TagSupport {
  private String format;

  public void setFormat(String format) {
    this.format = format;
  }

  public int doStartTag() throws JspException {
    JspWriter out = pageContext.getOut();
    try {
      if (format != null) {
        SimpleDateFormat sdf = new SimpleDateFormat(format);
        out.println(sdf.format(new Date()));
      }
      else {
        out.println(new Date().toString());
      }
    }
    catch (IOException e) {
      throw new JspException(e.getMessage());
    }
    finally {
      format = null;
    }

    return SKIP_BODY;
  }
}
