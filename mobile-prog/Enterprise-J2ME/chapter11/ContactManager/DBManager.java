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
import java.awt.*;

class DBManager {
    private static DBManager instance; // for Singleton Pattern
    private String driver;
    private String url;
    private String user;
    private String password;
    private boolean delay;
    private Connection connection;
    private Statement statement;
    private PreparedStatement insert;
    private PreparedStatement find;
    private PreparedStatement delete;
    private PreparedStatement update;
    private PreparedStatement all;

    static DBManager getInstance() {
        if (instance == null) {
            instance = new DBManager();
        }
        return instance;
    }

    private DBManager() {
        // get parameters
        Properties properties = ContactManager.getProperties();
        driver =
            properties.getProperty("driver","com.pointbase.me.jdbc.jdbcDriver");
        url =
            properties.getProperty("url", "jdbc:pointbase:micro:pbdemo");
        user =
            properties.getProperty("user", "PBPUBLIC");
        password =
            properties.getProperty("password", "PBPUBLIC");
        delay =
            properties.getProperty("delayread","true").equals("true");
        connect();
    }

    private void connect() {
        try {
            System.out.println("Connecting to the database...");

            Class.forName(driver);

            // If the database doesn't exist, create a new database.
            connection = DriverManager.getConnection(url, user, password);
            statement = connection.createStatement();

            System.out.println("Initializing the database...");
            boolean newdb=initDatabase();

            System.out.println("Creating prepared statements...");
            createStatement();

            if(newdb) {
                System.out.println("Making a sample entry...");
                SampleDataCreator.insert(connection);
            }

        } catch (Exception e) {
            System.out.println("Error connecting to database");
            e.printStackTrace();
            System.exit(1);
        }
    }

    void disconnect() {
        try {
            System.out.println("Closing the database...");
            connection.commit();
            statement.close();
            insert.close();
            find.close();
            delete.close();
            update.close();
            all.close();
            connection.close();
            System.exit(0);
        } catch (Exception e) {
            System.out.println("Problem in closing database");
            e.printStackTrace();
            System.exit(1);
        }
    }

    private boolean initDatabase() {
        try {
            String sql = "CREATE TABLE NameCard (ID INT PRIMARY KEY, "+
                "Name VARCHAR(254), Company VARCHAR(254), Title VARCHAR(254), "+
                "Address1 VARCHAR(254), Address2 VARCHAR(254), "+
                "Phone VARCHAR(254), Email VARCHAR(254), "+
                "Picture Binary(1000000))";
            // if the table already exists, this will throw an exception
            statement.executeUpdate(sql);
            // this means the database already exists
            return true;
        } catch (SQLException e) {
            // ignore the error - the table already exists, which is good
            // so we don't need to add demo data later on
            // in productive systems, it would be better to see in the metadata
            // if the table already exists
            return false;
        }
    }

    private void createStatement() {
        try {
            insert = connection.prepareStatement(
                "INSERT INTO NameCard (ID, Name, Company, Title, Address1, "+
                "Address2, Phone, Email, Picture) "+
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
            find = connection.prepareStatement(
                "SELECT * FROM NameCard WHERE (Name LIKE ?) "+
                "AND (Company LIKE ?) AND (Title LIKE ?) "+
                "AND ((Address1 LIKE ?) OR (Address2 LIKE ?)) "+
                "AND (Phone LIKE ?) AND (Email LIKE ?)");
            delete = connection.prepareStatement(
                "DELETE FROM NameCard WHERE ID = ?");
            update = connection.prepareStatement(
                "UPDATE NameCard SET ID=?, Name=?, Company=?, Title=?, "+
                "Address1=?, Address2=?, Phone=?, Email=?, Picture=? "+
                "WHERE ID = ?");
            all = connection.prepareStatement(
                "SELECT ID, Name, Company, Title, Address1, Address2, "+
                "Phone, Email FROM NameCard");
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    Vector findNameCardsByKeyword(String name, String company,
            String title, String address1, String address2,
            String phone, String email) {
        Vector NameCards = new Vector();
        String[] keywords = {name, company, title, address1, address2,
        phone, email};
        try {
            for (int i = 0; i < keywords.length; i++) {
                String criteria = (keywords[i].equals("")) ? "%" :
                        "%" + keywords[i] + "%";
                find.setString(i + 1, criteria);
            }
            ResultSet resultSet = find.executeQuery();
            while (resultSet.next()) {
                NameCard nameCard = new NameCard(resultSet.getInt(1),
                        resultSet.getString(2), resultSet.getString(3),
                        resultSet.getString(4), resultSet.getString(5),
                        resultSet.getString(6),
                        resultSet.getString(7), resultSet.getString(8));
                if (!delay)
                    loadPicture(nameCard);
                NameCards.addElement(nameCard);
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return NameCards;
    }

    Vector findNameCardsBySQL(String sql) {
        Vector NameCards = new Vector();
        try {
            ResultSet resultSet =
                    statement.executeQuery("SELECT ID, Name, Company, Title, "+
                    "Address1, Address2, Phone, Email FROM NameCard WHERE " +
                    sql);
            while (resultSet.next()) {
                NameCard nameCard = new NameCard(resultSet.getInt(1),
                        resultSet.getString(2), resultSet.getString(3),
                        resultSet.getString(4), resultSet.getString(5),
                        resultSet.getString(6),
                        resultSet.getString(7), resultSet.getString(8));
                if (!delay)
                    loadPicture(nameCard);

                NameCards.addElement(nameCard);
            }
        }
        catch (SQLException e) {
        }
        return NameCards;
    }

    void addNameCard(NameCard nameCard) {
        nameCard.setID(getNewID());
        try {
            insert.setInt(1, nameCard.getID());
            insert.setString(2, nameCard.getName());
            insert.setString(3, nameCard.getCompany());
            insert.setString(4, nameCard.getTitle());
            insert.setString(5, nameCard.getAddress1());
            insert.setString(6, nameCard.getAddress2());
            insert.setString(7, nameCard.getPhone());
            insert.setString(8, nameCard.getEmail());
            insert.setBytes(9, nameCard.getPicture().getBytes());
            insert.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    void updateNameCard(NameCard nameCard) {
        try {
            update.setInt(1, nameCard.getID());
            update.setString(2, nameCard.getName());
            update.setString(3, nameCard.getCompany());
            update.setString(4, nameCard.getTitle());
            update.setString(5, nameCard.getAddress1());
            update.setString(6, nameCard.getAddress2());
            update.setString(7, nameCard.getPhone());
            update.setString(8, nameCard.getEmail());
            update.setBytes(9, nameCard.getPicture().getBytes());
            update.setInt(10, nameCard.getID());
            update.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    void deleteNameCard(NameCard nameCard) {
        try {
            delete.setInt(1, nameCard.getID());
            delete.executeUpdate();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    Vector getAllNameCards() {
        Vector NameCards = new Vector();
        try {
            ResultSet resultSet = all.executeQuery();
            while (resultSet.next()) {
                NameCard nameCard = new NameCard(resultSet.getInt(1),
                        resultSet.getString(2), resultSet.getString(3),
                        resultSet.getString(4), resultSet.getString(5),
                        resultSet.getString(6),
                        resultSet.getString(7), resultSet.getString(8));
                if (!delay)
                    loadPicture(nameCard);
                NameCards.addElement(nameCard);
            }
        }
        catch (SQLException e) {
            e.printStackTrace();
        }
        return NameCards;
    }

    void loadPicture(NameCard nameCard) {
        try {
            ResultSet resultSet =
                    statement.executeQuery(
                        "SELECT Picture FROM NameCard WHERE ID = " +
                        nameCard.getID());
            resultSet.next();
            Picture picture = new Picture();
            picture.setBytes(resultSet.getBytes(1));
            nameCard.setPicture(picture);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private int getNewID() {
        try {

            ResultSet resultSet = statement.executeQuery(
                "SELECT MAX(ID)+1 FROM NameCard");
            if (resultSet.next()) {
                return resultSet.getInt(1);
            } else {
                return 0;
            }

        } catch (Exception e) {
            e.printStackTrace();
        }
        return 0;
    }

    void center(Window window) {
        int w=window.getSize().width;
        int h=window.getSize().height;
        Dimension d = Toolkit.getDefaultToolkit().getScreenSize();
        window.setBounds((d.width - w) / 2, (d.height - h) / 2, w, h);
    }

}
