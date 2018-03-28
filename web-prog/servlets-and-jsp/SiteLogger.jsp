<%@ page import="java.util.logging.*"%>
<%
// get logger
Logger logger = com.jspbook.SiteLogger.getLogger();

// get required request parameters
String info = request.getParameter("info");
String level = request.getParameter("level");

// log information appropriately
if (info != null && !info.equals("") &&
    level != null && !level.equals("")) {
  logger.log(Level.parse(level), info);
}
%>
<html>
  <head>
    <title>A Simple Logger</title>
  </head>
  <body>
  <form>
  <table>
    <tr>
      <td>Level:</td>
      <td>
        <select name="level">
          <option value="SEVERE">Severe</option>
          <option value="WARNING">Warning</option>
          <option value="INFO">Information</option>
          <option value="CONFIG">Configuration</option>
        </select>
      </td>
    </tr>
    <tr>
      <td>Information to log:</td>
      <td><input name="info"><br></td>
    </tr>
  </table>
  <input type="submit">
  </form>
  <a href="SiteLogger">View/Configure Log</a>
  </body>
</html>
