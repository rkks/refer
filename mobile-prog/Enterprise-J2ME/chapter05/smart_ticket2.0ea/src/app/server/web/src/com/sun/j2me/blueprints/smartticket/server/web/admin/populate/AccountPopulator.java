

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
 * $Id: AccountPopulator.java,v 1.4 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.admin.populate;

import javax.ejb.*;
import javax.naming.*;
import javax.xml.parsers.*;
import java.util.*;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocal;
import com.sun.j2me.blueprints.smartticket.server.ejb.account.AccountLocalHome;
import org.xml.sax.XMLFilter;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.*;

public class AccountPopulator {
    private static final String EJB_REF_ACCOUNT = "ejb/Account";
    private static final String XML_ACCOUNTS = "Accounts";
    private static final String XML_ACCOUNT = "Account";
    private static final String XML_USER_NAME = "Account/@userName";
    private static final String XML_PASSWORD = "Password";
    private static final String XML_FIRST_NAME = "FirstName";
    private static final String XML_LAST_NAME = "LastName";
    private static final String XML_EMAIL = "EMail";
    private static final String XML_ZIP_CODE = "ZipCode";
    private static final String XML_CREDIT_CARD_NUMBER = "CreditCardNumber";
    private static final String XML_CREDIT_CARD_HOLDER_NAME = 
        "CreditCardHolderName";
    private static final String XML_CREDIT_CARD_EXPIRATION_DATE = 
        "CreditCardExpirationDate";
    private static final String XML_CREDIT_CARD_TYPE = "CreditCardType";
    private String rootTag;
    private AccountLocalHome accountHome = null;
    private AccountLocal account;

    public AccountPopulator() throws PopulateException {
        this(XML_ACCOUNTS);

        return;
    }

    public AccountPopulator(String rootTag) {
        this.rootTag = rootTag;

        return;
    }

    public XMLFilter setup(XMLReader reader) throws PopulateException {
        return new XMLDBHandler(reader, rootTag, XML_ACCOUNT) {
            public void update() throws PopulateException {}

            public void create() throws PopulateException {
                account = 
                    createAccount(getValue(XML_USER_NAME), 
                                  getValue(XML_PASSWORD), 
                                  getValue(XML_FIRST_NAME), 
                                  getValue(XML_LAST_NAME), 
                                  getValue(XML_EMAIL), 
                                  getValue(XML_ZIP_CODE), 
                                  getValue(XML_CREDIT_CARD_NUMBER), 
                                  getValue(XML_CREDIT_CARD_HOLDER_NAME), 
                                  getValue(XML_CREDIT_CARD_EXPIRATION_DATE), 
                                  getValue(XML_CREDIT_CARD_TYPE));

                return;
            } 

        };
    } 

    public boolean check() throws PopulateException {
        try {
            if (accountHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                accountHome = 
                    (AccountLocalHome) context.lookup(EJB_REF_ACCOUNT);
            } 

            Collection accounts = accountHome.findAllAccounts();

            if ((accounts == null) || (accounts.size() == 0)) {
                return false;
            } 
        } catch (Exception exception) {
            return false;
        } 

        return true;
    } 

    public void clear() throws PopulateException {
        try {
            if (accountHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                accountHome = 
                    (AccountLocalHome) context.lookup(EJB_REF_ACCOUNT);
            } 

            Collection accounts = accountHome.findAllAccounts();

            for (Iterator i = accounts.iterator(); i.hasNext(); ) {
                ((AccountLocal) i.next()).remove();
            } 
        } catch (Exception exception) {
            throw new PopulateException("Could not clear: " 
                                        + exception.getMessage(), exception);
        } 

        return;
    } 

    private AccountLocal createAccount(String userName, String password, 
                                       String firstName, String lastName, 
                                       String eMail, String zipCode, 
                                       String creditCardNumber, 
                                       String creditCardHolderName, 
                                       String creditCardExpirationDate, 
                                       String creditCardType) throws PopulateException {
        try {
            if (accountHome == null) {
                Context context = 
                    (Context) new InitialContext().lookup("java:comp/env");

                accountHome = 
                    (AccountLocalHome) context.lookup(EJB_REF_ACCOUNT);
            } 

            AccountLocal account = accountHome.create(userName, password);

            account.setFirstName(firstName);
            account.setLastName(lastName);
            account.setEMail(eMail);
            account.setZipCode(zipCode);
            account.setCreditCardNumber(creditCardNumber);
            account.setCreditCardHolderName(creditCardHolderName);
            account.setCreditCardExpirationDate(creditCardExpirationDate);
            account.setCreditCardType(creditCardType);

            return account;
        } catch (Exception exception) {
            throw new PopulateException("Could not create: " 
                                        + exception.getMessage(), exception);
        } 
    } 

    public AccountLocal getAccount() {
        return account;
    } 

}

