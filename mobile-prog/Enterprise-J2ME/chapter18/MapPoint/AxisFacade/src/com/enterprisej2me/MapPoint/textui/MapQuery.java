package com.enterprisej2me.MapPoint.textui;

import com.enterprisej2me.MapPoint.facade.*;

public class MapQuery {
  
  public static void main(String[] args) {
    try {
      MPClient c = new MPClient ();
      String [] directions =
        c.getDirections("1 Microsoft Way", "Redmond", "WA", "",
                   "2345 Congress Ave", "Austin", "TX", "");
      int seglen = c.getSegmentNum();
      
      System.out.println("Overview map size is " +
                         c.getMap(0, 200, 200).length);
      for (int i = 1; i < seglen+1; i++) {
        System.out.print(directions[i] + " ");
        System.out.println("map size is " + c.getMap(i, 200, 200).length);
      }
    } catch (Exception e) {
      e.printStackTrace();
    }
  }
}
