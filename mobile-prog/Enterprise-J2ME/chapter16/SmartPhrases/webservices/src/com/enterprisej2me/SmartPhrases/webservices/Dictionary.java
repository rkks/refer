package com.enterprisej2me.SmartPhrases.webservices;

import java.util.*;

public class Dictionary {

  DICTClient dic;

  public Dictionary () throws Exception {
    // Use the dict.org server and WorldNet v1.7
    dic = new DICTClient("www.dict.org", "wn");
  }

  public String getWordMeaning( String wordtoSearch ) {

    try {
      return dic.getDefinitions(wordtoSearch);
    } catch (Exception e) {
      return "";
    }
  }

  public String [] getSpellSugg( String wordtoSearch ) {

    try {
      // Match words within Levenshtein distance one
      // Sound based match
      Vector v = dic.getMatches("lev", wordtoSearch);
      String [] result = new String [v.size()];
      for (int i = 0; i < v.size(); i++ ) {
        result[i] = (String) v.elementAt(i);
      }
      return result;
    } catch (Exception e) {
      e.printStackTrace();
      return new String[0];
    }
  }

}
