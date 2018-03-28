

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
 * $Id: SynchronizingBean.java,v 1.10 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing;

import javax.ejb.*;
import javax.naming.*;
import java.rmi.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingData;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome;

/**
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SynchronizingLocal}>
 * @ejbDontSynchronizeNames
 * @ejbStateful
 * @ejbJNDIname ejb/smartticket/smartticket/Synchronizing
 * @ejbLocalJNDIname ejb/smartticket/smartticket/Synchronizing
 * @displayName SynchronizingSB
 * @undefined
 * @ejbTransactionAttribute Required
 * @ejbLocalHome <{SynchronizingLocalHome}>
 */
public class SynchronizingBean implements SessionBean {

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal
     * @EJBLink MovieBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Movie
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE = "ejb/Movie";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal
     * @EJBLink MovieRatingBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/MovieRating
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE_RATING = "ejb/MovieRating";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal
     * @EJBLink AccountBean
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome
     * @REF_JNDI_NAME ejb/smartticket/smartticket/Account
     * @undefined
     */
    private static final String EJB_REF_ACCOUNT = "ejb/Account";

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncClientLocal
     * @EJBLink AccountBean
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncClientLocalHome
     * @REF_JNDI_NAME ejb/smartticket/smartticket/SyncClient
     * @undefined
     */
    private static final String EJB_REF_SYNC_CLIENT = "ejb/SyncClient";
    private SessionContext ctx;
    private Context context;
    private AccountLocal account;
    private MovieRatingLocalHome movieRatingHome;
    private MovieLocalHome movieHome;
    private SyncClientLocalHome syncClientHome;
    private SyncConduitLocal syncConduit;
    private SyncClientLocal syncClient = null;
    private HashMap updatedMovieRatings = new HashMap();
    private long initiationTime;
    private long timeShift;

    /**
     * @link dependency
     */
    /* # AccountBean lnkAccountBean; */

    /**
     * @link dependency
     */
    /* # SyncConduitBean lnkSyncConduitBean; */
    public void setSessionContext(SessionContext context) 
            throws RemoteException, EJBException {
        ctx = context;
    } 

    public void ejbActivate() throws EJBException {}

    public void ejbPassivate() throws EJBException {}

    public void ejbRemove() throws EJBException {}

    public void ejbCreate(AccountLocal account) 
            throws CreateException, EJBException {
        this.account = account;

        try {
            context = (Context) new InitialContext().lookup("java:comp/env");
            movieRatingHome = 
                (MovieRatingLocalHome) context.lookup(EJB_REF_MOVIE_RATING);
            movieHome = (MovieLocalHome) context.lookup(EJB_REF_MOVIE);
            syncClientHome = 
                (SyncClientLocalHome) context.lookup(EJB_REF_SYNC_CLIENT);
        } catch (Exception e) {
            throw new CreateException("SynchronizingBean.ejbCreate(): " 
                                      + e.getMessage());
        } 
    } 

    public SyncClientAnchor initiateSynchronization(SyncClientAnchor syncClientAnchor, 
            long clientTime) {
        syncConduit = account.getMovieRatingSyncConduit();

        if (syncClientAnchor.getClientId() == -1) {
            try {
                syncClient = 
                    syncClientHome.create(syncConduit.makeSyncClientId());

                syncConduit.getSyncClients().add(syncClient);
                syncClient.setLastSynchronizationTime(0L);
            } catch (CreateException ce) {
                throw new EJBException("SynchronizingBean.initiateSynchronization: " 
                                       + ce.getMessage());
            } 
        } else if (syncClient == null) {
            syncClient = 
                syncConduit.getSyncClient(syncClientAnchor.getClientId());

            if (syncClient == null) {
                try {
                    syncClient = 
                        syncClientHome.create(syncConduit.makeSyncClientId());

                    syncConduit.getSyncClients().add(syncClient);

                    return new SyncClientAnchor(syncClient.getId(), 0L, true);
                } catch (CreateException ce) {
                    throw new EJBException("SynchronizingBean.initiateSynchronization: " 
                                           + ce.getMessage());
                } 
            } 
        } else if (syncClient.getId() != syncClientAnchor.getClientId()) {
            syncClient = 
                syncConduit.getSyncClient(syncClientAnchor.getClientId());

            if (syncClient == null) {
                try {
                    syncClient = 
                        syncClientHome.create(syncConduit.makeSyncClientId());

                    syncConduit.getSyncClients().add(syncClient);

                    return new SyncClientAnchor(syncClient.getId(), 0L, true);
                } catch (CreateException ce) {
                    throw new EJBException("SynchronizingBean.initiateSynchronization: " 
                                           + ce.getMessage());
                } 
            } 
        } 

        if (syncClientAnchor.getClientId() == -1 
                || syncClientAnchor.getLastSyncTime() 
                   == syncClient.getLastSynchronizationTime()) {
            initiationTime = new Date().getTime();
            timeShift = 
                initiationTime 
                - clientTime;    // approximate due to network latency and so on..

            return new SyncClientAnchor(syncClient.getId(), initiationTime, 
                                        false);
        } 

        syncClient.setLastSynchronizationTime(0L);    // to force slow sync

        return new SyncClientAnchor(syncClient.getId(), 0L, true);
    } 

    public MovieRatingData[] synchronizeMovieRatings(MovieRatingData[] movieRatings, 
            int conflictResolutionStrategyId) {
        try {
            syncConduit = account.getMovieRatingSyncConduit();

            Collection serverSideChanges = 
                syncConduit.getChangedSyncMetaData(syncClient.getLastSynchronizationTime());
            Map clientSideChanges = movieRatingsArrayToMap(movieRatings);

            updatedMovieRatings.clear();

            if (conflictResolutionStrategyId 
                    == SyncConstants.STRATEGY_FIRST_MODIFIED_WINS 
                    || conflictResolutionStrategyId 
                       == SyncConstants.STRATEGY_LAST_MODIFIED_WINS) {
                // Handle conflicting changes and build the list of updates
                for (Iterator i = serverSideChanges.iterator(); 
                        i.hasNext(); ) {
                    SyncMetaDataLocal serverSideChange = 
                        (SyncMetaDataLocal) i.next();
                    MovieRatingData clientSideChange = 
                        (MovieRatingData) clientSideChanges.get(serverSideChange.getId());

                    if (clientSideChange != null) {
                        // Align the time
                        clientSideChange.alignTime(timeShift);

                        // Conflicting changes
                        if ((conflictResolutionStrategyId == SyncConstants.STRATEGY_FIRST_MODIFIED_WINS && serverSideChange.getLastModificationTime() > clientSideChange.getLastModificationDate()) 
                                || (conflictResolutionStrategyId 
                                    == SyncConstants.STRATEGY_LAST_MODIFIED_WINS 
                                       && serverSideChange.getLastModificationTime() 
                                          < clientSideChange.getLastModificationDate())) {
                            if (clientSideChange.getModificationStatus() 
                                    == SyncConstants.STATUS_DELETED) {
                                removeMovieRating(clientSideChange.getMovieId());
                            } else {
                                updateMovieRating(clientSideChange);
                            } 
                        } else {
                            if (serverSideChange.getDeleted()) {
                                if (clientSideChange.getModificationStatus() 
                                        != SyncConstants.STATUS_DELETED) {
                                    clientSideChange.setModificationStatus(SyncConstants.STATUS_DELETED);
                                    updatedMovieRatings.put(clientSideChange.getMovieId(), 
                                                            clientSideChange);
                                } 
                            } else {
                                updatedMovieRatings.put(serverSideChange.getId(), 
                                                        account.getMovieRating(serverSideChange.getId()).getData());
                            } 
                        } 

                        clientSideChanges.remove(clientSideChange.getMovieId());
                    } else {
                        // Non-conflicting changes
                        MovieRatingData movieRating = 
                            !serverSideChange.getDeleted() 
                            ? account.getMovieRating(serverSideChange.getId()).getData() 
                            : new MovieRatingData(serverSideChange.getId(), 
                                                  serverSideChange.getLastModificationTime());

                        updatedMovieRatings.put(serverSideChange.getId(), 
                                                movieRating);
                    } 
                } 

                // Apply directly remaining non-conflicting client-side changes
                for (Iterator i = clientSideChanges.values().iterator(); 
                        i.hasNext(); ) {
                    MovieRatingData clientSideChange = 
                        (MovieRatingData) i.next();

                    updateMovieRating(clientSideChange);
                } 

                syncClient.setLastSynchronizationTime(initiationTime);

                return (MovieRatingData[]) new ArrayList(updatedMovieRatings.values()).toArray(new MovieRatingData[0]);
            } else {    // SyncConstants.STRATEGY_ASK_USER
                HashMap conflictingMovieRatings = new HashMap();

                // Handle conflicting changes and build the list of updates
                for (Iterator i = serverSideChanges.iterator(); 
                        i.hasNext(); ) {
                    SyncMetaDataLocal serverSideChange = 
                        (SyncMetaDataLocal) i.next();
                    MovieRatingData clientSideChange = 
                        (MovieRatingData) clientSideChanges.get(serverSideChange.getId());

                    if (clientSideChange != null) {
                        MovieRatingData movieRating = 
                            !serverSideChange.getDeleted() 
                            ? account.getMovieRating(serverSideChange.getId()).getData() 
                            : new MovieRatingData(serverSideChange.getId(), 
                                                  serverSideChange.getLastModificationTime());

                        conflictingMovieRatings.put(serverSideChange.getId(), 
                                                    movieRating);
                        clientSideChanges.remove(clientSideChange.getMovieId());
                    } else {
                        // Non-conflicting changes
                        MovieRatingData movieRating = 
                            !serverSideChange.getDeleted() 
                            ? account.getMovieRating(serverSideChange.getId()).getData() 
                            : new MovieRatingData(serverSideChange.getId(), 
                                                  serverSideChange.getLastModificationTime());

                        updatedMovieRatings.put(serverSideChange.getId(), 
                                                movieRating);
                    } 
                } 

                // Apply directly remaining non-conflicting client-side changes
                for (Iterator i = clientSideChanges.values().iterator(); 
                        i.hasNext(); ) {
                    MovieRatingData clientSideChange = 
                        (MovieRatingData) i.next();

                    if (clientSideChange.getModificationStatus() 
                            == SyncConstants.STATUS_DELETED) {
                        removeMovieRating(clientSideChange.getMovieId());
                    } else {
                        updateMovieRating(clientSideChange);
                    } 
                } 

                return (MovieRatingData[]) new ArrayList(conflictingMovieRatings.values()).toArray(new MovieRatingData[0]);
            } 
        } catch (Exception e) {
            e.printStackTrace(System.err);

            throw new EJBException("SynchronizingBean.synchronizeMovieRatings: " 
                                   + e);
        } 
    } 

    public MovieRatingData[] commitMovieRatings(MovieRatingData[] movieRatings) {
        try {
            syncConduit = account.getMovieRatingSyncConduit();

            for (int i = 0; i < movieRatings.length; i++) {
                MovieRatingData change = movieRatings[i];

                if (change.isModifiedOnClient()) {
                    if (change.getModificationStatus() 
                            == SyncConstants.STATUS_DELETED) {
                        removeMovieRating(change.getMovieId());
                    } else {
                        updateMovieRating(change);
                    } 
                } else {
                    if (change.getModificationStatus() 
                            == SyncConstants.STATUS_DELETED) {
                        updatedMovieRatings.put(change.getMovieId(), change);
                    } else {
                        MovieRatingLocal movieRating = 
                            account.getMovieRating(change.getMovieId());

                        if (movieRating != null) {
                            updatedMovieRatings.put(change.getMovieId(), 
                                                    movieRating.getData());
                        } 
                    } 
                } 
            } 

            syncClient.setLastSynchronizationTime(initiationTime);

            return (MovieRatingData[]) new ArrayList(updatedMovieRatings.values()).toArray(new MovieRatingData[0]);
        } catch (Exception e) {
            throw new EJBException("SynchronizingBean.synchronizeMovieRatings: " 
                                   + e);
        } 
    } 

    private Map movieRatingsArrayToMap(MovieRatingData[] movieRatings) {
        Map map = new HashMap();

        for (int i = 0; i < movieRatings.length; i++) {
            map.put(movieRatings[i].getMovieId(), movieRatings[i]);
        } 

        return map;
    } 

    private void updateMovieRating(MovieRatingData data) 
            throws FinderException, CreateException, RemoveException {
        MovieRatingLocal movieRating = 
            account.getMovieRating(data.getMovieId());

        if (movieRating == null) {
            SyncMetaDataLocal metaData = 
                syncConduit.getSyncMetaData(data.getMovieId());

            if (metaData != null) {
                metaData.remove();
            } 

            MovieLocal movie = movieHome.findByPrimaryKey(data.getMovieId());

            movieRating = movieRatingHome.create(movie, account);
        } 

        movieRating.setData(data, initiationTime);

        return;
    } 

    private void removeMovieRating(String movieId) 
            throws FinderException, CreateException, RemoveException {
        MovieRatingLocal movieRating = account.getMovieRating(movieId);

        if (movieRating != null) {
            movieRating.remove();
        } 

        return;
    } 

}

