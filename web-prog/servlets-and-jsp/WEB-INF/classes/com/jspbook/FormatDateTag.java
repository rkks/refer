package com.jspbook;

import javax.servlet.jsp.tagext.*;
import javax.servlet.jsp.*;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.io.IOException;

public class FormatDateTag extends SimpleTagSupport {
  private String format;

  public void setFormat(String format) {
    this.format = format;
  }

  public void doTag() throws JspException, IOException {
    JspWriter out = jspContext.getOut();
    if (format != null) {
      SimpleDateFormat sdf = new SimpleDateFormat(format);
      out.println(sdf.format(new Date()));
    }
    else {
      out.println(new Date().toString());
    }
  }
}
