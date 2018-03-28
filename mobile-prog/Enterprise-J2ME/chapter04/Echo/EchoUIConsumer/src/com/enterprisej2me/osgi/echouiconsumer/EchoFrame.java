package com.enterprisej2me.osgi.echouiconsumer;

import java.awt.*;
import java.awt.event.*;
import org.osgi.framework.*;
import org.osgi.util.tracker.*;
import com.enterprisej2me.osgi.echoservice.*;

public class EchoFrame extends Frame
    implements WindowListener, ActionListener {

  private TextField entryText;
  private Label echoedText;
  private Button submit;
  private Button exit;
  private Panel content, top, bottom, middle;
  private ServiceTracker echoTracker;
  private Bundle echoUIConsumerBundle;

  public EchoFrame (int width, int height,
                    ServiceTracker t, Bundle b) {
    super ("Echo UI");
    setBounds(0, 0, width, height);
    echoTracker = t;
    echoUIConsumerBundle = b;

    entryText = new TextField (20);
    echoedText = new Label ("  ");
    submit = new Button ("Echo");
    exit = new Button ("Exit");
    submit.addActionListener (this);
    exit.addActionListener (this);

    top = new Panel ();
    middle = new Panel ();
    bottom = new Panel ();
    top.setLayout(new FlowLayout(FlowLayout.LEFT));
    top.add(new Label("Echo: "));
    top.add(echoedText);
    middle.setLayout(new FlowLayout(FlowLayout.LEFT));
    middle.add(new Label("Input: "));
    middle.add(entryText);
    bottom.setLayout(new FlowLayout(FlowLayout.CENTER));
    bottom.add(submit);
    bottom.add(exit);

    content = new Panel ();
    content.setLayout(new GridLayout(3, 1));
    content.add(top);
    content.add(middle);
    content.add(bottom);

    add (content);
    addWindowListener(this);
    pack ();
    setVisible (true);
  }

  public void actionPerformed (ActionEvent e) {
    if ( e.getSource() == submit ) {
      top.remove (echoedText);
      EchoService echoObj = (EchoService) echoTracker.getService();
      echoedText = new Label ( echoObj.echo(entryText.getText()) );
      top.add (echoedText);
      entryText.setText("");
      setVisible (true);
    } else if ( e.getSource() == exit ) {
        // echoUIConsumerBundle.stop();
        dispose ();
    }
  }

  public void windowClosing(WindowEvent e) {}
  public void windowOpened(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowActivated(WindowEvent e) {}
  public void windowDeactivated(WindowEvent e) {}

}
