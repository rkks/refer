<%@ page import="java.util.logging.*, java.io.*"%>
<%
  // get logger
  Logger logger = com.jspbook.SiteLogger.getLogger();

  // get request parameters
  String level = request.getParameter("level");
  if (level != null && !level.equals("")) {
    logger.setLevel(Level.parse(level));
  }
  
  // set current level
  request.setAttribute("l", logger.getLevel());

  // set current log
  StringWriter sw = new StringWriter();
  request.setAttribute("log", sw);

  // parse in current log
  InputStream is  =
    application.getResourceAsStream("/WEB-INF/log.txt");
  if (is != null) {
    for (int i = is.read();i!=-1;i=is.read()) {
      sw.write((char)i);
    }
  } else {
    sw.write("Can't load log file!");
  }
%>
<html>
<head>
<title>Site Logging Configuration</title>
</head>
<body>
<h3>Set the site's logging level:</h3>
<b>Current level:</b> ${l}<br><br>
<form>
  <select name="level">
    <option value="SEVERE">Severe</option>
    <option value="WARNING">Warning</option>
    <option value="INFO">Info</option>
    <option value="CONFIG">Config</option>
  </select><br>
  <input type="submit" value="Update Level"><br>
</form>

<h3>Current Log:</h3>
<pre>${log}</pre>
</body>
</html>
