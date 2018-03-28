package com.jspbook.foo;

public class ExceptionThrower {

  public static void throwException(String message) throws Exception{
    throw new Exception(message);
  }

  public static void throwCustomException(String message)
      throws CustomException{
    throw new CustomException(message);
  }
}
