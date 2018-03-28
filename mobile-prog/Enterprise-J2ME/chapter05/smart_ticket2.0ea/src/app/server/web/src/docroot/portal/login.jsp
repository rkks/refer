<%--
    Copyright 2001, 2002, 2003 Sun Microsystems, Inc. All Rights Reserved.

Except for any files in PNG format (which are marked with the filename
extension ".png"), GIF format (which are marked with the filename
extension ".gif"), or JPEG format (which are marked with the filename
extension ".jpg"), redistribution and use in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

- Redistribution of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

- Redistribution in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

You may compile, use, perform and display the following files with
original Java Smart Ticket Sample Application code obtained from Sun
Microsystems, Inc. only:

- files in PNG format and having the ".png" extension
- files in GIF format and having the ".gif" extension
- files in JPEG format and having the ".jpg" extension

You may not modify or redistribute .png, .gif, or .jpg files in any
form, in whole or in part, by any means without prior written
authorization from Sun Microsystems, Inc. and its licensors, if any.

Neither the name of Sun Microsystems, Inc., the 'Java Smart Ticket
Sample Application', 'Java', 'Java'-based names, or the names of
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

This software is provided "AS IS," without a warranty of any kind. ALL
EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN
MIDROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE FOR
ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR
ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

You acknowledge that this software is not designed, licensed or
intended for use in the design, construction, operation or maintenance
of any nuclear facility.
 $Id: login.jsp,v 1.4 2003/03/24 22:43:12 zull Exp $
--%>

<%@ page import="javax.ejb.*" %>
<%@ page import="javax.naming.*" %>
<%@ page import="java.util.*" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocal" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocalHome" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeException" %>

<jsp:include page="/common/header.jsp" />

<%
    final String EJB_REF_FACADE = "ejb/SmartTicketFacade";
    final String SESSSION_ATTRIBUTE_FACADE = "com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocal";

    SmartTicketFacadeLocal facade = (SmartTicketFacadeLocal) session.getAttribute(SESSSION_ATTRIBUTE_FACADE);
    if (facade == null) {
          Context context = (Context)new InitialContext().lookup("java:comp/env");
          facade = ((SmartTicketFacadeLocalHome)context.lookup(EJB_REF_FACADE)).create();
          session.setAttribute(SESSSION_ATTRIBUTE_FACADE, facade);
    }
    String action = request.getParameter("action");
    if (action == null || action.equals("display")) {
%>
    <h1>Java Smart Ticket Demo - Web Application</h1>
    <h2>Please sign into SmartTicket Web Application</h2>
    <br><br><br><br>
      <form action="/smartticket/portal/login.jsp" method="get">
      <input type="hidden" name="action" value="handle">
      <table>
      <tr>
       <td align="center" >
       <table border="0">
       <tr>
        <td><b>User ID:</b></td>
        <td>
          <input type="text" size="15" name="username" value=""> 
        </td>
       </tr>
       <tr>
        <td><b>Password:</b></td>
         <td> 
          <input type="password" size="15" name="password" value="">
        </td>
       </tr>
       <tr>
        <td></td>
        <td align="right"> 
         <input type="image" border="0" src="images/button_submit.gif" name="submit">
        </td>
       </tr>
       <tr>
        <td><br></td>
       </tr>
       </table>
       </td>
      </tr>
      </table>
    </form>
<%
    } else if (action.equals("handle")) {
       try {
          facade.login(request.getParameter("username"), request.getParameter("password"));
          response.sendRedirect("/smartticket/portal/menu.jsp");
       } catch (SmartTicketFacadeException stfe) {
          response.sendRedirect("/smartticket/portal/login.jsp?action=error");
       }
%>
        
<%
    } else if (action.equals("error")) {
%>
         <h2>Login Error</h2>
         <hr>
         You could not be authenticated with the information provided. <br>
         Please check your Username and Password.<br>

         <p><a href="/smartticket/portal/login.jsp?action=display">Return to Sign in Page</a></p>
<%    }
%>

<jsp:include page="/common/footer.jsp" flush="true" />
