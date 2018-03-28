<jsp:include page="header.jsp"/>
<h3>Add News</h3>
<p>Fill in all fields to add your news to Foo news.</p>
<form method="post">
 Tile: <input size="50" name="title" value="${form.title}"><br>
 Link: <input size="50" name="link" value="${form.link}"><br>
 Story:
   <textarea cols="50" rows="10" name="story">
     ${form.story}
   </textarea>
 <br>
 <input type="submit" value="Add News">
</form>
<jsp:include page="footer.jsp"/>
