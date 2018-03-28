<%@ page import="com.jspbook.foo.*" %>
<html>
 <head>
   <title>ExceptionThrower</title>
 </head>
 <body>
 <%
 try {
   double rand = Math.random();
   if (rand <.5) {
       ExceptionThrower.throwException("foo");
   }
   ExceptionThrower.throwCustomException("foo");
 }
 catch (CustomException e) {
   out.println("Caught a CustomException: " + e.getMessage());
 }
 catch (Exception e) {
   out.println("Caught a Exception: " + e.getMessage());
 }
 %>
 </body>
</html>
