import java.io.*;
import java.text.*;
import java.util.*;
import javax.servlet.*;
import javax.servlet.http.*;
/**
 * Example servlet showing request headers
 */

public class RequestServlet2 extends HttpServlet {

    
    public void doPost(HttpServletRequest request,
                      HttpServletResponse response)
        throws IOException, ServletException
    {
        response.setContentType("text/plain");
        PrintWriter out = response.getWriter();
        BufferedReader br = request.getReader();
        String buf = br.readLine();
        out.print("Rec: "+buf);
    }
}
