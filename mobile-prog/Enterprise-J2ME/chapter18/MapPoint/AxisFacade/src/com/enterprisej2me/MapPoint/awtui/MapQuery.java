package com.enterprisej2me.MapPoint.awtui;

import java.io.*;
import java.awt.*;
import java.awt.event.*;
import com.enterprisej2me.MapPoint.facade.*;

public class MapQuery extends Frame
    implements WindowListener, ActionListener {
  
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
  private List directionsList;
  
  private int width, height;
  private MPClient mpc;
  
  public MapQuery (int width, int height) {
    super("MapPoint Java demo");
    try {
      go = new Button("Go");
      go.addActionListener(this);
      startOver = new Button("Start Over");
      startOver.addActionListener(this);
      showMap = new Button("Show Map");
      showMap.addActionListener(this);
      showDirections = new Button("Show Directions");
      showDirections.addActionListener(this);

      mpc = new MPClient ();
      
      fromStreet = new TextField("", 10);
      fromCity = new TextField("", 10);
      fromState = new TextField("", 10);
      fromZip = new TextField("", 10);
      toStreet = new TextField("", 10);
      toCity = new TextField("", 10);
      toState = new TextField("", 10);
      toZip = new TextField("", 10);
      
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
      int idx = directionsList.getSelectedIndex();
      if (idx == -1) idx = 0;
      mapScreen(idx);
    } else {
      System.out.println("No such event?");
    }
  }
  
  public static void main(String[] args) {
    new MapQuery(250, 250);
  }
  
  public void startScreen () {
    try {
      /*
      Panel from = new Panel ();
      from.add(new Label("From Address");
      Panel to = new Panel ();
      to.add(new Label("To Address");
      */
      
      Panel fStreet = new Panel ();
      fStreet.add(new Label("Street"));
      fStreet.add(fromStreet);
      Panel fCity = new Panel ();
      fCity.add(new Label("City"));
      fCity.add(fromCity);
      Panel fState = new Panel ();
      fState.add(new Label("State"));
      fState.add(fromState);
      Panel fZip = new Panel ();
      fZip.add(new Label("Zip"));
      fZip.add(fromZip);
      
      Panel tStreet = new Panel ();
      tStreet.add(new Label("Street"));
      tStreet.add(toStreet);
      Panel tCity = new Panel ();
      tCity.add(new Label("City"));
      tCity.add(toCity);
      Panel tState = new Panel ();
      tState.add(new Label("State"));
      tState.add(toState);
      Panel tZip = new Panel ();
      tZip.add(new Label("Zip"));
      tZip.add(toZip);
      
      
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
      content.add(fZip);
      content.add(new Label("To Address"));
      content.add(tStreet);
      content.add(tCity);
      content.add(tState);
      content.add(tZip);
      content.add(bottom);
      
      scroll.add(content);
      setVisible(true);
      repaint();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  
  private void listScreen (boolean newSearch) {
    try {
      if (newSearch) {
        String [] directions =
                mpc.getDirections (fromStreet.getText(), fromCity.getText(),
                        fromState.getText(), fromZip.getText(),
                        toStreet.getText(), toCity.getText(),
                        toState.getText(), toZip.getText());
        directionsList = new List(10, false);
        directionsList.add("Overview Map");
        for (int i = 0; i < directions.length; i++) {
          directionsList.add(directions[i]);
        }
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
      repaint();

    } catch (Exception e) {
      e.printStackTrace();
    }
  }
  
  private void mapScreen (int i) {
    try {
      ImageItem img  = new ImageItem(mpc.getMap(i, 200, 200), 200, 200);
      
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
      repaint();

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
