<%
  String format = "yyyy/MM/dd";
  request.setAttribute("format", format);
  String language = request.getHeader("accept-language");
  if (language != null) {
    if (language.indexOf("en-us") != -1 ||
        language.indexOf("en-US") != -1 ||
        language.indexOf("EN-US") != -1) {
        format = "MM/dd/yyyy";
    }
    else if (
        language.indexOf("en-uk") != -1 ||
        language.indexOf("en-UK") != -1 ||
        language.indexOf("EN-UK") != -1) {
        format = "dd/MM/yyyy";
    }
  }
%>
<%@ taglib prefix="ex" uri="http://www.jspbook.com/example" %>
<html>
  <head>
    <title>A test of FormatDateTag.java</title>
  </head>
  <body>
    The date is <ex:date format="${format}" />.
  </body>
</html>
