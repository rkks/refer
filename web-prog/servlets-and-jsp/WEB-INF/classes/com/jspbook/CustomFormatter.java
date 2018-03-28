package com.jspbook;

import java.io.*;
import java.util.*;
import java.util.logging.*;

public class CustomFormatter extends Formatter {

  public String format(LogRecord log) {
    Date date = new Date(log.getMillis());
    String level = log.getLevel().getName();
    String string = "[" + level + " " +date.toString() + "]\n";
    string += log.getMessage() + "\n\n";
    Throwable thrown = log.getThrown();
    if (thrown != null) {
      string += thrown.toString();
    }
    return string;
  }
}
