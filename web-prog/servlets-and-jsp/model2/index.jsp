<%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
<jsp:include page="header.jsp"/>
<c:forEach var="story" begin="0" items="${news}">
  <hr>
  <h3>
    <a href="${story.link}">${story.title}</a>
  </h3>
  ${story.story}
</c:forEach>
<jsp:include page="footer.jsp"/>
