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

public class MainFrame extends Frame
{
    // Fields
    private NameCardList nameCardList;
    private EditDialog   editDialog;
    private SearchDialog searchDialog;

    private Button addButton;
    private Button editButton;
    private Button deleteButton;
    private Button searchButton;
    private Button allButton;
    private Button exitButton;
    private Button syncButton;


    // Constructor
    public MainFrame() {
        super("PointBase Micro");

        editDialog   = new EditDialog(this);
        searchDialog = new SearchDialog(this);

        initUI();
        initEventHandler();
    }

    // Methods
    private void initUI() {
        int width  = 240;
        int height = 295;
        setSize(width, height);
        setResizable(false);

        DBManager.getInstance().center(this);

        Panel panel = new Panel();
        panel.setLayout(null);
        panel.setBackground(new Color(220, 220, 220));

        setLayout(new BorderLayout());
        add(panel, BorderLayout.CENTER);

        Label label = new Label("Name List");
        label.setBounds(5, 10, 140, 17);
        label.setFont(new Font("Arial", Font.ITALIC, 12));
        label.setForeground(new Color(100, 100, 220));
        panel.add(label);

        nameCardList = new NameCardList();
        nameCardList.setBounds(5, 30, 160, 229);
        panel.add(nameCardList);

        addButton = new Button("Add");
        addButton.setBounds(170, 30, 58, 25);
        panel.add(addButton);

        editButton = new Button("Edit");
        editButton.setBounds(170, 57, 58, 25);
        panel.add(editButton);

        deleteButton = new Button("Delete");
        deleteButton.setBounds(170, 84, 58, 25);
        panel.add(deleteButton);

        searchButton = new Button("Search");
        searchButton.setBounds(170, 130, 58, 25);
        panel.add(searchButton);

        allButton = new Button("All");
        allButton.setBounds(170, 157, 58, 25);
        panel.add(allButton);


        syncButton = new Button("Sync");
        syncButton.setBounds(170, 208, 58, 25);
        panel.add(syncButton);


        exitButton = new Button("Exit");
        exitButton.setBounds(170, 235, 58, 25);
        panel.add(exitButton);
    }

    private void initEventHandler() {
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                DBManager.getInstance().disconnect();
                System.exit(0);
            }
        });

        addButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (editDialog.open(null) == true) {
                    NameCard nameCard = editDialog.getNameCard();
                    nameCardList.addNameCard(nameCard);
                }
            }
        });

        editButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (nameCardList.getSelectedIndex() == -1) return;
                if (editDialog.open(nameCardList.getSelectedNameCard())) {
                    NameCard nameCard = editDialog.getNameCard();
                    nameCardList.updateNameCard(nameCard);
                }
            }
        });

        deleteButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (nameCardList.getSelectedIndex() == -1) return;
                NameCard nameCard = nameCardList.getSelectedNameCard();
                nameCardList.deleteNameCard(nameCard);
            }
        });

        searchButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                if (searchDialog.open()) {
                    nameCardList.setNameCards(searchDialog.getNameCards());
                }
            }
        });

        allButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                Vector nameCards = DBManager.getInstance().getAllNameCards();
                nameCardList.setNameCards(nameCards);
            }
        });

        exitButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                DBManager.getInstance().disconnect();
                System.exit(0);
            }
        });

        nameCardList.addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                if (e.getClickCount() >= 2) {
                    if (nameCardList.getSelectedIndex() == -1) return;
                    if (editDialog.open(nameCardList.getSelectedNameCard())) {
                        NameCard nameCard = editDialog.getNameCard();
                        nameCardList.updateNameCard(nameCard);
                    }
                }
            }
        });



        syncButton.addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                DBManager.getInstance().sync();
                Vector nameCards = DBManager.getInstance().getAllNameCards();
                nameCardList.setNameCards(nameCards);
            }
        });


    }
}
