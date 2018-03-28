package com.jspbook;

import javax.servlet.jsp.tagext.*;
import javax.servlet.jsp.*;
import java.util.*;
import java.io.IOException;

public class DynamicAttributeTag
    extends SimpleTagSupport implements DynamicAttributes {
  protected Hashtable map = new Hashtable();

  public void setDynamicAttribute(String uri, String name,
      Object value) throws JspException{
    map.put(name, value);
  }

  public void doTag() throws JspException, IOException {
    JspWriter out = jspContext.getOut();
    for (Enumeration keys = map.keys(); keys.hasMoreElements();) {
      Object key = keys.nextElement();
      Object value = map.get(key);
      out.print("<b>Attribute:</b><br>");
      out.print("name: " + key.toString() + "<br>");
      out.print("value: " + value.toString() + "<br>");
    }
  }
}
