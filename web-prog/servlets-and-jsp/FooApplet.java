import javax.swing.*;
import java.awt.*;

public class FooApplet extends JApplet {
  public void init() {
    JLabel label = new JLabel("I'm an Applet.");
    label.setHorizontalAlignment(JLabel.CENTER);
    getContentPane().add(label, BorderLayout.CENTER);
  }
}
