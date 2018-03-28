<%@ page
  isErrorPage="true"
  import="
    java.util.*,
    javax.mail.*,
    javax.mail.internet.*" %>
<%
  Properties props = new Properties();
  props.put("mail.smtp.server","your smtp server");
  Session msession = Session.getInstance(props,null);
  // e-mail address from chapter 2
  String email = application.getInitParameter("admin email");

  MimeMessage message= new MimeMessage(msession);
  message.setSubject("[Application Error]");
  message.setFrom(new InternetAddress(email));
  message.addRecipient(Message.RecipientType.TO, new InternetAddress(email));
  String debug = "";

  Integer status_code
   =(Integer)request.getAttribute("javax.servlet.error.status_code");
  if (status_code != null) {
    debug += "status_code: "+status_code.toString() + "\n";
  }
  Class exception_type=
   (Class)request.getAttribute("javax.servlet.error.exception_type");
  if (exception_type != null) {
    debug += "exception_type: "+exception_type.getName() + "\n";
  }
  String m=
    (String)request.getAttribute("javax.servlet.error.message");
  if (m != null) {
    debug += "message: "+m + "\n";
  }
  Throwable e = 
   (Throwable) request.getAttribute("javax.servlet.error.exception");
  if (e != null) {
    debug += "exception: "+ e.toString() + "\n";
  }
  String request_uri =
    (String)request.getAttribute("javax.servlet.error.request_uri");
  if (request_uri != null) {
    debug += "request_uri: "+request_uri + "\n";
  }
  String servlet_name=
    (String)request.getAttribute("javax.servlet.error.servlet_name");
  if (servlet_name != null) {
    debug += "servlet_name: "+servlet_name;
  }

  //set message, send e-mail
  message.setText(debug);
  Transport.send(message);
%>
<html>
 <head>
  <title>EmailErrorPage</title>
 </head>
 <body>
  <h3>An Error Has Occurred</h3>
  Sorry, but this site is unavailable to render the service you
  requested. A bug in the system has caused an error to occur.
  Please send a description of the problem to
  <a href="mailto:<%=email%>"><%=email%></a>.
  </body>
</html>
