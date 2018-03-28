<%@ taglib uri="http://java.sun.com/jstl/core_rt" prefix="c" %>

<html>
<head>
  <title>&lt;c:import&gt; Action Example</title>
</head>
<body>
Here is the source-code of the main page:
<pre>
<c:import url="welcome.html" var="value"/>
<c:out value="${value}"/>
</pre>
</body>
</html>
