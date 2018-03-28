/******************************************************************************
 * Mail4ME - Mail for the Java 2 Micro Edition
 *
 * A lightweight, J2ME- (and also J2SE-) compatible package for sending and
 * receiving Internet mail messages using the SMTP and POP3 protocols.
 *
 * Copyright (c) 2000-2002 Jörg Pleumann <joerg@pleumann.de>
 *
 * Mail4ME is part of the EnhydraME family of projects. See the following web
 * sites for more information:
 *
 * -> http://mail4me.enhydra.org
 * -> http://me.enhydra.org
 *
 * Mail4ME is distributed under the Enhydra Public License (EPL), which is
 * discussed in great detail here:
 *
 * -> http://www.enhydra.org/software/license/index.html
 *
 * Have fun!
 ******************************************************************************/
package de.trantor.mail;

import java.util.Vector;

/**
 * Represents an envelope used for sending a message via the SMTP protocol.
 * The envelope holds information about a message's sender and recipients
 * as used during an SMTP session. It serves more or less the same
 * purpose that an envelope is used for when sending snail mail. Note that
 * there is a difference between the message envelope and the message header.
 * <p>
 * Most of the time the envelope data will be identical to the "From:", "To:",
 * "CC:" and "BCC:" fields of the message header. There are cases, though,
 * where one might want to send a message to a recipient different from the
 * one originally specified in the message. A mailing list is an example of
 * an application that makes use of this feature: Postings are forwarded to
 * the subscribers *without* changing the original message. This is one reason
 * for separating the envelope from the message itself.
 * <p>
 * Another reason for separating the message from its envelope is memory
 * savings: The envelope information in only needed while sending a message via
 * SMTP. It is not required for receiving messages via POP3 or for holding
 * messages in-memory.
 *
 * @see Message
 * @see SmtpClient
 */
public class Envelope {
    
    /**
     * Holds the message that is contained in this envelope.
     */
    private Message message;
    
    /**
     * Holds the envelope's sender field, that is, the data used for the
     * "MAIL ... FROM: ..." command during an SMTP session.
     */
    private String sender;
    
    /**
     * Holds the envelope's list of recipients, that is, the data used for the
     * "RCPT ... TO: ..." command during an SMTP session.
     */
    private Vector recipients;
    
    /**
     * Creates a new envelope for a given message. The value of the "autofill"
     * parameter determines whether sender and recipient data is automatically
     * copied from the message's "From:", "To:", "CC:" and "BCC:" fields.
     */
    public Envelope(Message message, boolean autofill) {
        this.message = message;
        recipients = new Vector();
        
        if (autofill) {
            sender = message.getHeaderValue("From");
            
            String[] toFields = message.getAllHeaderValues("To");
            for (int i = 0; i < toFields.length; i++) {
                recipients.addElement(toFields[i]);
            }
            
            String[] ccFields = message.getAllHeaderValues("CC");
            for (int i = 0; i < ccFields.length; i++) {
                recipients.addElement(ccFields[i]);
            }
            
            String[] bccFields = message.getAllHeaderValues("Bcc");
            for (int i = 0; i < bccFields.length; i++) {
                recipients.addElement(bccFields[i]);
            }
        }
    }
    
    /**
     * Returns the message contained in this envelope.
     */
    public Message getMessage() {
        return message;
    }
    
    /**
     * Sets the envelope's sender field to the given value. Note that SMTP
     * requires a message to have a (valid) sender. Otherwise the message
     * can't be delivered.
     *
     * @see #getSender
     * @see #addRecipient
     * @see #getRecipientCount
     * @see #getRecipient
     * @see #setRecipient
     * @see #removeRecipient
     */
    public void setSender(String address) {
        sender = address;
    }
    
    /**
     * Returns the envelope's sender field. This will be equal to the message's
     * "From:" field most of the time, but this is not a necessity.
     *
     * @see #setSender
     * @see #addRecipient
     * @see #getRecipientCount
     * @see #getRecipient
     * @see #setRecipient
     * @see #removeRecipient
     */
    public String getSender() {
        return sender;
    }
    
    /**
     * Adds a recipient to the message's envelope. The method returns the
     * index of the new recipient, starting from zero. Note that SMTP requires
     * a message to have a least one (valid) recipient. Otherwise the message
     * can't be delivered.
     *
     * @see #getSender
     * @see #setSender
     * @see #getRecipientCount
     * @see #getRecipient
     * @see #setRecipient
     * @see #removeRecipient
     */
    public int addRecipient(String address) {
        recipients.addElement(address);
        return recipients.size() - 1;
    }
    
    /**
     * Replaces the envelope's recipient at the given index by a new one.
     *
     * @see #getSender
     * @see #setSender
     * @see #addRecipient
     * @see #getRecipientCount
     * @see #getRecipient
     * @see #removeRecipient
     */
    public void setRecipient(int index, String address) throws ArrayIndexOutOfBoundsException {
        recipients.setElementAt(address, index);
    }
    
    /**
     * Returns the number of recipients specified in this envelope.
     *
     * @see #getSender
     * @see #setSender
     * @see #addRecipient
     * @see #getRecipient
     * @see #setRecipient
     * @see #removeRecipient
     */
    public int getRecipientCount() {
        return recipients.size();
    }
    
    /**
     * Returns one of the envelope's recipients by index. The first recipient
     * has index zero.
     *
     * @see #getSender
     * @see #setSender
     * @see #addRecipient
     * @see #getRecipientCount
     * @see #setRecipient
     * @see #removeRecipient
     */
    public String getRecipient(int index) throws ArrayIndexOutOfBoundsException {
        return (String)recipients.elementAt(index);
    }
    
    /**
     * Removes a recipient from the message's envelope.
     *
     * @see #getSender
     * @see #setSender
     * @see #addRecipient
     * @see #getRecipientCount
     * @see #getRecipient
     * @see #setRecipient
     */
    public void removeRecipient(int index) throws ArrayIndexOutOfBoundsException {
        recipients.removeElementAt(index);
    }
}