<%@ page import="java.util.logging.*"%>
<%
Logger logger = Logger.getLogger("example");
logger.addHandler(new ConsoleHandler());

String info = request.getParameter("info");
if (info != null && !info.equals("")) {
  logger.info(info);
}
%>
<html>
  <head>
    <title>A Simple Logger</title>
  </head>
  <body>
  <form>
  Information to log:<input name="info"><br>
  <input type="submit">
  </form>
  </body>
</html>
