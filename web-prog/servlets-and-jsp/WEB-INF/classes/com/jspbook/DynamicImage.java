package com.jspbook;

import javax.servlet.*;
import javax.servlet.http.*;
import java.io.*;
import java.awt.*;
import java.awt.image.*;
import com.sun.image.codec.jpeg.*;

public class DynamicImage extends HttpServlet {

  public void doGet(
    HttpServletRequest request,
    HttpServletResponse response)
    throws ServletException, IOException {

    response.setContentType("image/jpeg");

    // Create Image
    int width = 200;
    int height = 30;
    BufferedImage image = new BufferedImage(
      width, height, BufferedImage.TYPE_INT_RGB);

     // Get drawing context
     Graphics2D g = (Graphics2D)image.getGraphics();

     // Fill background
     g.setColor(Color.gray);
     g.fillRect(0, 0, width, height);

     // Draw a string
     g.setColor(Color.white);
     g.setFont(new Font("Dialog", Font.PLAIN, 14));
     g.drawString("http://www.jspbook.com",10,height/2+4);

     // Draw a border
     g.setColor(Color.black);
     g.drawRect(0,0,width-1,height-1);

     // Dispose context
     g.dispose();

     // Send back image
     ServletOutputStream sos = response.getOutputStream();
     JPEGImageEncoder encoder = JPEGCodec.createJPEGEncoder(sos);
     encoder.encode(image);
   }
}
