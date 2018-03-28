/*
 *
 * The contents of this file are subject to the GNU Public License
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific terms governing rights and limitations
 * under the License.
 *
 * Developed by Michael Juntao Yuan 2002.
 *
 * */
package com.enterprisej2me.GoogleAPI;

import java.util.*;

/**
  * Simple construct to hold search results. You can expand it
  * to hold more information.
  */
public class SearchResults {

  public Vector urls, titles, sizes;
  public int estimatedTotalResultsCount;
  public String searchTime;

  public SearchResults () {
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
