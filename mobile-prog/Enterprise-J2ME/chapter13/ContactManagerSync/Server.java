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

import java.sql.*;
import java.util.*;
import java.io.*;
import com.pointbase.unisync.api.*;

public class Server {

    static String PARAMETER[]=new String[]{
        "catdriver",       "com.pointbase.jdbc.jdbcUniversalDriver",
        "caturl",          "jdbc:pointbase:embedded:hub",
        "catuser",         "PBPUBLIC",
        "catpassword",     "PBPUBLIC",
        "hub",     "Hub",
    };
    static void printHelp(Properties prop) {
        System.out.println("Usage:");
        System.out.println("java Server [options]");
        System.out.println("options are:");
        System.out.println("-options <file>  read the options from a properties file");
        for(int i=0;i<PARAMETER.length;i+=2) {
            System.out.print("-"+PARAMETER[i]+" <value>   ");
            System.out.println("(default: "+PARAMETER[i+1]+")");
        }
    }
    public static void main(String arg[]) {
        Properties prop = new Properties();
        for (int i = 0; i < PARAMETER.length; i+=2) {
            String key=PARAMETER[i];
            String value=PARAMETER[i+1];
            if(value!=null) {
                prop.put(key,value);
            }
        }
        for (int i = 0; i < arg.length; i++) {
            String p = arg[i];
            if (p.equals("-?")) {
                printHelp(prop);
                return;
            } else if (p.charAt(0) == '-') {
                prop.put(p.substring(1), arg[++i]);
            }
        }
        String options=prop.getProperty("options",null);
        if(options!=null) {
            try {
                FileInputStream fin=new FileInputStream(options);
                prop.load(fin);
                fin.close();
            } catch(IOException e) {
                e.printStackTrace();
            }
        }
        try {
            configure(prop);
        } catch(Exception e) {
            System.out.println("Error");
            e.printStackTrace();
        }
    }
    static void configure(Properties prop) throws Exception {
        String driver=prop.getProperty("catdriver");
        String url=prop.getProperty("caturl");
        String user=prop.getProperty("catuser");
        String password=prop.getProperty("catpassword");
        String hubName=prop.getProperty("hub");

        SyncManager manager=SyncManager.getInstance(url,driver,user,password);
        Hub hub=manager.getHub(hubName);
        hub.startServer();
        doCommandLoop();
        hub.stopServer();

    }


    static void doCommandLoop() throws Exception {
        print("");
        print("UniSync Server is running");
        print("X or Q stops the server and quits; H for help");
        BufferedReader br=new BufferedReader(
            new InputStreamReader(System.in)
        );
        while(true) {
            String s = br.readLine();
            if(s==null) {
                // on devices without input, we don't loop at all
                break;
            }
            if(s.length() == 0) {
                continue;
            }
            char c=s.toUpperCase().charAt(0);
            switch(c) {
            case 'H':
            case '?':
                print("");
                print("Key     Meaning");
                print("------  -------");
                print("x or q  stops the server and quits");
                print("h or ?  prints this message");
                print("");
                break;
            case 'X':
            case 'Q':
                print("Stopping...");
                return;
            default:
                print("Unrecognized character, press h for help");
                break;
            }
        }
    }
    static void print(String s) {
        System.out.println(s);
    }
}
