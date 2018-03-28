<%! int pageCount = 0;
 void addCount() {
   pageCount++;
 }
%>
<% addCount(); %>
<html>
<head>
<title>Header/Footer Example</title>
</head>
<body>
<center>
<h2>Servlets and JSP the J2EE Web Tier</h2>
<a href="http://www.jspbook.com">Book Support Site</a> - 
<a href="ShowSource">Sites Source-Code</a><br>
This site has been visited <%= pageCount %> times.
</center>
<br><br>
