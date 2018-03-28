

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
 * $Id: SyncClientBean.java,v 1.2 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing;

import javax.ejb.*;
import java.util.*;
import java.util.Collection;

/**
 * @createTableDeploy
 * @ejbLocal <{com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncClientLocal}>
 * @ejbLocalHome <{com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncClientLocalHome}>
 * @ejbTransactionAttribute Required
 * @ejbPrimaryKey <{Object}>
 * @ejbJNDIname ejb/smartticket/smartticket/SyncClient
 * @ejbAbstractSchemaName SyncClient
 * @displayName SyncClientEB
 * @ejbDontSynchronizeNames
 * @undefined
 * @deleteTableUndeploy
 * @hasSimplePK
 */
abstract public class SyncClientBean implements EntityBean {
    private EntityContext ctx;

    /**
     * @shapeType EJBRelationshipLink
     * @relation_source <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncClientBean#getSyncConduit#(##)#:oiref>
     * @relation_destination <oiref:java#Member#com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitBean#getSyncClients#(##)#:oiref>
     */
    /* #SyncConduitBean lnkSyncConduitBean; */
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
     * @undefined
     */
    public abstract int getId();
    public abstract void setId(int param);

    /**
     * @undefined
     * @ejbTransactionAttribute Required
     */
    public abstract long getLastSynchronizationTime() throws EJBException;

    /**
     * @ejbTransactionAttribute Required
     */
    public abstract void setLastSynchronizationTime(long param) 
            throws EJBException;
    public Object ejbCreate(int id) throws CreateException {
        setId(id);

        return null;
    } 

    public void ejbPostCreate(int id) throws CreateException {}

    /**
     * @multiplicity_type Collection
     * @CMP-Relationship
     * @CMP-Relationship-Destination com.sun.j2me.blueprints.smartticket.server.ejb.synchronizing.SyncConduitBean.syncClients
     * @CMP-RelationshipMultiplicity many
     * @CMP-RelationshipRoleName SyncClient to SyncConduit
     * @relation_name SyncConduit-SyncClient
     */
    public abstract SyncConduitLocal getSyncConduit();
    public abstract void setSyncConduit(SyncConduitLocal param);
}

