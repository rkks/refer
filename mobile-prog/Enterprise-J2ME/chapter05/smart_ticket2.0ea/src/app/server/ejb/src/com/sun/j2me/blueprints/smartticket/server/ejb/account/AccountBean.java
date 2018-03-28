

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
 * $Id: AccountBean.java,v 1.5 2003/03/24 22:43:10 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.account;

import javax.ejb.*;
import javax.naming.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitLocalHome;
import com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingLocal;

/**
 * @createTableDeploy
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal}>
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome}>
 * @hasSimplePK
 * @deleteTableUndeploy
 * @ejbAbstractSchemaName Account
 * @ejbJNDIname ejb/smartticket/smartticket/Account
 * @displayName AccountEB
 * @ejbDontSynchronizeNames
 * @ejbLocalJNDIname ejb/smartticket/smartticket/Account
 * @undefined
 * @ejbTransactionAttribute Required
 */
abstract public class AccountBean implements EntityBean {
    private EntityContext ctx;

    /**
     * @EJB-REF
     * @ReferenceType Entity
     * @ReferenceRemoteType EJB-LOCAL-REF
     * @RemoteInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitLocal
     * @EJBLink SyncConduitBean
     * @REF_JNDI_NAME ejb/smartticket/smartticket/SyncConduit
     * @HomeInterface com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitLocalHome
     * @undefined
     */
    private static final String EJB_REF_SYNC_CONDUIT = "ejb/SyncConduit";

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountBean#getMovieRatings#(##)#:oiref>
     * @relation_destination <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingBean#getAccount#(##)#:oiref>
     */
    /* #MovieRatingBean lnkMovieRatingBean; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountBean#getReservations#(##)#:oiref>
     * @relation_destination <oiref:java#Class#com.sun.j2me.blueprints.smartticket.server.ejb.reservation.ReservationLocal:oiref>
     */
    /* #ReservationBean lnkReservationBean; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountBean#getMovieRatingSyncConduit#(##)#:oiref>
     * @relation_destination <oiref:java#Class#com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitLocal:oiref>
     */
    /* #SyncConduitBean lnkSyncConduitBean; */

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountBean#getMovieRatingSyncConduit#(##)#:oiref>
     * @relation_destination <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitBean#getAccount#(##)#:oiref>
     */
    /* #com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitBean lnkSyncConduitBean1; */
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

    /**
     * @primaryKey
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getUserName();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setUserName(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getPassword();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setPassword(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getLastName();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setLastName(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getFirstName();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setFirstName(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getEMail();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setEMail(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getZipCode();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setZipCode(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getCreditCardNumber();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setCreditCardNumber(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getCreditCardHolderName();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setCreditCardHolderName(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getCreditCardExpirationDate();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setCreditCardExpirationDate(String param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract String getCreditCardType();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setCreditCardType(String param);

    /**
     * @finder-query SELECT DISTINCT OBJECT(o) FROM MovieRating o WHERE o.account.userName = ?1 and o.id = ?2
     */
    public abstract MovieRatingLocal ejbSelectMovieRating(String userName, 
            String id) throws FinderException;

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.reservation.ReservationLocal
     * @Relationship-Unidirectional
     * @CMP-RelationshipEJBName-UNIDIR ReservationBean
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipMultiplicity-UNIDIR many
     * @CMP-RelationshipRoleName Account to Reservation
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL AccountBean
     * @relation_name Account-Reservation
     * @ejbTransactionAttribute Required
     */
    public abstract Collection getReservations();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setReservations(Collection param);

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitBean.account
     * @CMP-RelationshipEJBName-UNIDIR SyncConduitBean
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipRoleName Account to SyncConduit
     * @CMP-RelationshipRoleName-UNIDIR UNIDIRECTIONAL AccountBean
     * @relation_name Account-SyncConduit
     * @ejbTransactionAttribute Required
     */
    public abstract SyncConduitLocal getMovieRatingSyncConduit();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setMovieRatingSyncConduit(SyncConduitLocal param);
    public String ejbCreate(String userName, 
                            String password) throws CreateException {
        setUserName(userName);
        setPassword(password);

        return null;
    } 

    public void ejbPostCreate(String userName, 
                              String password) throws CreateException {
        try {
            Context context = 
                (Context) new InitialContext().lookup("java:comp/env");
            SyncConduitLocalHome syncConduitHome = 
                (SyncConduitLocalHome) context.lookup(EJB_REF_SYNC_CONDUIT);
            SyncConduitLocal syncConduit = syncConduitHome.create();

            setMovieRatingSyncConduit(syncConduit);
        } catch (NamingException ne) {
            throw new CreateException("AccountBean.ejbPostCreate(): " 
                                      + ne.getMessage());
        } 
    } 

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.rating.MovieRatingBean.account
     * @CMP-RelationshipMultiplicity one
     * @CMP-RelationshipRoleName Account to MovieRating
     * @relation_name Account-MovieRating
     * @ejbTransactionAttribute Required
     */
    public abstract Collection getMovieRatings();

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setMovieRatings(Collection param);

    /**
     * @ejbTransactionAttribute Required
     */
    public MovieRatingLocal getMovieRating(String id) {
        try {
            return ejbSelectMovieRating(getUserName(), id);
        } catch (FinderException fe) {
            return null;
        } 
    } 

}

