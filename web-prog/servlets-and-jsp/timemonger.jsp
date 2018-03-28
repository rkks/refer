<html>
  <head>
    <title>Cache Filter Test</title>
  </head>
  <body>
A test of the cache Filter.
<%
 // mock time-consuming code
 for (int i=0;i<100000;i++) {
   for (int j=0;j<1000;j++) {
     //noop
   }
 }
%>
  </body>
</html>
