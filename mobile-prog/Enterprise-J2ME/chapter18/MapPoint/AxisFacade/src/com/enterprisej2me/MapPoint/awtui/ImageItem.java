package com.enterprisej2me.MapPoint.awtui;
 
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
 
public class ImageItem extends Component implements ImageObserver {

  private Image img;
  private int width, height;
  
  public ImageItem (String fname, int w, int h) throws Exception {
    super();
    Class c = this.getClass();
    InputStream is = c.getResourceAsStream(fname);
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    byte [] b = new byte[1];
    while ( is.read(b) != -1 ) {
       baos.write(b);
    }
    img = getToolkit().createImage(baos.toByteArray());
    baos.close();
    is.close();

    width = w;
    height = h;
    setVisible(true);
  }
  
  public ImageItem (byte [] binImg, int w, int h) throws Exception {
    super();
    img = getToolkit().createImage(binImg);
    width = w;
    height = h;
    setVisible(true);
  }

  public void paint(Graphics g) {
    super.paint(g);
    g.drawImage(img, 0, 0, this);
  }

  public Dimension getPreferredSize() {
    return new Dimension(width, height);
  }

  public Dimension getMinimumSize() {
    return new Dimension(width, height);
  }

  public boolean imageUpdate(Image img, int infoflags, int x, int y,
                           int width, int height) {
    if (infoflags == ALLBITS) {
      repaint();
      return false;
    } else {
      return true;
    }
  }
}
