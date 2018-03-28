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
import com.pointbase.me.tools.toolsLoad;
import com.pointbase.unisync.api.*;

public class ResetServer {
    static String PARAMETER[]=new String[]{
        "driver",       "com.pointbase.jdbc.jdbcUniversalDriver",
        "newurl",       "jdbc:pointbase:embedded:hub,create=force",
        "url",          "jdbc:pointbase:embedded:hub",
        "user",         "PBPUBLIC",
        "password",     "PBPUBLIC",
        "script",       "data_to_load/schema.sql",
        "catdriver",    null,
        "caturl",       null,
        "catuser",      null,
        "catpassword",  null,
        "catscript",    "data_to_load/schema.sql"
    };
    static void printHelp(Properties prop) {
        System.out.println("Usage:");
        System.out.println("java ResetServer [options]");
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
        String driver=prop.getProperty("driver");
        String unisync_driver=prop.getProperty("unisync_driver",driver);
        String url=prop.getProperty("url");
        String unisync_url=prop.getProperty("unisync_url",url);
        String newurl=prop.getProperty("newurl");
        String user=prop.getProperty("user");
        String password=prop.getProperty("password");
        String catdriver=prop.getProperty("catdriver");
        String caturl=prop.getProperty("caturl");
        String catuser=prop.getProperty("catuser");
        String catpassword=prop.getProperty("catpassword");
        String script=prop.getProperty("script");
        String catscript=prop.getProperty("catscript");
        Class.forName(driver);
        if(catdriver!=null) {
            Class.forName(catdriver);
        }
        if(unisync_driver!=null) {
            Class.forName(unisync_driver);
        }

        Connection conn;

        // create a new database if required
        conn = DriverManager.getConnection(newurl,user,password);
        conn.close();

        // delete old synchronization tables
        reset(url,user,password,script);

        // create the table and insert the demo record
        conn = DriverManager.getConnection(url,user,password);
        SampleDataCreator.insert(conn);
        conn.close();

        boolean catalogIsDifferent=false;
        if(caturl!=null) {
            catalogIsDifferent=true;
            reset(caturl,catuser,catpassword,catscript);
        } else {
            catdriver=driver;
            caturl=url;
            catuser=user;
            catpassword=password;
        }


        SyncManager manager;
        log("Getting the SyncManager instance");
        manager=SyncManager.getInstance(caturl,catdriver,catuser,catpassword);
        String dsname;
        if(catalogIsDifferent) {
            // the actual data is in another database
            dsname="DATA";
            log(" Creating a new DataSource");
            // create a new datasource connecting to this database
            manager.createSyncDataSource(dsname,unisync_url,unisync_driver,user,password);
        } else {
            // the actual data is in the same database as the catalog
            dsname=SyncDataSource.DEFAULT;
        }

        String hubname="Hub";
        log("Creating the hub "+hubname);
        Hub hub=manager.createHub(hubname);

        Publication pub;
        String pubname;
        SpokeConfig spoke;
        Subscription sub;
        String subname="SubNameCard";
        String tablename="NAMECARD";
        String[] tables=new String[]{tablename};

        // publish the complete namecard table
        pubname="PubNameCard";
        log("Creating a new publication "+pubname);
        pub=hub.newPublication(pubname,dsname,tables);
        log(" Publishing");
        hub.publish(pub);

        // create two spokes and subscribe to this publication
        for(int i=1;i<=2;i++) {
            String name="Spoke"+i;
            log("Creating the spoke "+name);
            spoke=hub.createSpokeConfig(name);
            spoke.savePassword("pass"+i);
            log(" Creating the subscription "+subname);
            sub=spoke.newSubscription(subname,SyncDataSource.DEFAULT,pubname);
            log(" Subscribing");
            spoke.subscribe(sub);
        }

        if(!catalogIsDifferent) {
            // publish the namecard table; without the picture column
            // but only in the simple case

            pubname="PubNameCardNoPicture";
            log("Creating a new publication "+pubname);
            pub=hub.newPublication(pubname,dsname,tables);
            log(" Getting the table "+tablename);
            SyncTable table=pub.getSyncTable(tablename);
            log(" Dropping a column");
            table.dropSyncColumns(new String[]{"PICTURE"});
            log(" Publishing");
            hub.publish(pub);

            // create two spokes and subscribe to this publication
            for(int i=3;i<=4;i++) {
                String name="Spoke"+i;
                log("Creating the spoke "+name);
                spoke=hub.createSpokeConfig(name);
                spoke.savePassword("pass"+i);
                log(" Creating the subscription "+subname);
                sub=spoke.newSubscription(subname,SyncDataSource.DEFAULT,pubname);
                log(" Subscribing");
                spoke.subscribe(sub);
            }
        }

        manager.close();
    }

    static void log(String s) {
        System.out.println(s);
    }

    static void reset(String url,String user,String password,String script) {
        try {
            Connection conn;
            conn = DriverManager.getConnection(url,user,password);
            Statement stat = conn.createStatement();
            Vector synctables=new Vector();
            // drop the NAMECARD table if it exists
            DatabaseMetaData meta=conn.getMetaData();
            ResultSet rs=meta.getTables(null,null,"NAMECARD",null);
            if(rs.next()) {
                String schema=rs.getString("TABLE_SCHEM");
                String table=rs.getString("TABLE_NAME");
                if(schema!=null && !schema.equals("")) {
                    table=schema+"."+table;
                }
                synctables.addElement(table);
            }
            rs.close();
            for(int i=0;i<synctables.size();i++) {
                stat.executeUpdate("DROP TABLE "+synctables.elementAt(i));
            }
            // run the script to create the NAMECARD table
            toolsLoad.load(conn,script);
            conn.close();
        } catch(Exception e) {
            System.out.println("Error");
            e.printStackTrace();
        }
    }
}
