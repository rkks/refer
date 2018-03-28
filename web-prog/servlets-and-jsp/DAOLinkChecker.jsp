<%@ page import="com.jspbook.*, java.net.*, java.util.*" %>
<%
  LinkBean[] links = LinkFactory.read(null);
  for (int i=0;i<links.length;i++) {
    LinkBean link = links[i];
    String url = link.getUrl();
    LinkedList deleted = new LinkedList();
    request.setAttribute("deleted", deleted);
    boolean failed = false;
    try {
      URL u = new URL(url);
      HttpURLConnection huc =
        (HttpURLConnection)u.openConnection();
      int responseCode = huc.getResponseCode();
      if (responseCode >= 400 && responseCode < 500) {
        failed = true;
      }
    }
    catch (UnknownHostException e) {
      failed = true;
    }
    catch (UnknownServiceException e) {
      failed = true;
    }
    catch (MalformedURLException e) {
      failed = true;
    }
    if (failed) {
      LinkFactory.delete(new LinkBean[] {link});
      deleted.add(url);
    }
  }
%>
<html>
  <head>
    <title>Link Checker</title>
  </head>
  <body>
  <h2>Links Deleted</h2>
  <%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
  <c:forEach var="link" begin="0" items="${deleted}">
    ${link}<br>
  </c:forEach>
  </body>
</html>
