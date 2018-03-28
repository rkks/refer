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
import java.util.*;

public class NameCardList extends java.awt.List {
    // Fields
    private Vector nameCards;
    private DBManager manager;

    // Constructors
    public NameCardList() {
        manager = DBManager.getInstance();
        nameCards = manager.getAllNameCards();
        updateList();
    }

    // Methods
    public void updateList() {
        setVisible(false);

        int index = getSelectedIndex();

        removeAll();

        Enumeration e = nameCards.elements();
        while (e.hasMoreElements()) {
            NameCard nameCard = (NameCard) e.nextElement();
            add(nameCard.getName());
        }

        if (getItemCount() > 0) {
            if (getItemCount() > index) {
                select(index);
            } else {
                select(getItemCount() - 1);
            }
        }

        setVisible(true);
    }

    public NameCard getSelectedNameCard() {
        int index = getSelectedIndex();
        return (NameCard) nameCards.elementAt(index);
    }

    public void addNameCard(NameCard nameCard) {
        manager.addNameCard(nameCard);
        nameCards.addElement(nameCard);
        updateList();
    }

    public void updateNameCard(NameCard nameCard) {
        manager.updateNameCard(nameCard);
        updateList();
    }

    public void deleteNameCard(NameCard nameCard) {
        manager.deleteNameCard(nameCard);
        nameCards.removeElement(nameCard);
        updateList();
    }

    public void setNameCards(Vector nameCards) {
        this.nameCards = nameCards;
        updateList();
    }
}

