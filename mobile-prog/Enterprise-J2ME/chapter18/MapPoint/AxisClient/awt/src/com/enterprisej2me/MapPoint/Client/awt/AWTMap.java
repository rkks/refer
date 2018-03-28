package com.enterprisej2me.MapPoint.Client.awt;

import java.util.*;
import java.awt.*;
import java.awt.event.*;

import org.ksoap.*;
import org.ksoap.transport.*;
import org.ksoap.marshal.*;

public class AWTMap extends Frame
    implements WindowListener, ActionListener {
  
  private String endPointURL;
  
  private ScrollPane scroll;
  private Panel content;
  private Button go;
  private Button startOver;
  private Button showMap;
  private Button showDirections;
  private TextField fromStreet;
  private TextField fromCity;
  private TextField fromState;
  private TextField fromZip;
  private TextField toStreet;
  private TextField toCity;
  private TextField toState;
  private TextField toZip;
  private java.awt.List directionsList;

  private ClassMap cm;
  private Marshal md;
  
  public AWTMap (int width, int height) {
    super("MapPoint Java demo");
    try {

      cm = new ClassMap ();
      md = new MarshalBase64 ();
      md.register (cm);

      // Windows Box: 192.168.2.101
      // Mac     Box: 192.168.2.100
      endPointURL = "http://128.83.129.226:8080/axis/services/MapPoint";
      
      go = new Button("Go");
      go.addActionListener(this);
      startOver = new Button("Start Over");
      startOver.addActionListener(this);
      showMap = new Button("Show Map");
      showMap.addActionListener(this);
      showDirections = new Button("Show Directions");
      showDirections.addActionListener(this);
      
      
      fromStreet = new TextField("", 16);
      fromCity = new TextField("", 18);
      fromState = new TextField("", 4);
      fromZip = new TextField("", 6);
      toStreet = new TextField("", 16);
      toCity = new TextField("", 18);
      toState = new TextField("", 4);
      toZip = new TextField("", 6);
      
      scroll = new ScrollPane ();
      add(scroll);
      addWindowListener(this);
      pack();
      Insets is = getInsets();
      width += is.left + is.right;
      height += is.top + is.bottom;
      Dimension ss = getToolkit().getScreenSize();
      width = Math.min(width, ss.width);
      height = Math.min(height, ss.height);
      setBounds((ss.width - width) / 2, 
                (ss.height - height) / 2, width, height);
      
      startScreen();
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  // Listens to button events
  public void actionPerformed (ActionEvent e) {
    if ( e.getSource() == startOver ) {
      startScreen();
    } else if ( e.getSource() == go ) {
      listScreen(true);
    } else if ( e.getSource() == showDirections ) {
      listScreen(false);
    } else if ( e.getSource() == showMap ) {
      mapScreen(directionsList.getSelectedIndex());
    } else {
      System.out.println("No such event?");
    }
  }
  
  public static void main(String[] args) {
    new AWTMap(250, 250);
  }
  
  public void startScreen () {
    try {
      /*
      Panel from = new Panel ();
      from.add(new Label("From Address");
      Panel to = new Panel ();
      to.add(new Label("To Address");
      */
      
      Panel fStreet = new Panel (new FlowLayout(FlowLayout.LEFT));
      fStreet.add(new Label("Street"));
      fStreet.add(fromStreet);
      Panel fCity = new Panel (new FlowLayout(FlowLayout.LEFT));
      fCity.add(new Label("City"));
      fCity.add(fromCity);
      Panel fState = new Panel (new FlowLayout(FlowLayout.LEFT));
      fState.add(new Label("State"));
      fState.add(fromState);
      fState.add(new Label("Zip"));
      fState.add(fromZip);
      
      Panel tStreet = new Panel (new FlowLayout(FlowLayout.LEFT));
      tStreet.add(new Label("Street"));
      tStreet.add(toStreet);
      Panel tCity = new Panel (new FlowLayout(FlowLayout.LEFT));
      tCity.add(new Label("City"));
      tCity.add(toCity);
      Panel tState = new Panel (new FlowLayout(FlowLayout.LEFT));
      tState.add(new Label("State"));
      tState.add(toState);
      tState.add(new Label("Zip"));
      tState.add(toZip);
      
      Panel bottom = new Panel ();
      bottom.add(go);
      
      if (content != null) {
        scroll.remove(content);
      }
      content = new Panel ();
      content.setLayout(new GridLayout(11, 1));
      
      content.add(new Label("From Address"));
      content.add(fStreet);
      content.add(fCity);
      content.add(fState);
      content.add(new Label("To Address"));
      content.add(tStreet);
      content.add(tCity);
      content.add(tState);
      content.add(bottom);
      
      scroll.add(content);
      setVisible(true);
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  
  private void listScreen (boolean newSearch) {
    try {
      if (newSearch) {
        SoapObject method =
            new SoapObject("", "getDirections");
        // Use the SE version for standard JDK
        // Http methods
        HttpTransportSE rpc = 
          new HttpTransportSE(endPointURL, "\"\"");
        rpc.setClassMap(cm);
        method.addProperty("in0",
            fromStreet.getText());
        method.addProperty("in1",
            fromCity.getText());
        method.addProperty("in2",
            fromState.getText());
        method.addProperty("in3",
            fromZip.getText());
        method.addProperty("in4",
            toStreet.getText());
        method.addProperty("in5",
            toCity.getText());
        method.addProperty("in6",
            toState.getText());
        method.addProperty("in7",
            toZip.getText());
        Vector v = (Vector) rpc.call (method);

        directionsList = 
            new java.awt.List(10, false);
        directionsList.add("Overview Map");
        for (int i = 0; i < v.size(); i++) {
          directionsList.add(
              (String) v.elementAt(i));
        }
        directionsList.setSize(200, 200);
      }

      Panel top = new Panel ();
      top.setLayout(new FlowLayout(FlowLayout.LEFT));
      top.add(directionsList);
      Panel bottom = new Panel ();
      bottom.setLayout(new FlowLayout(FlowLayout.LEFT));
      bottom.add(startOver);
      bottom.add(showMap);
      
      scroll.remove(content);
      content = new Panel ();
      content.setLayout(new BorderLayout());
      content.add(top, BorderLayout.CENTER);
      // content.add(mid, BorderLayout.CENTER);
      content.add(bottom, BorderLayout.SOUTH);
      scroll.add(content);
      setVisible(true);
      
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  
  private void mapScreen (int i) {
    try {
      ImageItem img;
      byte [] imgarray;
      SoapObject method =
          new SoapObject("", "getMap");
      HttpTransportSE rpc = 
        new HttpTransportSE(endPointURL, "\"\"");
      rpc.setClassMap(cm);
      method.addProperty("in0", new Integer(i));
      method.addProperty("in1", new Integer(200));
      method.addProperty("in2", new Integer(200));
      imgarray = (byte []) rpc.call (method);
      img = new ImageItem(imgarray, 200, 200);
      
      Panel top = new Panel ();
      top.add(img);
      Panel bottom = new Panel ();
      bottom.add(startOver);
      bottom.add(showDirections);
      
      scroll.remove(content);
      content = new Panel ();
      content.setLayout(new BorderLayout());
      content.add(top, BorderLayout.CENTER);
      content.add(bottom, BorderLayout.SOUTH);
      scroll.add(content);
      setVisible(true);
      
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  
  public void closeDown () {
    System.exit(0);
    setVisible(true);
  }

  public void windowClosing(WindowEvent e) {
    closeDown ();
  }

  public void windowOpened(WindowEvent e) {}
  public void windowClosed(WindowEvent e) {}
  public void windowIconified(WindowEvent e) {}
  public void windowDeiconified(WindowEvent e) {}
  public void windowActivated(WindowEvent e) {}
  public void windowDeactivated(WindowEvent e) {}

}
