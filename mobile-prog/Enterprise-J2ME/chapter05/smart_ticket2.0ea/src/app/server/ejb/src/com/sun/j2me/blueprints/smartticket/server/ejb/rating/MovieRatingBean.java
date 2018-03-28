

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
 * $Id: MovieRatingBean.java,v 1.6 2003/03/24 22:43:10 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.rating;

import javax.ejb.*;
import javax.naming.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocalHome;

/**
 * @createTableDeploy
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal}>
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocalHome}>
 * @ejbTransactionAttribute Required
 * @displayName MovieRatingEB
 * @ejbAbstractSchemaName MovieRating
 * @ejbJNDIname ejb/smartticket/smartticket/MovieRating
 * @ejbLocalJNDIname ejb/smartticket/smartticket/MovieRating
 * @ejbDontSynchronizeNames
 * @undefined
 * @deleteTableUndeploy
 */
abstract public class MovieRatingBean implements EntityBean {
    private EntityContext ctx;

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocal
     * @EJBLink SyncMetaDataBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/SyncMetaData
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocalHome
     * @undefined
     */
    private static final String EJB_REF_SYNC_META_DATA = "ejb/SyncMetaData";
    private boolean modified = false;

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingBean#getRatedMovie#(##)#:oiref>
     * @relation_destination <oiref:java#Class#com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal:oiref>
     */
    /* #MovieBean lnkMovieBean; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingBean#getSyncMetaData#(##)#:oiref>
     * @relation_destination <oiref:java#Class#com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocal:oiref>
     */
    /* #SyncMetaDataBean lnkSyncMetaDataBean; */
    public void setEntityContext(EntityContext context) throws EJBException {
        ctx = context;
    } 

    public void unsetEntityContext() throws EJBException {
        ctx = null;
    } 

    public void ejbActivate() throws EJBException {
        modified = false;

        return;
    } 

    public void ejbPassivate() throws EJBException {
        modified = false;

        return;
    } 

    public void ejbRemove() throws EJBException {
        getSyncMetaData().markAsDeleted();
    } 

    public void ejbStore() throws EJBException {
        if (modified) {
            getSyncMetaData().markAsModified();

            modified = false;
        } 

        return;
    } 

    public void ejbLoad() throws EJBException {}

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getId();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setId(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract int getStarNumber();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setStarNumber(int param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract long getLastViewingDate();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setLastViewingDate(long param);

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.movie.MovieLocal
     * @Relationship-Unidirectional
     * @CMP-RelationshipEJBName-UNIDIR MovieBean
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipMultiplicity-UNIDIR one
     * @CMP-RelationshipRoleName MovieRating to Movie
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL MovieRatingBean
     * @relation_name MovieRating-Movie
     * @ejbTransactionAttribute Required
     */
    public abstract MovieLocal getRatedMovie();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setRatedMovie(MovieLocal param);

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncMetaDataLocal
     * @Relationship-Unidirectional
     * @CMP-RelationshipEJBName-UNIDIR SyncMetaDataBean
     * @CMP-RelationshipMultiplicity one
     * @Cmp-RelationshipMultiplicity-UNIDIR one
     * @CMP-RelationshipRoleName MovieRating to SyncMetaData
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL MovieRatingBean
     * @relation_name MovieRating-SyncMetaData
     * @ejbTransactionAttribute Required
     */
    public abstract SyncMetaDataLocal getSyncMetaData();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setSyncMetaData(SyncMetaDataLocal param);
    public Object ejbCreate(MovieLocal movie, 
                            AccountLocal account) throws CreateException {
        setId(movie.getId());
        setStarNumber(MovieRatingConstants.RATING_UNRATED);

        return null;
    } 

    public void ejbPostCreate(MovieLocal movie, 
                              AccountLocal account) throws CreateException {
        setRatedMovie(movie);
        setAccount(account);

        try {
            Context context = 
                (Context) new InitialContext().lookup("java:comp/env");
            SyncMetaDataLocalHome syncMetaDataHome = 
                (SyncMetaDataLocalHome) context.lookup(EJB_REF_SYNC_META_DATA);
            SyncMetaDataLocal syncMetaData = 
                syncMetaDataHome.create(movie.getId());

            account.getMovieRatingSyncConduit().getSyncMetaDataSet().add(syncMetaData);
            setSyncMetaData(syncMetaData);
        } catch (NamingException ne) {
            throw new CreateException("MovieRatingBean.ejbPostCreate(): " 
                                      + ne.getMessage());
        } 
    } 

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountBean.movieRatings
     * @CMP-RelationshipMultiplicity many
     * @CMP-RelationshipRoleName MovieRating to Account
     * @relation_name Account-MovieRating
     * @ejbTransactionAttribute Required
     */
    public abstract AccountLocal getAccount();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setAccount(AccountLocal param);

    /**
     * @ejbTransactionAttribute Required
     */
    public void setData(MovieRatingData data, long modificationTime) {
        setStarNumber(data.getStarNumber());
        setLastViewingDate(data.getLastViewingDate());
        getSyncMetaData().markAsModified(modificationTime);

        return;
    } 

    /**
     * @ejbTransactionAttribute Required
     */
    public MovieRatingData getData() {
        return new MovieRatingData(getId(), getStarNumber(), 
                                   getLastViewingDate(), 
                                   getSyncMetaData().getLastModificationTime());
    } 

    /**
     * @ejbTransactionAttribute Required
     */
    public void changeStarNumber(int starNumber) {
        setStarNumber(starNumber);

        modified = true;

        return;
    } 

    /**
     * @ejbTransactionAttribute Required
     */
    public void changeLastViewingDate(long lastViewingDate) {
        setLastViewingDate(lastViewingDate);

        modified = true;

        return;
    } 

}

