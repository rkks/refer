<%
  Cookie[] cookies = request.getCookies();
  String timestamp = new java.util.Date().toString();
  Cookie ts = new Cookie("timestamp",timestamp);
  response.addCookie(ts);
  Cookie lastvisit = null;
  for (int i=0;i<cookies.length;i++) {
    Cookie temp = cookies[i];
    if (temp.getName().equals("timestamp")) {
      lastvisit  = temp;
      break;
    }
  }
  if (lastvisit == null) {
    lastvisit = ts;
  }
  request.setAttribute("lastvisit", lastvisit);
%>
<html>
<head>
<title>Cookie Visitor Tracker</title>
</head>
<body>
You last visited this website: ${lastvisit.value}
</body>
</html>
