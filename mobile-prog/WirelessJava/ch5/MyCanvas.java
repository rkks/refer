import javax.microedition.lcdui.*;

public class MyCanvas extends Canvas {
   public void paint(Graphics g) {
      g.setColor(255, 0, 0);
      g.fillRect(0, 0, getWidth(), getHeight());
      g.setColor(255, 255, 255);
      g.drawString("Hello World!", 0, 0, g.TOP | g.LEFT);
   }
}
