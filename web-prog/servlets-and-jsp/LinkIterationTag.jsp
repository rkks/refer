<%
  // An array of links
  String[] links = new String[5];
  links[0] = "http://www.jspbook.com";
  links[1] = "http://java.sun.com";
  links[2] = "http://www.jspinsider.com";
  links[3] = "http://www.developmentor.com";
  links[4] = "http://www.aw.com";
  request.setAttribute("links", links);
%>
<%@ taglib prefix="x" uri="http://www.jspbook.com/example" %>
<html>
  <head>
    <title>Eliminating Redundency</title>
  </head>
  <body>
    Some helpful JSP and Servlet links
    <ul>
    <x:iterateLinks>
      <li><a href="${link}">${link}</a></li>
    </x:iterateLinks>
    </ul>
  </body>
</html>
