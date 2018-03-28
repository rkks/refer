<jsp:include page="header.jsp"/>
<%@ page import="
java.io.*,
javax.xml.parsers.*,
org.w3c.dom.*" %>
<%
  ServletContext sc = pageContext.getServletContext();
  String dir = sc.getRealPath("/model1");
  File file = new File(dir+"/news.xml");
  DocumentBuilderFactory dbf =
    DocumentBuilderFactory.newInstance();
  DocumentBuilder db = dbf.newDocumentBuilder();
  Document doc= null;
  if (file.exists()) {
    doc = db.parse(file);
  }
  if (doc != null) {
    NodeList nodes = doc.getElementsByTagName("story");
    for (int i = 0; i < nodes.getLength();i++) {
      Element e = (Element)nodes.item(i);
%>
    <hr>
    <h3><a href="<%= e.getAttribute("link") %>">
    <%= e.getAttribute("title") %></a></h3>
    <%= e.getAttribute("story") %>
<%
    }
  }
%>
<jsp:include page="footer.jsp"/>
