package com.jspbook;

public class UserException extends Exception {

  // A constructor to set the description of the exception
  public UserException(String description) {
    super(description);
  }
}
