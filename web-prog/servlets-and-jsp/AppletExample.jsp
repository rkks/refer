<html>
  <head>
    <title>Applet Example</title>
  </head>
  <body>
  <jsp:plugin
    type="applet"
    code="FooApplet.class"
    height="100"
    width="100"
    jreversion="1.2">
      <jsp:fallback>
      Applet support not found, can't run example.</jsp:fallback>
  </jsp:plugin>
  </body>
</html>
