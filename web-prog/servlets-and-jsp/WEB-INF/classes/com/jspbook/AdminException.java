package com.jspbook;

public class AdminException extends RuntimeException {

  // A constructor to set the description of the exception
  public AdminException(String description) {
    super(description);
  }
}
