<%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>
<html>
<head>
    <title>Cookies Sent By Your Client</title>
</head>
<body>
<h1>Cookies sent by your client</h1>
<c:forEach var="c" begin="0" items="${cookie}">
 Name: <b>${c.value.name}</b><br>
 Value: ${c.value.value}<br>
</c:forEach>
</body>
</html>
