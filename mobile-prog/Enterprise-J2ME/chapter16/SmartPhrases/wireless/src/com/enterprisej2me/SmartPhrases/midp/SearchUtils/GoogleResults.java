package com.enterprisej2me.SmartPhrases.midp.SearchUtils;
 
import java.util.*;

/**
  * Simple construct to hold search results. You can expand it
  * to hold more information.
  */
public class GoogleResults {

  public Vector urls, titles, sizes;
  public int estimatedTotalResultsCount;
  public String searchTime;

  public GoogleResults () {
    urls = new Vector ();
    titles = new Vector ();
    sizes = new Vector ();
    estimatedTotalResultsCount = 0;
    searchTime = "0.0";
  }

  public int getSize () {
    return urls.size();
  }

}
