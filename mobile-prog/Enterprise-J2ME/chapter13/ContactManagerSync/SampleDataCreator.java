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
import java.sql.*;

public class SampleDataCreator {
    public static void insert(Connection conn) {
        try {
            insertDemoRecord(conn,0,
                "Eddie Pratt","PointBase","Product Manager",
                "3910 Freedom Circle","Santa Clara, CA 95054",
                "408 961 1100","eddie.pratt@pointbase.com","images/eddie.jpg");
            insertDemoRecord(conn,1,
                "Bruce Scott","PointBase","CEO",
                "3910 Freedom Circle","Santa Clara, CA 95054",
                "408 961 1100","ask.bruce@pointbase.com","images/bruce.jpg");
        } catch(SQLException e) {
            e.printStackTrace();
        }
    }
    static void insertDemoRecord(Connection conn,int id,String name,
    String company,String title,String address1,String address2,String phone,
    String email,String picture) throws SQLException {
        PreparedStatement prep=conn.prepareStatement(
            "INSERT INTO NameCard (ID,Name,Company,Title, "+
            "Address1,Address2,Phone,Email,Picture) VALUES("+
            "?,?,?,?,?,?,?,?,?)");
        try {
            prep.setInt(1,id);
            prep.setString(2,name);
            prep.setString(3,company);
            prep.setString(4,title);
            prep.setString(5,address1);
            prep.setString(6,address2);
            prep.setString(7,phone);
            prep.setString(8,email);
            File f=new File(picture);
            int len=(int)f.length();
            byte b[]=new byte[len];
            FileInputStream fin=new FileInputStream(f);
            int offset=0;
            while(offset<len) {
                offset+=fin.read(b,offset,len-offset);
            }
            fin.close();
            prep.setBytes(9,b);
        } catch(IOException e) {
            e.printStackTrace();
        }
        prep.execute();
    }
}
