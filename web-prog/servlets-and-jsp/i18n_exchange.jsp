<%@ page import="java.util.*, java.text.*"%>
<%
 MessageFormat usdf =
   new MessageFormat("{0,number,currency}", Locale.US);
 MessageFormat gbpf =
   new MessageFormat("{0,number,currency}", Locale.UK);
 MessageFormat cadf = 
   new MessageFormat("{0,number,currency}", Locale.CANADA);

 double usd = 1;
 double gbp = .65505;
 double cad = 1.5716;
 String usdusd = usdf.format(new Object[]{new Double(usd)});
 String usdgbp = usdf.format(new Object[]{new Double(gbp)});
 String usdcad = usdf.format(new Object[]{new Double(cad)});
 String gbpusd = gbpf.format(new Object[]{new Double(usd/gbp)});
 String gbpgbp = gbpf.format(new Object[]{new Double(1)});
 String gbpcad = gbpf.format(new Object[]{new Double(cad/gbp)});
 String cadusd = cadf.format(new Object[]{new Double(usd/cad)});
 String cadgbp = cadf.format(new Object[]{new Double(gbp/cad)});
 String cadcad = cadf.format(new Object[]{new Double(1)});

%>
<html>
<head>
  <title>I18N Currency Exchange</title>
</head>
<body>
  <h1>Currency Exchange Rates</h1>
  <b>Values current as of:
    <%= DateFormat.getInstance().format(new Date()) %><b>
 <table border="1">
  <tr>
   <td></td><td>USD</td><td>GBP</td><td>CAD</td>
  </tr>
  <tr>
   <td>U.S. Dollar</td><td><%= usdusd %></td>
   <td><%= usdgbp %></td><td><%= usdcad %></td>
  </tr>
  <tr>
   <td>G.B. Pound</td><td><%= gbpusd %></td>
   <td><%= gbpgbp %></td><td><%= gbpcad %></td>
  </tr>
  <tr>
   <td>CA. Dollar</td><td><%= cadusd %></td>
   <td><%= cadgbp %></td><td><%= cadcad %></td>
  </tr>
 </table>
</body>
</html>
