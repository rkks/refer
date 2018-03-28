/*-----------------------------------------------------------------------

  P O I N T B A S E,	 I N C .

  -----------------------------------------------------------------------

 (C) Copyright 2002, 2003 PointBase, Inc.	All rights reserved.

 NOTICE:  All information contained herein or attendant hereto is,
          and remains, the property of PointBase, Inc.  Many of the
          intellectual and technical concepts contained herein are
          proprietary to PointBase, Incorporated and may be covered
          by U.S. and Foreign Patents or Patents Pending, or are
          protected as trade secrets.  Any dissemination of this
          information or reproduction of this material is strictly
          forbidden unless prior written permission is obtained
          from PointBase, Inc.

 -----------------------------------------------------------------------
*/

import java.io.*;
import java.util.*;
import java.lang.reflect.*;

public class ContactManager
{
    private static Properties properties = new Properties();
    public static void main(String[] args) {
        // parse command line parameters
        for(int i=0;i<args.length;i++) {
            String p=args[i];
            if(p.equals("-?")) {
                printHelp();
            }
            if(p.charAt(0)=='-') {
                    properties.put(p.substring(1),args[i+1]);
                    i++;
            }
        }
        new MainFrame().show();
    }
    static void printHelp() {
        System.out.println(
            "Usage: java ContactManager [options] \n"+
            "options are: \n"+
            "-driver <jdbcDriver>       default: com.pointbase.me.jdbc.jdbcDriver \n"+
            "-url <jdbcUrl>             default: jdbc:pointbase:pbdemo \n"+
            "-user <name>               default: public \n"+
            "-password <password>       default: public \n"+
            "-delayread                 default: true \n");
        System.exit(0);
    }
    static Properties getProperties() {
        return properties;
    }
}
