<%@ variable name-given="pageCount"
             variable-class="java.lang.Integer"
             scope="AT_BEGIN"%>
<%
  Integer pageCount = 
    (Integer)application.getAttribute("pageCount");
  if (pageCount == null) {
    application.setAttribute("pageCount", new Integer(1));
  }
  else {
    pageCount = new Integer(1+pageCount.intValue());
    application.setAttribute("pageCount", pageCount);
  }
%>
