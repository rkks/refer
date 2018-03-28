<%@ attribute name="link" required="true" %>
<%@ attribute name="image" required="true" %>
<a href="${link}" onmouseover="${image}.src='${image}_on.gif';"
onmouseout="${image}.src='${image}_off.gif';">
<img name="${image}" src="${image}_off.gif" border="0"></a>
