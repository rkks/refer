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
package de.trantor.mail.demo.j2me;

import java.io.IOException;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Vector;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.rms.*;
import de.trantor.mail.*;

/**
 * Provides a small MIDlet for doing e-mail on an MIDP-enabled device, like
 * a PDA or a cell phone. Currently not very well documented due to lack of
 * time.
 */
public class MailMIDlet extends MIDlet implements CommandListener {

    private Display display = Display.getDisplay(this);

    private SetupScreen setupScreen = new SetupScreen(this);
    private InboxScreen inboxScreen = new InboxScreen(this);
    private ReadScreen readScreen = new ReadScreen(this);
    private WriteScreen writeScreen = new WriteScreen(this);
    private InfoScreen infoScreen;

  /*
  private String address = "yourname@yourisp.com";
  private String hostname = "yourhost.yourisp.com";
  private String pop3Host = "pop.yourisp.com";
  private String pop3User = "your-username";
  private String pop3Pass = "your-password";
  private String smtpHost = "smtp.yourisp.com";
   */

    private String address = "yourname@yourisp.com";
    private String hostname = "localhost";
    private String pop3Host = "pop.yourisp.com";
    private String pop3User = "username";
    private String pop3Pass = "password";
    private String smtpHost = "smtp.yourisp.com";
    private boolean smtpAuth = false;
    
    private boolean httpProxy = false;
    private String httpHost = "localhost";

    private boolean debug = false;
    private boolean imap = false;

    private InboxClient pop3Client;

    private Vector msgNumbers = new Vector();

    public MailMIDlet() {
        try {
            loadSetupData();
        }
        catch (Exception e) {
            e.printStackTrace();
        }

        setupScreen.setAddress(address);
        setupScreen.setHostname(hostname);
        setupScreen.setPop3Host(pop3Host);
        setupScreen.setPop3User(pop3User);
        setupScreen.setPop3Pass(pop3Pass);
        setupScreen.setSmtpHost(smtpHost);
        setupScreen.setSmtpAuth(smtpAuth);
        setupScreen.setHttpProxy(httpProxy);
        setupScreen.setHttpHost(httpHost);
        setupScreen.setDebug(debug);
        setupScreen.setImap(imap);
    }

    public void commandAction(Command cmd, Displayable dsp) {
        if (debug) System.out.println("Command: " + cmd.getLabel());

        try {
            if (cmd == SetupScreen.OK) {

                address  = setupScreen.getAddress();
                hostname = setupScreen.getHostname();
                pop3Host = setupScreen.getPop3Host();
                pop3User = setupScreen.getPop3User();
                pop3Pass = setupScreen.getPop3Pass();
                smtpHost = setupScreen.getSmtpHost();
                smtpAuth = setupScreen.getSmtpAuth();
				httpProxy = setupScreen.getHttpProxy();
				httpHost = setupScreen.getHttpHost();
                debug = setupScreen.getDebug();
                imap = setupScreen.getImap();

                try {
                    saveSetupData();
                }
                catch (Exception e) {
                    e.printStackTrace();
                }

                if (imap) {
	                if (httpProxy) {
	                     pop3Client = new ImapClient(new de.trantor.mail.http.ConnectionImpl(httpHost, 8080));
	                }
	                else {
                        pop3Client = new ImapClient();
                    }
                }
                else {
                    if (httpProxy) {
                        pop3Client = new Pop3Client(new de.trantor.mail.http.ConnectionImpl(httpHost, 8080));
                    }
                    else {
                        pop3Client = new Pop3Client();
                    }
                }

                pop3Client.setDebug(debug);
                pop3Client.open(pop3Host, 0, false, pop3User, pop3Pass);

                long t0 = System.currentTimeMillis();
                getMessageList();
                long t1 = System.currentTimeMillis();

                if (debug) inboxScreen.setTitle("Time: " + (t1 - t0) + " ms");
                display.setCurrent(inboxScreen);
            }
            else if (cmd == InboxScreen.READ) {
                int num = inboxScreen.getMessageIndex();

                if (num == -1) return;

                long t0 = System.currentTimeMillis();
                Message message = pop3Client.getMessage(((Integer)msgNumbers.elementAt(num)).intValue());
                long t1 = System.currentTimeMillis();

                readScreen.setSender(Message.getMachineAddress(message.getHeaderValue("From", "No sender")));
                readScreen.setDate(message.getHeaderValue("Date", "No date"));
                readScreen.setSubject(message.getHeaderValue("Subject", "No subject"));

                readScreen.clearBody();

                MimeDecoder mime = new MimeDecoder(message);
                addPartToScreen(mime);

                if (debug) readScreen.setTitle("Time: " + (t1 - t0) + " ms");
                display.setCurrent(readScreen);
            }
            else if (cmd == InboxScreen.WRITE) {
                writeScreen.clear();

                display.setCurrent(writeScreen);
            }
            else if (cmd == InboxScreen.DELETE) {
                int num = inboxScreen.getMessageIndex();

                if (num == -1) return;

                pop3Client.removeMessage(((Integer)msgNumbers.elementAt(num)).intValue());
                inboxScreen.delete(inboxScreen.getMessageIndex());
                msgNumbers.removeElementAt(num);

                display.setCurrent(inboxScreen);
            }
            else if (cmd == InboxScreen.INFO) {
                infoScreen = new InfoScreen(this);
                display.setCurrent(infoScreen);
            }
            else if ((cmd == SetupScreen.EXIT) || (cmd == InboxScreen.EXIT) ){
                destroyApp(false);
                notifyDestroyed();
            }
            else if (cmd == InfoScreen.OK) {
                display.setCurrent(inboxScreen);
                infoScreen = null;
            }
            else if (cmd == ReadScreen.OK) {
                display.setCurrent(inboxScreen);
            }
            else if (cmd == WriteScreen.OK) {
                Message message = new Message(address, writeScreen.getRecipient(), writeScreen.getSubject());
                message.addBodyLine(writeScreen.getBody());

                SmtpClient smtpClient = null;
                if (httpProxy) {
                     smtpClient = new SmtpClient(new de.trantor.mail.http.ConnectionImpl(httpHost, 8080), hostname);
                }
                else {
                    smtpClient = new SmtpClient(hostname);
                }

                try {
                    smtpClient.setDebug(debug);

                    if (smtpAuth) {
                        smtpClient.open(smtpHost, 0, false, pop3User, pop3Pass);
                    }
                    else {
                        smtpClient.open(smtpHost);
                    }

                    smtpClient.sendMessage(message);
                }
                finally {
                    smtpClient.close();
                }

                display.setCurrent(inboxScreen);
            }
            else if (cmd == WriteScreen.CANCEL) {
                display.setCurrent(inboxScreen);
            }
        }
        catch (Exception error) {
            Alert alert = new Alert("Error");
            alert.setString(error.getClass().getName() + ": " + error.getMessage());
            error.printStackTrace();
            display.setCurrent(alert, setupScreen);
        }
    }

    protected void startApp() throws MIDletStateChangeException {
        display.setCurrent(setupScreen);
    }

    protected void destroyApp(boolean unconditional) {
        try {
            pop3Client.close();
        }
        catch (Exception error) {
        }
    }

    protected void pauseApp() {
        try {
        }
        catch (Exception error) {
        }
    }

    /**
     * Saves the setup data to RMS.
     */
    private void saveSetupData() throws RecordStoreException, IOException {
        try {
            RecordStore.deleteRecordStore("mail4me");
        }
        catch (RecordStoreException e) {
        }

        RecordStore store = RecordStore.openRecordStore("mail4me", true);

        try {
            ByteArrayOutputStream buffer = new ByteArrayOutputStream();
            DataOutputStream output = new DataOutputStream(buffer);

            output.writeUTF(address);
            output.writeUTF(hostname);
            output.writeUTF(pop3Host);
            output.writeUTF(pop3User);
            output.writeUTF(pop3Pass);
            output.writeUTF(smtpHost);
            output.writeBoolean(smtpAuth);
            output.writeBoolean(httpProxy);
            output.writeUTF(httpHost);
            output.writeBoolean(debug);
            output.writeBoolean(imap);

            store.addRecord(buffer.toByteArray(), 0, buffer.size());
        }
        finally {
            store.closeRecordStore();
        }
    }

    /**
     * Restore the setup data from RMS.
     */
    private void loadSetupData() throws RecordStoreException, IOException {
        try {
            RecordStore store = RecordStore.openRecordStore("mail4me", false);

            try {
                ByteArrayInputStream buffer = new ByteArrayInputStream(store.getRecord(1));
                DataInputStream input = new DataInputStream(buffer);

                address  = input.readUTF();
                hostname = input.readUTF();
                pop3Host = input.readUTF();
                pop3User = input.readUTF();
                pop3Pass = input.readUTF();
                smtpHost = input.readUTF();
                smtpAuth = input.readBoolean();
                httpProxy = input.readBoolean();;
                httpHost = input.readUTF();
                debug = input.readBoolean();
                imap = input.readBoolean();
            }
            finally {
                store.closeRecordStore();
            }
        }
        catch (Exception e) {
        }
    }

    private void getMessageList() throws MailException, IOException {
        Vector list = new Vector();
        int count = pop3Client.getMessageCount();
        for (int i = 0; i < count; i++) {
            String uid = pop3Client.getUniqueId(i);
            int size = pop3Client.getSize(i);
            System.out.println("[INFO] Message " + i + " has ID \"" + uid + "\" and is " + size + " bytes.");
            Message message = pop3Client.getHeaders(i);
            list.addElement(message.getHeaderValue("Subject", "No subject")
            + " ("
            + Message.getMachineAddress(message.getHeaderValue("From", "No sender"))
            + ")");

            msgNumbers.insertElementAt(new Integer(i), 0);
        }
        inboxScreen.setMessages(list);
    }

    private void addPartToScreen(MimeDecoder mime) {
        if (mime.getPartCount() == 0) {
            if ("image/png".equals(mime.getType())) {
                byte[] bytes = mime.getBodyBytes();
                readScreen.addImage(Image.createImage(bytes, 0, bytes.length));
            }
            else if ((mime.getType() == null) || ("text/plain".equals(mime.getType()))) {
                String s = "";
                for (int i = 0; i < mime.getBodyLineCount(); i++) {
                    s = s + "\n" + mime.getBodyLine(i);
                }
                readScreen.addBody(s);
            }
            else {
                readScreen.addBody("\n[Unable to display \"" + mime.getType() + "\" part.]");
            }
        }
        else {
            for (int p = 0; p < mime.getPartCount(); p++) {
                addPartToScreen(mime.getPart(p));
            }
        }
    }
}
