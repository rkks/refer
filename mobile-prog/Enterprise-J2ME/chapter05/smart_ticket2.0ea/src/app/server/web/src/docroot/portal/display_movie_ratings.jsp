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
 $Id: display_movie_ratings.jsp,v 1.6 2003/03/24 22:43:12 zull Exp $
--%>

<%@ page import="java.text.*" %>
<%@ page import="java.util.*" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocal" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocalHome" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal" %>
<%@ page import="com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeException" %>

<%
    final String SESSSION_ATTRIBUTE_FACADE = "com.sun.j2me.blueprints.smartticket.server.ejb.SmartTicketFacadeLocal";

    SmartTicketFacadeLocal facade = (SmartTicketFacadeLocal) session.getAttribute(SESSSION_ATTRIBUTE_FACADE);
    if (facade == null) {
%>
        <jsp:forward page="/portal/login.jsp" /> 
<%
    } else {
%>
    <jsp:include page="/common/header.jsp" />
    <h1>Java Smart Ticket Demo - Web Application</h1>
    <h2>Movie Ratings Update Page</h2>
    Your current movie ratings are displayed below. You may change the rating of individual movies
<%
        String action = request.getParameter("action");
        if (action == null || action.equals("display")) {
            MovieRatingLocal[] movieRatings = facade.getMovieRatings();
%>
        <br>
	    <br>
	    <table border="0"  bgcolor="#336666">
	    <tr>
		<td><font color="white" size="3">Title</font></td>
		<td><font color="white" size="3">Last Viewing Date</font></td>
		<td><font color="white" size="3">Number of Stars</font></td>
		<td><font color="white" size="3">Delete</font></td>
		<td></td>
	    </tr>

<%
            for (int i = 0; i < movieRatings.length; i++) {
		        MovieRatingLocal movieRating = movieRatings[i];
%>
        
    <form action="/smartticket/portal/display_movie_ratings.jsp" method="get">
        <input type="hidden" name="action" value="handle">
        <input type="hidden" name="id" value="<%= movieRating.getId() %>">
	    <tr bgcolor="#eeebcc">
		<td align="center"><%= movieRating.getRatedMovie().getTitle() %></td>
		<td align="right"><%= DateFormat.getInstance().format(new Date(movieRating.getLastViewingDate())) %></td>
		<td><input type="text"
			   name="stars"
			   value="<%= movieRating.getStarNumber() %>"
			   size="1"></td>
	    <td align="center"><input type="checkbox" name="deleted" value="true"></td>
   		<td align="center"><input type="image" border="0" src="images/button_submit.gif" name="submit"></td>
		</tr>
    </form>
<%
	    }
%>
        </table>
	    <br><br>
<%
        } else if (action.equals("handle")) {
          MovieRatingLocal movieRating = facade.getMovieRating(request.getParameter("id"));
            System.err.println("deleted=" + request.getParameter("deleted"));
          if (Boolean.valueOf(request.getParameter("deleted")).booleanValue()) {
            System.err.println("remove");
             movieRating.remove();
          } else { 
             movieRating.changeStarNumber(Integer.parseInt(request.getParameter("stars")));
          }
          response.sendRedirect("/smartticket/portal/display_movie_ratings.jsp?action=display");
        } else if (action.equals("error")) {
%>
         <h2>Error</h2>
         <hr>
         <p><a href="/smartticket/portal/display_movie_ratings.jsp?action=display">Return to Movie Ratings Update Page</a></p>
<%    }
    }
%>
    <p>
    <p>
    <p>
    <a href="/smartticket/portal/menu.jsp">Go Back to the Main Menu</a>
<jsp:include page="/common/footer.jsp" />


