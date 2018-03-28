package com.jspbook;

import java.util.*;

public class LinkComparator implements java.util.Comparator {
  public int compare(Object o1, Object o2) {
    Link l1 = (Link)o1;
    Link l2 = (Link)o2;
    return l2.getCount() - l1.getCount();
  }

  public boolean equals(Object o1, Object o2) {
    Link l1 = (Link)o1;
    Link l2 = (Link)o2;
    if (l2.getCount() == l1.getCount()) {
      return true;
    }
    return false;
  }
}
