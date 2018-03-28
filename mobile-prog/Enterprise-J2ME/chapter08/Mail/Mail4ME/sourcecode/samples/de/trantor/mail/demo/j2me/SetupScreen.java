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

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

/**
 * Represents the setup-screen that is shown at startup. Currently
 * not very well documented due to lack of time.
 */
public class SetupScreen extends Form {

    private TextField address  = new TextField("Address:",  "", 128, TextField.EMAILADDR);
    private TextField hostname = new TextField("Local host:", "", 128, TextField.URL);
    String[] names = {"POP3", "IMAP"};
    private ChoiceGroup services = new ChoiceGroup("Inbox type:", Choice.EXCLUSIVE, names, null);
    private TextField pop3Host = new TextField("Inbox host:", "", 128, TextField.URL);
    private TextField pop3User = new TextField("Username :", "", 128, TextField.ANY);
    private TextField pop3Pass = new TextField("Password:", "", 128, TextField.PASSWORD);
    private TextField smtpHost = new TextField("SMTP host:", "", 128, TextField.URL);
    String[] options = {"Off", "On"};
    private ChoiceGroup smtpAuth = new ChoiceGroup("SMTP auth:", Choice.EXCLUSIVE, options, null);

    private ChoiceGroup httpProxy = new ChoiceGroup("HTTP proxy:", Choice.EXCLUSIVE, options, null);
    private TextField httpHost = new TextField("Proxy host:", "", 128, TextField.URL);

    private ChoiceGroup debug = new ChoiceGroup("Debugging:", Choice.EXCLUSIVE, options, null);

    public static Command OK = new Command("Ok", Command.SCREEN, 1);
    public static Command EXIT = new Command("Exit", Command.EXIT, 2);

    //public static Command EXIT = new Command("Exit", Command.EXIT, 99);

    public SetupScreen(MailMIDlet midlet) {
        super("Setup");

        append(address);
        append(hostname);
        append(services);
        append(pop3Host);
        append(pop3User);
        append(pop3Pass);
        append(smtpHost);
        append(smtpAuth);
        append(httpProxy);
        append(httpHost);
        append(debug);

        addCommand(OK);
        addCommand(EXIT);

        setCommandListener(midlet);
    }

    public String getAddress() {
        return address.getString();
    }

    public String getHostname() {
        return hostname.getString();
    }

    public boolean getImap() {
        return services.isSelected(1);
    }

    public String getPop3Host() {
        return pop3Host.getString();
    }

    public String getPop3User() {
        return pop3User.getString();
    }

    public String getPop3Pass() {
        return pop3Pass.getString();
    }

    public boolean getDebug() {
        return debug.isSelected(1);
    }

    public String getSmtpHost() {
        return smtpHost.getString();
    }

    public boolean getSmtpAuth() {
        return smtpAuth.isSelected(1);
    }

    public String getHttpHost() {
        return httpHost.getString();
    }

    public boolean getHttpProxy() {
        return httpProxy.isSelected(1);
    }

    public void setAddress(String address) {
        this.address.setString(address);
    }

    public void setHostname(String hostname) {
        this.hostname.setString(hostname);
    }

    public void setImap(boolean value) {
        if (value) {
            services.setSelectedIndex(1, true);
        }
    }

    public void setPop3Host(String pop3Host) {
        this.pop3Host.setString(pop3Host);
    }

    public void setPop3User(String pop3User) {
        this.pop3User.setString(pop3User);
    }

    public void setPop3Pass(String pop3Pass) {
        this.pop3Pass.setString(pop3Pass);
    }

    public void setSmtpHost(String smtpHost) {
        this.smtpHost.setString(smtpHost);
    }

    public void setSmtpAuth(boolean value) {
        if (value) {
            smtpAuth.setSelectedIndex(1, true);
        }
    }

    public void setHttpHost(String httpHost) {
        this.httpHost.setString(httpHost);
    }

    public void setHttpProxy(boolean value) {
        if (value) {
            httpProxy.setSelectedIndex(1, true);
        }
    }

    public void setDebug(boolean value) {
        if (value) {
            debug.setSelectedIndex(1, true);
        }
    }
}