<%! int pageCount = 0;
 void addCount() {
   pageCount++;
 }
%>
<html>
<head>
<title>PageCounter.jsp</title>
</head>
<body>
<% addCount(); %>
This page has been visited <%= pageCount %> times.
</body>
</html>
