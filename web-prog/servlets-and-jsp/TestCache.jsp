<%@ page import="java.util.*, java.net.*, java.io.*" %>
<%
  String url = request.getParameter("url");
  long[] times = new long[2];
  if (url != null) {
    for (int i=0;i<2;i++) {
      long start = Calendar.getInstance().getTimeInMillis();
      URL u = new URL(url);
      HttpURLConnection huc =
        (HttpURLConnection)u.openConnection();
      huc.setRequestProperty("user-agent","Mozilla(MSIE)");
      huc.connect();
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      InputStream is = huc.getInputStream();
      while(is.read() != -1) {
        baos.write((byte)is.read());
      }
      long stop = Calendar.getInstance().getTimeInMillis();
      times[i] = stop-start;
    }
  }
  request.setAttribute("t1", new Long(times[0]));
  request.setAttribute("t2", new Long(times[1]));
  request.setAttribute("url", url);

%><html>
<head>
  <title>Cache Test</title>
</head>
<body>
<h1>Cache Test Page</h1>
Enter a URL to test.
<form method="POST">
<input name="url" size="50">
<input type="submit" value="Check URL">
</form>
 <p><b>Testing: ${url}</b></p>
 Request 1: ${t1} milliseconds<br/>
 Request 2: ${t2} milliseconds<br/>
 Time saved: ${t1-t2} milliseconds<br/>
</body>
</html>
