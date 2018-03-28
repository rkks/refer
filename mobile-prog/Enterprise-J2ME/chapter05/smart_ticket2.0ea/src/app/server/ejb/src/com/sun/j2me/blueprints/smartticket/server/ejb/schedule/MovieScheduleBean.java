

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
 * $Id: MovieScheduleBean.java,v 1.3 2003/03/24 22:43:10 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.schedule;

import javax.ejb.*;
import javax.naming.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.show.*;

/**
 * @createTableDeploy
 * @ejbTransactionAttribute Required
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocalHome}>
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleLocal}>
 * @ejbDontSynchronizeNames
 * @ejbPrimaryKey <{com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieSchedulePK}>
 * @ejbJNDIname ejb/smartticket/smartticket/MovieSchedule
 * @ejbLocalJNDIname ejb/smartticket/smartticket/MovieSchedule
 * @ejbAbstractSchemaName MovieSchedule
 * @displayName MovieScheduleEB
 * @deleteTableUndeploy
 * @undefined
 */
abstract public class MovieScheduleBean implements EntityBean {
    private EntityContext ctx;

    /**
     * @link dependency
     */
    /* # ShowTime lnkShowTime; */

    /**
     * @link dependency
     */
    /* # ShowTimePattern lnkShowTimePattern; */

    /**
     * @link dependency
     */
    /* # Seating lnkSeatingPlan; */

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocal
     * @EJBLink MovieShowBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/MovieShow
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowLocalHome
     * @undefined
     */
    private static final String EJB_REF_MOVIE_SHOW = "ejb/MovieShow";

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleBean#getShows#(##)#:oiref>
     * @relation_destination <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowBean#getSchedule#(##)#:oiref>
     */
    /* #MovieShowBean lnkMovieShowBean; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.schedule.MovieScheduleBean#getMovie#(##)#:oiref>
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

    public MovieSchedulePK ejbCreate(String theaterId, MovieLocal movie) 
            throws CreateException {
        setTheaterId(theaterId);
        setMovieId(movie.getId());

        return null;
    } 

    public void ejbPostCreate(String theaterId, 
                              MovieLocal movie) throws CreateException {
        setMovie(movie);

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
     * @multiplicity_type <{Collection}>
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.schedule.TheaterScheduleBean.movieSchedules
     * @CMP-RelationshipMultiplicity many
     * @CMP-RelationshipRoleName MovieSchedule to TheaterSchedule
     * @relation_name TheaterSchedule-MovieSchedule
     */
    public abstract TheaterScheduleLocal getTheaterSchedule();
    public abstract void setTheaterSchedule(TheaterScheduleLocal param);

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.show.MovieShowBean.schedule
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipRoleName MovieSchedule to MovieShow
     * @relation_name MovieSchedule-MovieShow
     */
    public abstract Collection getShows();
    public abstract void setShows(Collection param);

    /**
     * @multiplicity_type <{Collection}>
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal
     * @Relationship-Unidirectional
     * @CMP-RelationshipEJBName-UNIDIR MovieBean
     * @CMP-RelationshipMultiplicity many
     * @CMP-RelationshipMultiplicity-UNIDIR one
     * @CMP-RelationshipRoleName MovieSchedule to Movie
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL MovieScheduleBean
     * @relation_name MovieSchedule-Movie
     */
    public abstract MovieLocal getMovie();
    public abstract void setMovie(MovieLocal param);

    /**
     * @undefined
     */
    public abstract ShowTimePattern[] getShowTimePatterns();
    public abstract void setShowTimePatterns(ShowTimePattern[] param);

    /**
     * @undefined
     */
    public abstract Seating getSeating();
    public abstract void setSeating(Seating param);
    public MovieShowLocal getShow(ShowTime time) {
        // Should first check if the given showTime matches the pattern
        try {
            Context context = 
                (Context) new InitialContext().lookup("java:comp/env");
            MovieShowLocalHome movieShowHome = 
                (MovieShowLocalHome) context.lookup(EJB_REF_MOVIE_SHOW);
            MovieShowLocal movieShow = null;

            try {
                movieShow = 
                    movieShowHome.findByPrimaryKey(new MovieShowPK(getTheaterSchedule().getTheater().getId(), 
                        getMovie().getId(), time.toDate()));
            } catch (FinderException fe) {
                movieShow = 
                    movieShowHome.create(getTheaterSchedule().getTheater().getId(), 
                                         getMovie(), time.toDate());

                movieShow.setSeating(getSeating());
                getShows().add(movieShow);
            } 

            return movieShow;
        } catch (Exception e) {
            throw new EJBException("MovieScheduleBean.getShow(): " 
                                   + e.getMessage());
        } 
    } 

}

