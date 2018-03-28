<jsp:include page="header.jsp"/>
<%@ page import="
java.io.*,
javax.xml.parsers.*,
org.w3c.dom.*,
javax.xml.transform.*,
javax.xml.transform.stream.*,
javax.xml.transform.dom.*" %>
<%
  String title = request.getParameter("title");
  String link = request.getParameter("link");
  String story = request.getParameter("story");
  if (title != null && !title.equals("") &&
      story != null && !story.trim().equals("") &&
      link != null && !link.equals("")) {
    ServletContext sc = pageContext.getServletContext();
    String dir = sc.getRealPath("/model1");
    File file = new File(dir+"/news.xml");
    DocumentBuilderFactory dbf =
      DocumentBuilderFactory.newInstance();
    DocumentBuilder db = dbf.newDocumentBuilder();
    Document doc = null;
    if (file.exists()){
      doc = db.parse(file);
    }
    else {
      doc = db.newDocument();
      Element root = doc.createElement("news");
      doc.appendChild(root);
    }
    Element news = doc.createElement("story");
    news.setAttribute("title",title);
    news.setAttribute("link",link);
    news.setAttribute("story",story);
    doc.getDocumentElement().appendChild(news);

    TransformerFactory tf = TransformerFactory.newInstance();
    Transformer t = tf.newTransformer();
    DOMSource source = new DOMSource(doc);
    StreamResult result = 
      new StreamResult(new FileOutputStream(file));
    t.transform(source, result);
%>

<p align="center">You news has been added!<br>
<a href="index.jsp">Back to main page.</a></p>

<% }
  else {
    if(title == null) {
      title = "";
    }
    if(link == null) {
      link = "";
    }
    if(story == null) {
      story = "";
    }
%>
<h3>Add News</h3>
<p>Fill in all fields to add your news to Foo news.</p>
<form method="post">
 Tile: <input size="50" name="title" value="<%= title%>"><br>
 Link: <input size="50" name="link" value="<%= link%>"><br>
 Story:
   <textarea cols="50" rows="10" name="story">
     <%= story%>
   </textarea>
 <br>
 <input type="submit" value="Add News">
</form>
<% } %>
<jsp:include page="footer.jsp"/>
