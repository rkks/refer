

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
 * $Id: RMSCacheHandler.java,v 1.2 2003/03/26 22:22:08 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.client.midp.model;

import com.sun.j2me.blueprints.smartticket.shared.midp.ApplicationException;
import com.sun.j2me.blueprints.smartticket.shared.midp.model.*;

public class RMSCacheHandler extends RemoteModelRequestHandler {

    /**
     * @link aggregation
     */
    private RMSAdapter rmsAdapter;
    private boolean offline = true;
    private Theater cachedTheater = null;
    private TheaterSchedule cachedTheaterSchedule = null;

    /**
     * Constructs a new RMSCacheHandler which is to be inserted in a RequestHandler chain.
     * @param nextHandler The next handler in the chain.
     * @param progressObserver The progress observer.
     */
    public RMSCacheHandler(RemoteModelRequestHandler nextHandler) 
            throws ApplicationException {
        super(nextHandler);

        rmsAdapter = new RMSAdapter();

        return;
    }

    /**
     * Notifies of the destruction of the handler chain. This RMSCacheHandler when destroyed, caches in RMS the last accessed
     * Theater and TheaterSchedule and closes the RMS record stores.
     */
    public void destroy() throws ApplicationException {
        if (cachedTheater != null && cachedTheaterSchedule != null) {
            if (cachedTheaterSchedule.getExpirationDate() 
                    > System.currentTimeMillis()) {
                try {
                    removeCachedTheaterSchedule();
                    storeTheaterSchedule(cachedTheater, 
                                         cachedTheaterSchedule, 
                                         IndexEntry.MODE_CACHING);
                } catch (ModelException me) {
                    throw new ApplicationException(me);    // Fix me to pass the appriate error message code
                } 
            } 
        } 

        rmsAdapter.closeRecordStores();
        super.destroy();

        return;
    } 

    public boolean isOffline() throws ApplicationException {
        return offline || super.isOffline();
    } 

    public void setOffline(boolean offline) throws ApplicationException {
        this.offline = offline;

        super.setOffline(offline);

        return;
    } 

    public TheaterSchedule getTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        if (cachedTheater != null && cachedTheaterSchedule != null 
                && cachedTheater.equals(theater)) {
            if (cachedTheaterSchedule.getExpirationDate() 
                    > System.currentTimeMillis()) {
                return cachedTheaterSchedule;
            } 
        } 

        IndexEntry indexEntry = 
            rmsAdapter.getIndexEntry(theater.getPrimaryKey(), 
                                     IndexEntry.TYPE_THEATER_SCHEDULE, 
                                     IndexEntry.MODE_ANY);

        if (indexEntry != null) {
            if (isOffline() 
                    || indexEntry.getExpirationDate() 
                       > System.currentTimeMillis()) {
                return rmsAdapter.loadTheaterSchedule(indexEntry.getRecordId());
            } else if (getPreferences().isUpdatable(theater.getPrimaryKey())) {
                return downloadTheaterSchedule(theater);
            } else {
                // delete expired theater schedule which are not updatable
                rmsAdapter.deleteTheaterSchedule(indexEntry.getRecordId());
                rmsAdapter.deleteIndexEntry(indexEntry.getKey(), 
                                            indexEntry.getType());

                indexEntry = rmsAdapter.getIndexEntry(indexEntry.getKey(), 
                                                      IndexEntry.TYPE_THEATER, 
                                                      IndexEntry.MODE_ANY);

                if (indexEntry != null) {
                    rmsAdapter.deleteTheater(indexEntry.getRecordId());
                    rmsAdapter.deleteIndexEntry(indexEntry.getKey(), 
                                                indexEntry.getType());
                } 
                // Movies are to be removed (garbage collected) by an
                // independent thread using the cleanup method
            } 
        } 

        TheaterSchedule theaterSchedule = 
            super.getTheaterSchedule(theater.getPrimaryKey());

        if (getPreferences().isCaching()) {
            cachedTheater = theater;
            cachedTheaterSchedule = theaterSchedule;
        } 

        return theaterSchedule;
    } 

    public TheaterSchedule downloadTheaterSchedule(Theater theater) 
            throws ModelException, ApplicationException {
        TheaterSchedule theaterSchedule = 
            super.getTheaterSchedule(theater.getPrimaryKey());

        storeTheaterSchedule(theater, theaterSchedule, 
                             IndexEntry.MODE_PERSISTING);

        if (cachedTheater != null && cachedTheater.equals(theater)) {
            cachedTheater = null;
            cachedTheaterSchedule = null;
        } 

        return theaterSchedule;
    } 

    public Movie getMovie(String movieKey) 
            throws ModelException, ApplicationException {
        IndexEntry indexEntry = rmsAdapter.getIndexEntry(movieKey, 
                IndexEntry.TYPE_MOVIE, IndexEntry.MODE_ANY);

        if (indexEntry != null) {
            return rmsAdapter.loadMovie(indexEntry.getRecordId());
        } 

        return super.getMovie(movieKey);
    } 

    private void storeTheaterSchedule(Theater theater, 
                                      TheaterSchedule theaterSchedule, 
                                      int mode) throws ModelException, 
                                      ApplicationException {
        IndexEntry indexEntry = 
            rmsAdapter.getIndexEntry(theater.getPrimaryKey(), 
                                     IndexEntry.TYPE_THEATER_SCHEDULE, 
                                     IndexEntry.MODE_ANY);
        int recordId = 
            rmsAdapter.storeTheaterSchedule(theaterSchedule /* , mode == IndexEntry.MODE_PERSISTING */, 
                                            indexEntry != null 
                                            ? indexEntry.getRecordId() : -1);

        if (indexEntry == null) {
            indexEntry = new IndexEntry(recordId, 
                                        IndexEntry.TYPE_THEATER_SCHEDULE, 
                                        theater.getPrimaryKey(), mode);
        } else if ((mode == IndexEntry.MODE_PERSISTING || indexEntry.getMode() == IndexEntry.MODE_PERSISTING) 
                   && mode != indexEntry.getMode()) {
            // if persisting: overwrite caching entries
            indexEntry = new IndexEntry(recordId, indexEntry.getType(), 
                                        indexEntry.getKey(), 
                                        IndexEntry.MODE_PERSISTING);
        } 

        indexEntry.setExpirationDate(theaterSchedule.getExpirationDate());
        rmsAdapter.addIndexEntry(indexEntry);

        indexEntry = rmsAdapter.getIndexEntry(theater.getPrimaryKey(), 
                                              IndexEntry.TYPE_THEATER, 
                                              IndexEntry.MODE_ANY);

        if (indexEntry == null) {
            recordId = rmsAdapter.storeTheater(theater, -1);
            indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_THEATER, 
                                        theater.getPrimaryKey(), mode);

            rmsAdapter.addIndexEntry(indexEntry);
        } else if ((mode == IndexEntry.MODE_PERSISTING || indexEntry.getMode() == IndexEntry.MODE_PERSISTING) 
                   && mode != indexEntry.getMode()) {
            // if persisting: overwrite caching entries
            indexEntry = new IndexEntry(indexEntry.getRecordId(), 
                                        indexEntry.getType(), 
                                        indexEntry.getKey(), 
                                        IndexEntry.MODE_PERSISTING);

            rmsAdapter.addIndexEntry(indexEntry);
        } 

        TheaterSchedule.MovieSchedule[] movieSchedules = 
            theaterSchedule.getMovieSchedules();

        for (int i = 0; i < movieSchedules.length; i++) {
            Movie movie = movieSchedules[i].getMovie();

            indexEntry = rmsAdapter.getIndexEntry(movie.getPrimaryKey(), 
                                                  IndexEntry.TYPE_MOVIE, 
                                                  IndexEntry.MODE_ANY);

            if (indexEntry == null) {
                recordId = rmsAdapter.storeMovie(movie, -1);
                indexEntry = new IndexEntry(recordId, IndexEntry.TYPE_MOVIE, 
                                            movie.getPrimaryKey(), mode);

                rmsAdapter.addIndexEntry(indexEntry);
            } else if ((mode == IndexEntry.MODE_PERSISTING || indexEntry.getMode() == IndexEntry.MODE_PERSISTING) 
                       && mode != indexEntry.getMode()) {
                // if persisting: overwrite caching entries
                indexEntry = new IndexEntry(indexEntry.getRecordId(), 
                                            indexEntry.getType(), 
                                            indexEntry.getKey(), 
                                            IndexEntry.MODE_PERSISTING);

                rmsAdapter.addIndexEntry(indexEntry);
            } 
        } 

        return;
    } 

    private void removeCachedTheaterSchedule() 
            throws ModelException, ApplicationException {
        IndexEntry indexEntry = rmsAdapter.getIndexEntry(null, 
                IndexEntry.TYPE_THEATER, IndexEntry.MODE_CACHING);

        if (indexEntry != null) {
            String theaterKey = indexEntry.getKey();

            rmsAdapter.deleteTheater(indexEntry.getRecordId());
            rmsAdapter.deleteIndexEntry(indexEntry.getKey(), 
                                        indexEntry.getType());

            indexEntry = 
                rmsAdapter.getIndexEntry(theaterKey, 
                                         IndexEntry.TYPE_THEATER_SCHEDULE, 
                                         IndexEntry.MODE_CACHING);

            if (indexEntry != null) {
                TheaterSchedule theaterSchedule = 
                    rmsAdapter.loadTheaterSchedule(indexEntry.getRecordId());

                rmsAdapter.deleteTheaterSchedule(indexEntry.getRecordId());
                rmsAdapter.deleteIndexEntry(indexEntry.getKey(), 
                                            indexEntry.getType());
                // Movies are to be removed (garbage collected) by an independent thread
            } 
        } 

        return;
    } 

}

