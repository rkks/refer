<html>
<head>
<title>Examples of URL encoding</title>
</head>
<body>
A local URL: index.jsp<br>
<%= response.encodeURL("index.jsp") %><br>
<%= response.encodeRedirectURL("index.jsp") %><br>
<br>
A remote URL: http://www.jspbook.com<br>
<%= response.encodeURL("http://www.jspbook.com") %><br>
<%= response.encodeRedirectURL("http://www.jspbook.com") %><br>
<br>
</body>
</html>
