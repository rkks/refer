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

import java.awt.*;

public class NameCard
{
    // Fields
    private int    id;
    private String  name;
    private String  company;
    private String  title;
    private String  address1;
    private String  address2;
    private String  phone;
    private String  email;
    private Picture picture;

    // Constructors
    public NameCard() {
        picture = new Picture();
    }

    public NameCard(int id, String name, String company, String title,
    String address1, String address2, String phone, String email) {
        this.id       = id;
        this.name     = name;
        this.company  = company;
        this.title    = title;
        this.address1 = address1;
        this.address2 = address2;
        this.phone    = phone;
        this.email    = email;
        this.picture  = new Picture();
    }

    public NameCard(int id, String name, String company, String title,
    String address1, String address2, String phone, String email, String file) {
        this.id       = id;
        this.name     = name;
        this.company  = company;
        this.title    = title;
        this.address1 = address1;
        this.address2 = address2;
        this.phone    = phone;
        this.email    = email;
        this.picture  = new Picture(file);
    }

    // Methods
    public int getID() {
        return id;
    }

    public void setID(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCompany() {
        return company;
    }

    public void setCompany(String company) {
        this.company = company;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getAddress1() {
        return address1;
    }

    public void setAddress1(String address1) {
        this.address1 = address1;
    }

    public String getAddress2() {
        return address2;
    }

    public void setAddress2(String address2) {
        this.address2 = address2;
    }

    public String getPhone() {
        return phone;
    }

    public void setPhone(String phone) {
        this.phone = phone;
    }

    public String getEmail() {
        return email;
    }

    public void setEmail(String email) {
        this.email = email;
    }

    public void setPicture(Picture picture) {
        this.picture = picture;
    }

    public Picture getPicture() {
        return picture;
    }

    public Image getImage() {
        if (picture.getImage() == null) {
            DBManager.getInstance().loadPicture(this);
        }

        return picture.getImage();
    }
}
