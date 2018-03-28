<html>
<head>
<title>Iteration Example</title>
</head>
<body>
<%
 String[] strings = new String[4];
 strings[0] = "Alpha";
 strings[1] = "in";
 strings[2] = "between";
 strings[3] = "Omega";
 for (int i=0; i<strings.length;i++) { %>
 String[<%= i %>] = <%= strings[i] %><br>
<% } %>
</body>
</html>
