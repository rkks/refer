<%@ page isErrorPage="true" %>
<html>
  <head>
    <title>An Error Has Occurred!</title>
  </head>
  <body>
  <h3>An Error Has Occurred</h3>
  Sorry, but this site is unavailable to render the service you
  requested. A bug in the system has caused an error to occur.
  Please send a description of the problem to
  <%= application.getInitParameter("admin email") %>.
  </body>
</html>
