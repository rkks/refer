

/*
 * Copyright 2001, 2002, 2003 Sun Microsystems, Inc. All Rights Reserved.
 * Except for any files in PNG format (which are marked with the filename
 * extension ".png"), GIF format (which are marked with the filename
 * extension ".gif"), or JPEG format (which are marked with the filename
 * extension ".jpg"), redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the
 * following conditions are met:
 * - Redistribution of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * - Redistribution in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * You may compile, use, perform and display the following files with
 * original Java Smart Ticket Sample Application code obtained from Sun
 * Microsystems, Inc. only:
 * - files in PNG format and having the ".png" extension
 * - files in GIF format and having the ".gif" extension
 * - files in JPEG format and having the ".jpg" extension
 * You may not modify or redistribute .png, .gif, or .jpg files in any
 * form, in whole or in part, by any means without prior written
 * authorization from Sun Microsystems, Inc. and its licensors, if any.
 * Neither the name of Sun Microsystems, Inc., the 'Java Smart Ticket
 * Sample Application', 'Java', 'Java'-based names, or the names of
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * This software is provided "AS IS," without a warranty of any kind. ALL
 * EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. SUN
 * MIDROSYSTEMS, INC. ("SUN") AND ITS LICENSORS SHALL NOT BE LIABLE FOR
 * ANY DAMAGES SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR
 * DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES. IN NO EVENT WILL SUN OR
 * ITS LICENSORS BE LIABLE FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR
 * DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL, INCIDENTAL OR PUNITIVE
 * DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY OF LIABILITY,
 * ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF
 * SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * You acknowledge that this software is not designed, licensed or
 * intended for use in the design, construction, operation or maintenance
 * of any nuclear facility.
 * $Id: UIController.java,v 1.2 2003/04/09 21:33:36 ro89390 Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.ui;

import javax.microedition.lcdui.*;
import javax.microedition.midlet.*;
import java.io.*;
import com.sun.j2me.blueprints.smartticket.client.midp.model.ModelFacade;
import com.sun.j2me.blueprints.smartticket.client.midp.model.Preferences;
import com.sun.j2me.blueprints.smartticket.client.midp.model.ErrorMessageCodes;
import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.IndexedResourceBundle;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.SeatingPlan.Seat;

public class UIController {
    private static final String BASE_NAME_UI_RESOURCES = "UIResources2";
    private static final String PROPERTY_LOCALE = "SmartTicket-Locale";

    public static class EventIds {
        public static final byte EVENT_ID_ACCOUNTSETUPREQUESTED = 0;
        public static final byte EVENT_ID_ACCOUNTSETUPSAVED = 1;
        public static final byte EVENT_ID_PREFERENCESSAVED = 2;
        public static final byte EVENT_ID_MANAGETHEATERSCHEDULESREQUESTED = 3;
        public static final byte EVENT_ID_ACCOUNTINFOSAVED = 4;
        public static final byte EVENT_ID_DEFAULTSREQUESTED = 5;
        public static final byte EVENT_ID_CHOOSEMOVIEREQUESTED = 6;
        public static final byte EVENT_ID_LOOKUPTHEATERSCHEDULE = 7;
        public static final byte EVENT_ID_DOWNLOADTHEATERSCHEDULE = 8;
        public static final byte EVENT_ID_SELECTSEATSSELECTED = 9;
        public static final byte EVENT_ID_SEATSSELECTED = 10;
        public static final byte EVENT_ID_PURCHASEREQUESTED = 11;
        public static final byte EVENT_ID_RATEMYMOVIESREQUESTED = 12;
        public static final byte EVENT_ID_MOVIE_RATINGS_SAVE_REQUESTED = 13;
        public static final byte EVENT_ID_MOVIE_RATING_DELETE_REQUESTED = 14;
        public static final byte EVENT_ID_MOVIE_RATINGS_SYNC_REQUESTED = 15;
        public static final byte EVENT_ID_MOVIE_RATINGS_RESOLUTION_REQUESTED = 
            16;

        private EventIds() {}

    }

    private static final String[] iconPaths = {
        "/icons/g.png", "/icons/pg.png", "/icons/r.png", "/icons/q.png", 
        "/icons/fullstar.png", "/icons/halfstar.png", "/icons/remote.png", 
        "/icons/local.png", "/icons/expired.png", "/icons/smartTicket2.png", 
    };
    public Display display;
    private IndexedResourceBundle resourceBundle;
    private ModelFacade model;
    private AccountInfo accountInfo;
    private AccountInfo newAccountInfo;
    private Preferences preferences;
    private Preferences newPreferences;
    private Theater selectedTheater;
    private TheaterSchedule.MovieSchedule selectedMovieSchedule;
    private Movie selectedMovie;
    private int[] selectedShowTime;
    private Seat[] selectedSeats;
    private MovieRating[] selectedMovieRatings;
    private MovieRating[] resolvedMovieRatings;
    private Reservation reservation;
    private int theaterSelection;
    private Image[] icons = new Image[iconPaths.length];
    private Command mainMenuCommand;
    private Command exitCommand;
    private Command mySettingsCommand;
    // private Command showPreviewCommand;
    // private Command showByMoviesCommand;
    // private Command whatsNewCommand;
    // private Command aboutCommand;
    private Alert alert;
    private MainMenuUI mainMenuUI;
    private MySettingsUI mySettingsUI;
    private ManageTheaterSchedulesUI manageTheaterSchedulesUI;
    private DefaultsUI defaultsUI;
    private AccountSetupUI accountSetupUI;
    private ChooseMovieUI chooseMovieUI;
    private SeatingPlanUI seatingPlanUI;
    private SignOnUI signOnUI;
    private PurchaseTicketsUI purchaseTicketsUI;
    private ConfirmTicketUI confirmTicketUI;
    private PurchaseCompleteUI purchaseCompleteUI;
    private AccountInfoUI accountInfoUI;
    private ChangePasswordUI changePasswordUI;
    private BillingInfoUI billingInfoUI;
    private ProgressObserverUI progressObserverUI;
    private RateMyMoviesUI rateMyMoviesUI;
    private SyncMovieRatingsUI syncMovieRatingsUI;

    public UIController(MIDlet midlet, ModelFacade model) {
        this.display = Display.getDisplay(midlet);
        this.model = model;
    }

    public Display getDisplay() {
        return display;
    } 

    public String getString(int uiConstant) {
        return resourceBundle.getString(uiConstant);
    } 

    public void init() throws ApplicationException {
        resourceBundle = model.getResourceBundle(BASE_NAME_UI_RESOURCES);

        try {
            accountInfo = model.getAccountInfo();
            preferences = model.getPreferences();
        } catch (ApplicationException e) {
            System.err.println("Exception " + e);
        } 

        progressObserverUI = new ProgressObserverUI(this);

        model.setProgressObserver(progressObserverUI);

        mainMenuUI = new MainMenuUI(this);
        mySettingsUI = new MySettingsUI(this);
        manageTheaterSchedulesUI = new ManageTheaterSchedulesUI(this);
        defaultsUI = new DefaultsUI(this);
        signOnUI = new SignOnUI(this);
        accountSetupUI = new AccountSetupUI(this);
        chooseMovieUI = new ChooseMovieUI(this);
        seatingPlanUI = new SeatingPlanUI(this);
        purchaseTicketsUI = new PurchaseTicketsUI(this);
        confirmTicketUI = new ConfirmTicketUI(this);
        purchaseCompleteUI = new PurchaseCompleteUI(this);
        accountInfoUI = new AccountInfoUI(this);
        changePasswordUI = new ChangePasswordUI(this);
        billingInfoUI = new BillingInfoUI(this);
        rateMyMoviesUI = new RateMyMoviesUI(this);
        syncMovieRatingsUI = new SyncMovieRatingsUI(this);

        createCommands();
        setCommands(chooseMovieUI);
        setCommands(seatingPlanUI);
        setCommands(rateMyMoviesUI);
        setCommands(manageTheaterSchedulesUI);

        for (int i = 0; i < iconPaths.length; i++) {
            try {
                icons[i] = Image.createImage(iconPaths[i]);
            } catch (IOException ioe) {}
        } 

        Alert alert = new Alert(null, null, 
                                icons[UIConstants.ICON_IDX_SPLASH], null);

        if (accountInfo == null) {
            accountInfo = new AccountInfo();

            accountSetupUI.init(accountInfo, preferences);
            display.setCurrent(alert, accountSetupUI);
        } else {
            display.setCurrent(alert, mainMenuUI);
        } 
    } 

    public void destroy() {}

    public void commandAction(Command command, Displayable displayable) {
        if (command == mainMenuCommand) {
            mainMenuRequested();
        } else if (command == mySettingsCommand) {
            mySettingsRequested();
        } else if (command == exitCommand) {
            exitRequested();
        } 
    } 

    public void showErrorAlert(Exception e) {
        showErrorAlert(new ApplicationException(e), mainMenuUI);
    } 

    public void showErrorAlert(ApplicationException ae, Displayable d) {
        showErrorAlert(ae.getMessage(), d);
    } 

    public void showErrorAlert(String message) {
        showErrorAlert(message, display.getCurrent());
    } 

    private void showErrorAlert(String message, Displayable d) {
        Alert alert = new Alert(    // getString(UIMessageCodes.ERROR)
        "Error");

        alert.setType(AlertType.ERROR);
        alert.setTimeout(Alert.FOREVER);
        alert.setString(message);
        display.setCurrent(alert, d);
    } 

    public void showInfoAlert(String message, Displayable d) {
        showInfoAlert(null, message, d);
    } 

    public void showInfoAlert(String title, String message, Displayable d) {
        Alert alert = new Alert((title == null) 
                                ?    // getString(UIMessageCodes.SMART_TICKET)
        "Smart Ticket" : title);

        alert.setType(AlertType.INFO);
        alert.setTimeout(Alert.FOREVER);
        alert.setString(message);
        display.setCurrent(alert, d);
    } 

    public void runWithProgress(Thread thread, String title, 
                                boolean stoppable) {
        progressObserverUI.init(title, stoppable);
        getDisplay().setCurrent(progressObserverUI);
        thread.start();
    } 

    /**
     * Sign on, and eventually execute the given runnable after doing
     * so.
     */
    private void signOn(Runnable authorizedRunnable) {
        boolean isLoggedIn = false;

        try {
            model.setOffline(false);

            isLoggedIn = model.isLoggedIn();
        } catch (ApplicationException ae) {
            // Ignore. Therefore, isLoggedIn == false.
        } 

        if (isLoggedIn) {
            display.setCurrent(progressObserverUI);
            new Thread(authorizedRunnable).start();
        } else {
            String userName = accountInfo.getUserName();
            String password = accountInfo.getPassword();

            if (preferences.isSilentlyLoggingIn() && userName != null 
                    && password != null) {
                signOn(authorizedRunnable, userName, password);
            } else {
                signOnUI.init(authorizedRunnable);
                showInfoAlert(    // getString(UIMessageCodes.SIGN_IN_FIRST),
                // getString(UIMessageCodes.SIGN_IN_FIRST_LONG),
                "Sign in first", "You must sign in first", signOnUI);
            } 
        } 
    } 

    /**
     * Callback after user signs on, and finally executes the given
     * runnable.
     */
    public void signOn(final Runnable authorizedRunnable, 
                       final String userName, final String password) {
        Thread thread = new Thread() {
            public void run() {
                try {
                    model.login(userName, password);
                    authorizedRunnable.run();
                } catch (ApplicationException ae) {
                    if (authorizedRunnable instanceof EventDispatcher) {
                        showErrorAlert(ae, 
                                       ((EventDispatcher) authorizedRunnable).fallbackUI);
                    } else {
                        showErrorAlert(ae, signOnUI);
                    } 
                } catch (Exception e) {
                    showErrorAlert(e);
                } 
            } 

        };

        runWithProgress(thread, // getString(UIMessageCodes.SIGNING_IN),
        "Signing in...", false);
    } 

    public void mainMenuRequested() {
        display.setCurrent(mainMenuUI);
    } 

    public void mySettingsRequested() {
        display.setCurrent(mySettingsUI);
    } 

    public void accountSetupRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_ACCOUNTSETUPREQUESTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void accountSetupSaved(AccountInfo accountInfo, 
                                  Preferences preferences) {
        newAccountInfo = accountInfo;
        newPreferences = preferences;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_ACCOUNTSETUPSAVED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void preferencesExited() {
        mySettingsRequested();
    } 

    public void preferencesSaved(Preferences preferences) {
        newPreferences = preferences;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_PREFERENCESSAVED, mySettingsUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void changePasswordRequested() {
        if (accountInfo == null) {
            accountInfo = new AccountInfo();

            accountInfoUI.init(accountInfo);
        } 

        changePasswordUI.init(accountInfo);
        display.setCurrent(changePasswordUI);
    } 

    public void manageTheaterSchedulesRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_MANAGETHEATERSCHEDULESREQUESTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void accountInfoRequested() {
        if (accountInfo == null) {
            accountInfo = new AccountInfo();
        } 

        accountInfoUI.init(accountInfo);
        display.setCurrent(accountInfoUI);
    } 

    public void billingInfoSaved(AccountInfo accountInfo) {
        newAccountInfo = accountInfo;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_ACCOUNTINFOSAVED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void accountInfoSaved(AccountInfo accountInfo) {
        newAccountInfo = accountInfo;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_ACCOUNTINFOSAVED, mySettingsUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void defaultsRequested() {
        defaultsUI.init(preferences);
        display.setCurrent(defaultsUI);
    } 

    public void rateMyMoviesRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_RATEMYMOVIESREQUESTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void rateMyMoviesSyncRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_MOVIE_RATINGS_SYNC_REQUESTED, rateMyMoviesUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void rateMyMoviesSaveRequested(MovieRating[] movieRatings) {
        selectedMovieRatings = movieRatings;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_MOVIE_RATINGS_SAVE_REQUESTED, rateMyMoviesUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void rateMyMoviesDeleteRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_MOVIE_RATING_DELETE_REQUESTED, rateMyMoviesUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void rateMyMoviesResolutionRequested(MovieRating[] movieRatings) {
        resolvedMovieRatings = movieRatings;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_MOVIE_RATINGS_RESOLUTION_REQUESTED, rateMyMoviesUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void chooseMovieRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_CHOOSEMOVIEREQUESTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void lookupTheaterSchedule(Theater theater, int selection) {
        selectedTheater = theater;
        theaterSelection = selection;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_LOOKUPTHEATERSCHEDULE, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void downloadTheaterSchedule(Theater theater) {
        selectedTheater = theater;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_DOWNLOADTHEATERSCHEDULE, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void theaterScheduleDeletionRequested(Theater theater) {
        try {
            model.deleteTheater(theater.getPrimaryKey());
            manageTheaterSchedulesRequested();
        } catch (ApplicationException e) {
            System.err.println("Exception " + e);
        } 
    } 

    public void selectSeatsSelected(TheaterSchedule.MovieSchedule movieSchedule, 
                                    int[] showTime) {
        selectedShowTime = showTime;
        selectedMovie = movieSchedule.getMovie();
        selectedMovieSchedule = movieSchedule;

        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_SELECTSEATSSELECTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void seatsSelected(Seat[] seats) {
        selectedSeats = seats;

        /*
         * runWithProgress(
         * new EventDispatcher(EventIds.EVENT_ID_SEATSSELECTED, mainMenuUI),
         * getString(UIConstants.PROCESSING), false);
         */
        signOn(new EventDispatcher(EventIds.EVENT_ID_SEATSSELECTED, 
                                   seatingPlanUI));
    } 

    public void signOnConfirmed() {
        confirmTicketUI.init(selectedTheater.getName(), 
                             selectedMovie.getTitle(), selectedShowTime, 
                             selectedSeats);
        display.setCurrent(confirmTicketUI);
    } 

    public void purchaseRequested() {
        runWithProgress(new EventDispatcher(EventIds.EVENT_ID_PURCHASEREQUESTED, mainMenuUI), 
                        getString(UIConstants.PROCESSING), false);
    } 

    public void exitRequested() {
        System.out.println("Bye Bye");
        // FIXME - Not yet implemented.
    } 

    class EventDispatcher extends Thread {
        private int taskId;
        private Displayable fallbackUI;

        EventDispatcher(int taskId, Displayable fallbackUI) {
            this.taskId = taskId;
            this.fallbackUI = fallbackUI;

            return;
        }

        public void run() {
            try {
                switch (taskId) {

                case EventIds.EVENT_ID_ACCOUNTSETUPREQUESTED: {
                    AccountInfo accountInfo = new AccountInfo();

                    accountSetupUI.init(accountInfo, preferences);
                    display.setCurrent(accountSetupUI);

                    break;
                } 

                case EventIds.EVENT_ID_ACCOUNTSETUPSAVED: {
                    model.createAccount(newAccountInfo);
                    model.setPreferences(newPreferences);

                    accountInfo = newAccountInfo;
                    preferences = newPreferences;

                    billingInfoUI.init(accountInfo);
                    display.setCurrent(billingInfoUI);

                    break;
                } 

                case EventIds.EVENT_ID_PREFERENCESSAVED: {
                    model.setPreferences(newPreferences);

                    preferences = newPreferences;

                    preferencesExited();

                    break;
                } 

                case EventIds.EVENT_ID_MANAGETHEATERSCHEDULESREQUESTED: {
                    // will it possible preference is null?
                    Theater[] theaterList = getTheaterList();

                    if (theaterList == null) {
                        showErrorAlert(getString(UIConstants.NO_ZIP_ERROR), 
                                       mainMenuUI);
                    } else {
                        manageTheaterSchedulesUI.init(theaterList, 
                                                      model.getPersistedTheaters(null));
                        display.setCurrent(manageTheaterSchedulesUI);
                    } 

                    break;
                } 

                case EventIds.EVENT_ID_ACCOUNTINFOSAVED: {
                    model.setAccountInfo(newAccountInfo);

                    accountInfo = newAccountInfo;

                    display.setCurrent(fallbackUI);

                    break;
                } 

                case EventIds.EVENT_ID_CHOOSEMOVIEREQUESTED: {
                    model.setOffline(true);

                    if (accountInfo == null) {
                        accountSetupRequested();
                    } else {
                        Theater[] theaterList = getTheaterList();
                        Theater[] persistedTheaters = 
                            model.getPersistedTheaters(null);
                        Theater[] cachedTheaters = 
                            model.getCachedTheaters(null);
                        /* merge the persisted and cached theater list */
                        Theater[] theaters = 
                            new Theater[persistedTheaters.length + cachedTheaters.length];

                        System.arraycopy(persistedTheaters, 0, theaters, 0, 
                                         persistedTheaters.length);
                        System.arraycopy(cachedTheaters, 0, theaters, 
                                         persistedTheaters.length, 
                                         cachedTheaters.length);
                        chooseMovieUI.setTheaters(theaterList, theaters);
                        chooseMovieUI.init(model.getPreferences());
                        display.setCurrent(chooseMovieUI);
                    } 

                    break;
                } 

                case EventIds.EVENT_ID_LOOKUPTHEATERSCHEDULE: {
                    model.setOffline(false);

                    TheaterSchedule theaterSchedule = 
                        selectedTheater.getTheaterSchedule();

                    if (theaterSchedule != null) {
                        chooseMovieUI.theaterScheduleDownloadSuccess(theaterSchedule, 
                                theaterSelection);
                        display.setCurrent(chooseMovieUI);
                    } 

                    break;
                } 

                case EventIds.EVENT_ID_DOWNLOADTHEATERSCHEDULE: {
                    TheaterSchedule theaterSchedule = 
                        selectedTheater.getTheaterSchedule();

                    preferences.addUpdatableTheaterScheduleKey(selectedTheater.getPrimaryKey());
                    model.persistTheaterSchedule(selectedTheater);
                    model.setPreferences(preferences);
                    manageTheaterSchedulesRequested();

                    break;
                } 

                case EventIds.EVENT_ID_SELECTSEATSSELECTED: {
                    SeatingPlan seatingPlan = 
                        selectedMovieSchedule.getSeatingPlan(selectedShowTime);
                    String movieName = selectedMovie.getTitle();

                    seatingPlanUI.init(selectedTheater.getName(), movieName, 
                                       seatingPlan, selectedShowTime);
                    display.setCurrent(seatingPlanUI);

                    break;
                } 

                case EventIds.EVENT_ID_SEATSSELECTED: {
                    reservation = 
                        model.reserveSeats(selectedTheater.getPrimaryKey(), 
                                           selectedMovie.getPrimaryKey(), 
                                           selectedShowTime, selectedSeats);

                    purchaseTicketsUI.init(model.getAccountInfo());
                    display.setCurrent(purchaseTicketsUI);

                    break;
                } 

                case EventIds.EVENT_ID_PURCHASEREQUESTED: {
                    model.purchaseTickets(reservation);
                    purchaseCompleteUI.init(reservation.getId(), 
                                            selectedTheater.getName(), 
                                            selectedMovie.getTitle(), 
                                            selectedShowTime);
                    display.setCurrent(purchaseCompleteUI);

                    break;
                } 

                case EventIds.EVENT_ID_RATEMYMOVIESREQUESTED: {
                    MovieRating[] movieRatings = model.getMovieRatings();
                    MovieRating[] nonDeletedMovieRatings = 
                        new MovieRating[movieRatings.length];
                    int j = 0;

                    for (int i = 0; i != movieRatings.length; i++) {
                        if (movieRatings[i].getModificationStatus() 
                                != Synchronizable.STATUS_DELETED) {
                            nonDeletedMovieRatings[j++] = movieRatings[i];
                        } 
                    } 

                    rateMyMoviesUI.init(nonDeletedMovieRatings);
                    display.setCurrent(rateMyMoviesUI);

                    break;
                } 

                case EventIds.EVENT_ID_MOVIE_RATING_DELETE_REQUESTED: {
                    // TODO
                    // selectedMovieRating.setModificationStatus(Synchronizable.STATUS_DELETED);
                    // model.setMovieRating(selectedMovieRating);
                    taskId = EventIds.EVENT_ID_RATEMYMOVIESREQUESTED;

                    run();

                    break;
                } 

                case EventIds.EVENT_ID_MOVIE_RATINGS_SYNC_REQUESTED: {
                    progressObserverUI.init(getString(UIConstants.SYNC_MOVIE_RATINGS), 
                                            false);

                    int strategyId = 
                        preferences.getConflictResolutionStrategyId();

                    if (strategyId != Synchronizable.STRATEGY_ASK_USER) {
                        model.synchronizeMovieRatings(strategyId);
                        model.commitMovieRatings(new MovieRating[0]);

                        taskId = EventIds.EVENT_ID_RATEMYMOVIESREQUESTED;

                        run();
                        showInfoAlert(getString(UIConstants.SYNC_SUCCESSFUL), 
                                      rateMyMoviesUI);
                    } else {
                        MovieRating[][] movieRatingPairs = 
                            model.synchronizeMovieRatings();

                        if (movieRatingPairs.length != 0) {
                            syncMovieRatingsUI.init(movieRatingPairs);
                            showInfoAlert(getString(UIConstants.CONFLICTING_RATINGS_LONG), 
                                          syncMovieRatingsUI);
                        } else {
                            model.commitMovieRatings(new MovieRating[0]);

                            taskId = EventIds.EVENT_ID_RATEMYMOVIESREQUESTED;

                            run();
                            showInfoAlert(getString(UIConstants.SYNC_SUCCESSFUL), 
                                          rateMyMoviesUI);
                        } 
                    } 

                    break;
                } 

                case EventIds.EVENT_ID_MOVIE_RATINGS_SAVE_REQUESTED: {
                    for (int i = 0; i < selectedMovieRatings.length; i++) {
                        model.setMovieRating(selectedMovieRatings[i]);
                    } 

                    taskId = EventIds.EVENT_ID_RATEMYMOVIESREQUESTED;

                    run();
                    display.setCurrent(rateMyMoviesUI);

                    break;
                } 

                case EventIds.EVENT_ID_MOVIE_RATINGS_RESOLUTION_REQUESTED: {
                    progressObserverUI.init(getString(UIConstants.SYNC_MOVIE_RATINGS), 
                                            false);
                    model.commitMovieRatings(resolvedMovieRatings);

                    taskId = EventIds.EVENT_ID_RATEMYMOVIESREQUESTED;

                    run();
                    showInfoAlert(getString(UIConstants.SYNC_SUCCESSFUL), 
                                  rateMyMoviesUI);

                    break;
                } 
                }    // for switch - case
            } catch (ApplicationException ae) {
                if (ae.getCode() 
                        == ErrorMessageCodes.ERROR_OPERATION_INTERRUPTED) {
                    display.setCurrent(fallbackUI);
                } else {
                    showErrorAlert(ae.getMessage(), fallbackUI);
                } 
            } catch (Exception e) {
                e.printStackTrace();
                showErrorAlert(e.getMessage());
            } 
        } 

    }

    private void createCommands() {
        exitCommand = new Command(getString(UIConstants.EXIT), Command.EXIT, 
                                  1);
        mainMenuCommand = new Command(getString(UIConstants.MAIN_MENU), 
                                      Command.OK, 3);
        mySettingsCommand = new Command(getString(UIConstants.MY_SETTINGS), 
                                        Command.OK, 3);
        // aboutCommand = new Command(getString(
        // UIConstants.ABOUT), Command.OK, 6);
        // showPreviewCommand = new Command(getString(
        // UIConstants.SHOW_PREVIEW), Command.OK, 3);
        // showByMoviesCommand = new Command(getString(
        // UIConstants.SHOW_BY_MOVIES), Command.OK, 3);
        // whatsNewCommand = new Command(getString(
        // UIConstants.WHATS_NEW), Command.OK, 5);
    } 

    private void setCommands(Displayable displayable) {
        displayable.addCommand(exitCommand);
        displayable.addCommand(mainMenuCommand);
        // displayable.addCommand(showPreviewCommand);
        // displayable.addCommand(showByMoviesCommand);
        // displayable.addCommand(whatsNewCommand);
        displayable.addCommand(mySettingsCommand);
        // displayable.addCommand(aboutCommand);
    } 

    private Theater[] getTheaterList() throws ApplicationException {
        Preferences pref = model.getPreferences();
        String zipCode = pref.getDefaultZipCode();
        String zipCode2 = pref.getSecondZipCode();

        if (zipCode.length() == 0) {
            return null;
        } 

        Theater[] theaterList = model.getTheaters(zipCode);

        if (zipCode2.length() != 0) {
            Theater[] theaterList2 = model.getTheaters(zipCode2);
            Theater[] theaters = 
                new Theater[theaterList.length + theaterList2.length];

            System.arraycopy(theaterList, 0, theaters, 0, theaterList.length);
            System.arraycopy(theaterList2, 0, theaters, theaterList.length, 
                             theaterList2.length);

            theaterList = theaters;
        } 

        return theaterList;
    } 

    public Image getImage(byte imageIndex) {
        return icons[imageIndex];
    } 

}

