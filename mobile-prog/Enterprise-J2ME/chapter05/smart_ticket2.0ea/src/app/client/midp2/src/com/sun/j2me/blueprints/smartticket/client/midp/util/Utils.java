

package com.sun.j2me.blueprints.smartticket.client.midp.util;

import javax.microedition.lcdui.ChoiceGroup;
import javax.microedition.lcdui.Choice;
import javax.microedition.lcdui.Image;
import java.util.Calendar;
import com.sun.j2me.blueprints.smartticket.client.midp.ui.UIController;
import com.sun.j2me.blueprints.smartticket.client.midp.ui.UIConstants;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.Theater;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;

final public class Utils {
    private Utils() {}

    /*
     * can't use static factory pattern, as the item will be added to different
     * forms -> which will cause IllegalStateException
     */
    public static ChoiceGroup newPopUp(UIController uiController, 
                                       String label, int start, int end) {
        ChoiceGroup choiceGroup = new ChoiceGroup(label, Choice.POPUP);

        for (int i = start; i <= end; i++) {
            choiceGroup.append(uiController.getString(i), null);
        } 

        return choiceGroup;
    } 

    public static ChoiceGroup newMonthPopUp(UIController uiController) {
        return newPopUp(uiController, 
                        uiController.getString(UIConstants.MONTH), 
                        UIConstants.MONTH_START, UIConstants.MONTH_END);
    } 

    public static ChoiceGroup newYearPopUp(UIController uiController) {
        ChoiceGroup yearList = 
            new ChoiceGroup(uiController.getString(UIConstants.YEAR), 
                            Choice.POPUP);

        /* temporary set 6 years onward */
        for (int i = UIConstants.CREDIT_CARD_START_YEAR; 
                i < UIConstants.CREDIT_CARD_START_YEAR + 6; i++) {
            yearList.append(i + "", null);
        } 

        return yearList;
    } 

    public static ChoiceGroup newCardPopUp(UIController uiController) {
        ChoiceGroup cardList = 
            new ChoiceGroup(uiController.getString(UIConstants.CARD_TYPE), 
                            Choice.POPUP);

        cardList.append(uiController.getString(UIConstants.VISA), null);
        cardList.append(uiController.getString(UIConstants.MASTERCARD), null);

        return cardList;
    } 

    public static void setExpirationDate(UIController uiController, 
                                         String exprDate, 
                                         ChoiceGroup monthList, 
                                         ChoiceGroup yearList) {
        /* the format of expiration Date is MonYYYY, i.e. Jan2003 */
        if (exprDate.length() != 7) {
            return;
        } 

        String month = exprDate.substring(0, 3);
        String year = exprDate.substring(3, 7);

        for (int i = 0; i < monthList.size(); i++) {
            if (month.equals(monthList.getString(i).substring(0, 3))) {
                monthList.setSelectedIndex(i, true);
            } 
        } 

        for (int i = 0; i < yearList.size(); i++) {
            if (year.equals(yearList.getString(i).substring(0, 3))) {
                yearList.setSelectedIndex(i, true);
            } 
        } 
    } 

    public static boolean setChoiceGroupEntry(UIController uiController, 
                                              int preference, 
                                              ChoiceGroup choiceGroup) {
        for (int i = 0; i < choiceGroup.size(); i++) {
            if (uiController.getString(preference).equals(choiceGroup.getString(i))) {
                choiceGroup.setSelectedIndex(i, true);

                return true;
            } 
        } 

        return false;
    } 

    public static String showTimeToDate(UIController uiController, 
                                        int[] showTime) {
        // Day.
        if (showTime[0] == Calendar.MONDAY) {
            return (uiController.getString(UIConstants.PREF_MON));
        } else if (showTime[0] == Calendar.TUESDAY) {
            return (uiController.getString(UIConstants.PREF_TUE));
        } else if (showTime[0] == Calendar.WEDNESDAY) {
            return (uiController.getString(UIConstants.PREF_WED));
        } else if (showTime[0] == Calendar.THURSDAY) {
            return (uiController.getString(UIConstants.PREF_THU));
        } else if (showTime[0] == Calendar.FRIDAY) {
            return (uiController.getString(UIConstants.PREF_FRI));
        } else if (showTime[0] == Calendar.SATURDAY) {
            return (uiController.getString(UIConstants.PREF_SAT));
        } else if (showTime[0] == Calendar.SUNDAY) {
            return (uiController.getString(UIConstants.PREF_SUN));
        } else {
            return "";
        } 
    } 

    public static String showTimeToHourMinute(UIController uiController, 
                                              int[] showTime) {
        StringBuffer buf = new StringBuffer();
        boolean am = true;

        if (showTime[1] > 12) {
            buf.append(showTime[1] - 12 + ":");

            am = false;
        } else {
            buf.append(showTime[1] + ":");

            if (showTime[1] < 12) {
                am = true;
            } 
        } 

        if (showTime[2] < 10) {
            buf.append('0');
            buf.append(showTime[2] + " ");
        } else {
            buf.append(showTime[2] + " ");
        } 

        if (am) {
            buf.append(uiController.getString(UIConstants.PREF_AM));
        } else {
            buf.append(uiController.getString(UIConstants.PREF_PM));
        } 

        return buf.toString();
    } 

    public static void appendTheaterList(UIController uiController, 
                                         Choice cg, Theater[] theaters, 
                                         Theater[] localTheaters) {
        for (int i = 0; i < theaters.length; i++) {
            byte imageIndex = UIConstants.ICON_IDX_SCHEDULE_REMOTE;

            for (int j = 0; j < localTheaters.length; j++) {
                if (theaters[i].getPrimaryKey().equals(localTheaters[j].getPrimaryKey())) {
                    try {
                        if (System.currentTimeMillis() 
                                > theaters[i].getTheaterSchedule().getExpirationDate()) {
                            imageIndex = 
                                UIConstants.ICON_IDX_SCHEDULE_EXPIRED;
                        } else {
                            imageIndex = UIConstants.ICON_IDX_SCHEDULE_LOCAL;
                        } 

                        break;    // next theater
                    } catch (ApplicationException ae) {
                        ae.printStackTrace();
                    } 
                } 
            } 

            cg.append(theaters[i].getName() + " [" + theaters[i].getZipCode() 
                      + "]", uiController.getImage(imageIndex));
        } 
    } 

}

