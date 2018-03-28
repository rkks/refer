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
import java.awt.event.*;
import java.util.*;

public class SearchDialog extends Dialog
{
    // Fields
    private CheckboxGroup group;
    private Checkbox keywordSearch;
    private Checkbox sqlSearch;

    private TextField textName;
    private TextField textCompany;
    private TextField textTitle;
    private TextField textAddress;
    private TextField textPhone;
    private TextField textEmail;

    private TextArea sql;

    private Button searchButton;
    private Button cancelButton;

    private Vector nameCards;
    private boolean ok;

    private DBManager manager;

    // Constructors
    public SearchDialog(Frame frame) {
        super(frame, "PointBase Demo", true);

        manager = DBManager.getInstance();

        initUI();
        initEventHandler();
    }

    // Methods
    private void initUI() {
        int w = 240;
        int h = 295;
        setSize(w, h);
        setResizable(false);

        // show this frame at the center of screen
        DBManager.getInstance().center(this);

        Panel panel = new Panel();
        panel.setLayout(null);
        panel.setBackground(new Color(220, 220, 220));

        setLayout(new BorderLayout());
        add(panel, BorderLayout.CENTER);

        // Option button
        group = new CheckboxGroup();
        keywordSearch = new Checkbox("Keyword Search", group, true);
        sqlSearch = new Checkbox("SQL Search", group, false);

        Font checkboxFont = new Font("Arial", Font.ITALIC | Font.BOLD, 12);
        Color checkboxColor = new Color(100, 100, 200);

        keywordSearch.setFont(checkboxFont);
        keywordSearch.setForeground(checkboxColor);
        keywordSearch.setBounds(5, 2, 150, 20);
        panel.add(keywordSearch);

        sqlSearch.setFont(checkboxFont);
        sqlSearch.setForeground(checkboxColor);
        sqlSearch.setBounds(5, 155, 150, 20);
        panel.add(sqlSearch);

        // Keyword Search
        textName    = new TextField();
        textCompany = new TextField();
        textTitle   = new TextField();
        textAddress = new TextField();
        textPhone   = new TextField();
        textEmail   = new TextField();

        String[] names = {"Name", "Company", "Title", "Address", "Phone", "Email"};
        TextField[] textFields = {textName, textCompany, textTitle,
                                  textAddress, textPhone, textEmail};

        Color labelColor = new Color(100, 100, 200);
        Font  labelFont  = new Font("Arial", Font.ITALIC, 11);

        for (int i = 0; i < names.length; i++) {
            Label label = new Label(names[i]);
            label.setFont(labelFont);
            label.setForeground(labelColor);
            label.setBounds(5, 27 + i * 20, 60, 20);
            panel.add(label);

            TextField textField = textFields[i];
            textField.setBounds(65, 27 + i * 20, 165, 20);
            panel.add(textField);
        }

        // SQL Search
        sql = new TextArea("", 0, 0, TextArea.SCROLLBARS_VERTICAL_ONLY);
        sql.setBounds(5, 178, 225, 50);
        sql.setEnabled(false);
        panel.add(sql);

        searchButton = new Button("Search");
        searchButton.setBounds(108, 236, 60, 25);
        panel.add(searchButton);

        cancelButton = new Button("Cancel");
        cancelButton.setBounds(170, 236, 60, 25);
        panel.add(cancelButton);
    }

    private void initEventHandler() {
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                ok = false;
                setVisible(false);
            }
        });

        keywordSearch.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                textName.setEnabled(true);
                textCompany.setEnabled(true);
                textTitle.setEnabled(true);
                textAddress.setEnabled(true);
                textPhone.setEnabled(true);
                textEmail.setEnabled(true);

                sql.setEnabled(false);
            }
        });

        sqlSearch.addItemListener(new ItemListener() {
            public void itemStateChanged(ItemEvent e) {
                textName.setEnabled(false);
                textCompany.setEnabled(false);
                textTitle.setEnabled(false);
                textAddress.setEnabled(false);
                textPhone.setEnabled(false);
                textEmail.setEnabled(false);

                sql.setEnabled(true);
            }
        });

        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (keywordSearch.getState() == true) {
                    String name    = textName.getText();
                    String company = textCompany.getText();
                    String title   = textTitle.getText();
                    String address = textAddress.getText();
                    String phone   = textPhone.getText();
                    String email   = textEmail.getText();

                    nameCards = manager.findNameCardsByKeyword(name, company,
                        title, address, address, phone, email);
                }
                else {
                    nameCards = manager.findNameCardsBySQL(sql.getText());
                }

                ok = true;
                setVisible(false);
            }
        });

        cancelButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ok = false;
                setVisible(false);
            }
        });
    }

    public boolean open() {
        ok = false;
        setVisible(true);

        return ok;
    }

    public Vector getNameCards() {
        return nameCards;
    }
}

