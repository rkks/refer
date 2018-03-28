<%@ page import="com.jspbook.*"%>
<%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
<% Link[] links = LinkTrackerFilter.getRequests();
   Link[] responses = LinkTrackerFilter.getRequests(); 
   Link[] referers = LinkTrackerFilter.getReferers();
%>
<p class="h1">Requests</p>
<c:forEach var="r" begin="0" items="${requests}">
  <p class="nopad"><a href="${r.url}">${r.url}</a> ${r.count} ${r.lastVisited}</p>
</c:forEach>
<p class="h1">Responses</p>
<c:forEach var="r" begin="0" items="${responses}">
  <p class="nopad"><a href="${r.url}">${r.url}</a> ${r.count} ${r.lastVisited}</p>
</c:forEach>
<p class="h1">Referers</p>
<c:forEach var="r" begin="0" items="${referers}">
  <p class="nopad"><a href="${r.url}">${r.url}</a> ${r.count} ${r.lastVisited}</p>
</c:forEach>
