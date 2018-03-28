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

public class EditDialog extends Dialog
{
    // Constants -----------------------------------------------------------------//
    private static final String[] names =
        {"Name", "Company", "Title",
        "Address1", "Address2", "Phone", "Email"};
    private static final String[] labels =
        {"Name", "Company / Title", "Address", "Phone", "Email"};
    private static final String[] buttonLabels =
        {"OK", "Cancel", "Set Image..."};

    // Layout
    private static final int[][] labelBounds = {{110, 3  , 100, 17},
                                                {110, 48 , 100, 17},
                                                {5,  120, 45,  17},
                                                {5,  165, 45,  17},
                                                {5,  185, 45,  17}};

    private static final int[][] textBounds = {{110, 20,  120, 20},
                                               {110, 65,  120, 20},
                                               {110, 85,  120, 20},
                                               {65,  120, 165, 20},
                                               {65,  140, 165, 20},
                                               {65,  165, 165, 20},
                                               {65,  185, 165, 20}};

    private static final int[] pictureBounds = {5, 10, 100, 100};

    private static final int[][] buttonBounds = {{108,  210, 60, 25},
                                                 {170, 210, 60, 25},
                                                 {5, 210, 85, 25}};
    //----------------------------------------------------------------------------//

    private Hashtable textFields;
    private Button[] buttons;
    private Panel picturePanel;

    private Frame owner;
    private Image image;
    private NameCard nameCard;
    private boolean ok;

    public EditDialog(Frame owner) {
        super(owner, "PointBase Demo", true);
        this.owner = owner;

        initUI();
        initEventHandler();
    }

    private void initUI() {
        int width = 240;
        int height = 270;
        setSize(width, height);
        setResizable(false);

        DBManager.getInstance().center(this);

        Panel panel = new Panel();
        panel.setLayout(null);
        panel.setBackground(new Color(220, 220, 220));

        locateLabels(panel);
        locateTextFields(panel);
        locateButtons(panel);
        locatePicture(panel);

        setLayout(new BorderLayout());
        add(panel, BorderLayout.CENTER);
    }

    private void locateLabels(Container c) {
        Color labelColor = new Color(100, 100, 200);
        Font  labelFont  = new Font("Arial", Font.ITALIC, 11);

        for (int i = 0; i < labels.length; i++) {
            Label label = new Label(labels[i]);
            label.setForeground(labelColor);
            label.setFont(labelFont);

            int x      = labelBounds[i][0];
            int y      = labelBounds[i][1];
            int width  = labelBounds[i][2];
            int height = labelBounds[i][3];
            label.setBounds(x, y, width, height);

            c.add(label);
        }
    }

    private void locateTextFields(Container c) {
        textFields = new Hashtable();
        for (int i = 0; i < names.length; i++) {
            TextField text = new TextField();
            textFields.put(names[i], text);

            int x      = textBounds[i][0];
            int y      = textBounds[i][1];
            int width  = textBounds[i][2];
            int height = textBounds[i][3];
            text.setBounds(x, y, width, height);

            c.add(text);
        }
    }

    private void locatePicture(Container c) {
        picturePanel = new Panel() {
            public void paint(Graphics g) {
                if (image != null) {
                    int x = (getSize().width - image.getWidth(this)) / 2;
                    int y = (getSize().height - image.getHeight(this)) / 2;
                    g.drawImage(image, x, y, this);
                }
            }
        };
        picturePanel.setBackground(Color.lightGray);

        int x      = pictureBounds[0];
        int y      = pictureBounds[1];
        int width  = pictureBounds[2];
        int height = pictureBounds[3];
        picturePanel.setBounds(x, y, width, height);

        c.add(picturePanel);
    }

    private void locateButtons(Container c) {
        int len=buttonBounds.length;
        buttons = new Button[len];
        for (int i = 0; i < len; i++) {
            buttons[i] = new Button(buttonLabels[i]);

            int x      = buttonBounds[i][0];
            int y      = buttonBounds[i][1];
            int width  = buttonBounds[i][2];
            int height = buttonBounds[i][3];
            buttons[i].setBounds(x, y, width, height);

            c.add(buttons[i]);
        }
    }

    private void initEventHandler() {
        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                ok = false;
                setVisible(false);
            }
        });

        // Search Button
        buttons[0].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ok = true;

                nameCard.setName(getText("Name"));
                nameCard.setCompany(getText("Company"));
                nameCard.setTitle(getText("Title"));
                nameCard.setAddress1(getText("Address1"));
                nameCard.setAddress2(getText("Address2"));
                nameCard.setPhone(getText("Phone"));
                nameCard.setEmail(getText("Email"));

                setVisible(false);
            }
        });

        // Cancel Button
        buttons[1].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                ok = false;
                setVisible(false);
            }
        });

        // Picture
        buttons[2].addActionListener(new ActionListener() {
            public void actionPerformed(ActionEvent e) {
                FileDialog dialog = new FileDialog(EditDialog.this.owner);
                dialog.show();

                String dir  = dialog.getDirectory();
                String file = dialog.getFile();

                if (file != null) {
                    Picture picture = new Picture(dir + file);
                    nameCard.setPicture(picture);
                    image = picture.getImage();
                    picturePanel.repaint();
                }
            }
        });
    }

    public boolean open(NameCard nameCard) {
        setNameCard(nameCard);
        ok = false;
        setVisible(true);

        return ok;
    }

    private void setText(String name, String text) {
        TextField textField = (TextField)textFields.get(name);
        textField.setText(text);
    }

    private String getText(String name) {
        TextField textField = (TextField)textFields.get(name);
        return textField.getText();
    }

    private void setNameCard(NameCard nameCard) {
        if (nameCard != null) {
            this.nameCard = nameCard;
            image = nameCard.getImage();

            setText("Name",     nameCard.getName());
            setText("Company",  nameCard.getCompany());
            setText("Title",    nameCard.getTitle());
            setText("Address1", nameCard.getAddress1());
            setText("Address2", nameCard.getAddress2());
            setText("Phone",    nameCard.getPhone());
            setText("Email",    nameCard.getEmail());
        } else {
            this.nameCard = new NameCard();
            image = null;

            setText("Name", "");
            setText("Company", "");
            setText("Title", "");
            setText("Address1", "");
            setText("Address2", "");
            setText("Phone", "");
            setText("Email", "");
        }
    }

    public NameCard getNameCard() {
        return nameCard;
    }
}
