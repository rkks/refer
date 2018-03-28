

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
 * $Id: MovieShowBean.java,v 1.4 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.show;

import javax.ejb.*;
import javax.naming.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowPK;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.Seat;

/**
 * @createTableDeploy
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocal}>
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocalHome}>
 * @ejbPrimaryKey <{com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowPK}>
 * @ejbDontSynchronizeNames
 * @ejbJNDIname ejb/smartticket/smartticket/MovieShow
 * @ejbLocalJNDIname ejb/smartticket/smartticket/MovieShow
 * @ejbAbstractSchemaName MovieShow
 * @displayName MovieShowEB
 * @deleteTableUndeploy
 * @undefined
 * @ejbTransactionAttribute Required
 */
abstract public class MovieShowBean implements EntityBean {
    private EntityContext ctx;

    /**
     * @link dependency
     */
    /* # Seating lnkSeatingPlan; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowBean#getPlayedMovie#(##)#:oiref>
     * @relation_destination <oiref:java#Class#com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal:oiref>
     */
    /* #MovieBean lnkMovieBean; */
    public void setEntityContext(EntityContext context) throws EJBException {
        ctx = context;
    } 

    public void unsetEntityContext() throws EJBException {
        ctx = null;
    } 

    public void ejbActivate() throws EJBException {}

    public void ejbPassivate() throws EJBException {}

    public void ejbRemove() throws EJBException {}

    public void ejbStore() throws EJBException {}

    public void ejbLoad() throws EJBException {}

    public MovieShowPK ejbCreate(String theaterId, MovieLocal movie, 
                                 Date time) throws CreateException {
        setTheaterId(theaterId);
        setMovieId(movie.getId());
        setTime(time);

        return null;
    } 

    public void ejbPostCreate(String theaterId, MovieLocal movie, 
                              Date time) throws CreateException {
        setPlayedMovie(movie);

        return;
    } 

    /**
     * @undefined
     */
    public abstract String getTheaterId();
    public abstract void setTheaterId(String param);

    /**
     * @undefined
     */
    public abstract String getMovieId();
    public abstract void setMovieId(String param);

    /**
     * @undefined
     */
    public abstract Date getTime();
    public abstract void setTime(Date param);

    /**
     * @multiplicity_type <{Collection}>
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleBean.shows
     * @CMP-RelationshipMultiplicity many
     * @CMP-RelationshipRoleName MovieShow  to MovieSchedule
     * @relation_name MovieSchedule-MovieShow
     */
    public abstract MovieScheduleLocal getSchedule();
    public abstract void setSchedule(MovieScheduleLocal param);

    /**
     * @undefined
     */
    public abstract Seating getSeating();
    public abstract void setSeating(Seating param);
    public boolean bookSeats(Seat[] seats) {
        Seating seating = getSeating();
        boolean booked = seating.bookSeats(seats);

        if (booked) {
            setSeating(seating);
        } 

        return booked;
    } 

    public void unbookSeats(Seat[] seats) {
        Seating seating = getSeating();
        boolean unbooked = seating.unbookSeats(seats);

        if (unbooked) {
            setSeating(seating);
        } 

        return;
    } 

    public void confirmBookedSeats(Seat[] seats) {
        Seating seating = getSeating();
        boolean confirmed = seating.confirmBookedSeats(seats);

        if (confirmed) {
            setSeating(seating);
        } 

        return;
    } 

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal
     * @Relationship-Unidirectional
     * @CMP-RelationshipEJBName-UNIDIR MovieBean
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipMultiplicity-UNIDIR one
     * @CMP-RelationshipRoleName MovieShow  to Movie
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL MovieShowBean
     * @relation_name MovieShow-Movie
     */
    public abstract MovieLocal getPlayedMovie();
    public abstract void setPlayedMovie(MovieLocal param);
}

