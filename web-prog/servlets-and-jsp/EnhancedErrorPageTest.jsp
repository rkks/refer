<%@ page errorPage="EnhancedErrorPage" %>
<%
   if (true)
     throw new Exception("An Exception!");
%>
