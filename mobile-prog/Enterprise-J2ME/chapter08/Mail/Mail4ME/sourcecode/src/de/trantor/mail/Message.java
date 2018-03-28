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

import java.util.Calendar;
import java.util.Random;
import java.util.TimeZone;
import java.util.Vector;
import java.io.IOException;

/**
 * Represents an internet mail message according to RFC 822. From the user's
 * point of view, the class basically consists of two parts:
 *
 * <ul>
 *   <li> An array-like structure holding the message's header lines. Each
 *        header line is stored in the textual format that is also used during
 *        transmission. To make access to the header data convenient, there are
 *        several methods for dealing with header lines as a whole, with names
 *        and fields separately and for searching specific header fields.
 *
 *   <li> An array-like structure holding the message's body lines. Again,
 *        several methods are provided to manipulate the body part of the
 *        message.
 * </ul>
 *
 * From the classes' internal point of view, both array-like structures are
 * held in a single Vector instance to save memory.
 * <p>
 * One could argue that the fully textual representation of a message results
 * in a slower operation than would be possible with - for example - a
 * hashtable. This is true, but the representation has been chosen with regard
 * to several concerns:
 *
 * <ul>
 *   <li> The limited memory of J2ME devices. The textual representation
 *        requires a lot less memory than a data structure separating field
 *        names and field values would.
 *
 *   <li> The principal of minimal mangling. This principle dictates that a
 *        system (possibly) responsible for e-mail transfer (a mailing list
 *        processor, for example) should keep changes to the messages to a
 *        minimum. We're trying to adhere to this principle in order to not
 *        limit the possible applications of the Mail4ME package. Since the
 *        internal representation of an e-mail inside a Message instance is
 *        equal to the external one, there is no mangling at all, as long as
 *        the message header or body is not modified on purpose. Note that this
 *        would not be the case if the headers were kept in, say, a hashtable,
 *        because it would be complicated to reconstruct the original order of
 *        the header fields when converting the message to the external
 *        representation again.
 * </ul>
 *
 * Messages can be retrieved from a POP3 server using the Pop3Client class.
 * They can be sent across the Internet using an instance of SmtpClient.
 * Depending on the application, it might be necessary to enclose the message
 * in an Envelope instance before sending it.
 *
 * @see Pop3Client
 * @see SmtpClient
 * @see Envelope
 */
public class Message {

    /**
     * Holds the message's header and body, line by line and separated by the
     * usual empty line between header and body.
     */
    private Vector lines = new Vector();

    /**
     * Holds the number of lines that belong to the message's header, that is,
     * the number of lines up to (but no including) the empty line that separates
     * header and body.
     */
    private int headerSize = 0;

    /**
     * Contructs a new, totally empty message.
     *
     * @see #Message(java.lang.String, java.lang.String, java.lang.String)
     */
    public Message() {
        lines.addElement("");
    }

    /**
     * Contructs a new message with the given sender ("From:" field), recipient
     * ("To:" field) and subject, respectively. If any of the parameters is
     * null, the corresponding field is not added to the message. The method
     * ensures that the newly created message gets a message ID and a date
     * field holding the current date/time.
     *
     * @see #Message()
     */
    public Message(String from, String to, String subject) {
        lines.addElement("");

        if (from != null) {
            addHeaderLine("From: " + from);

            /**
             * If we have sender information, we use it to construct a (hopefully)
             * unique message ID.
             */
            String s = getMachineAddress(from);
            addHeaderLine("Message-ID: <" + getRandomString() + "." + s + ">");
        }
        else {
            /**
             * Without sender information we have nothing to feed into the message
             * ID generator, so all we can do is keep our fingers crossed and hope
             * that the mixture of millisecond counter and random number suffices.
             */
            addHeaderLine("Message-ID: <" + getRandomString() + ">");
        }

        if (to != null) {
            addHeaderLine("To: " + to);
        }

        if (subject != null) {
            addHeaderLine("Subject: " + subject);
        }

        addHeaderLine("Date: " + getCanonicalDate(Calendar.getInstance(), TimeZone.getDefault()));
    }

    /**
     * Returns one of the message's header lines by its index.
     *
     * @see #addHeaderLine
     * @see #getHeaderLineCount
     * @see #setHeaderLine
     * @see #insertHeaderLine
     * @see #removeHeaderLine
     */
    public String getHeaderLine(int index) throws ArrayIndexOutOfBoundsException {
        if ((index < 0) || (index >= headerSize)) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        return (String)lines.elementAt(index);
    }

    /**
     * Returns the field name one of the given header line. The field name is
     * everything up to, but not including the first ':' of the line. If the
     * given header line does not contain a ':' character at all, the line is
     * assumed not to hold a correct header field, and null is returned
     * instead.
     *
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public String getHeaderName(int index) throws ArrayIndexOutOfBoundsException {
        return getStringName(getHeaderLine(index));
    }

    /**
     * Returns the field value one of the given header line. The field value is
     * everything that follows the first ':' of the line. Leading and trailing
     * spaces are removed. If the given header line does not contain a ':'
     * character at all, the line is assumed to hold a nameless value, and the
     * whole line is returned instead.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public String getHeaderValue(int index) throws ArrayIndexOutOfBoundsException {
        return getStringValue(getHeaderLine(index));
    }

    /**
     * Searches for a header field. The method returns the index of the first
     * header line that contains the given field name, and it starts searching
     * from the given index. If the header field can't be found, -1 is returned
     * instead.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     */
    public int getHeaderIndex(String name, int startIndex) {
        String lowerName = name.toLowerCase();

        for (int i = startIndex; i < headerSize; i++) {
            String s = getHeaderName(i);
            if ((s != null) && (s.toLowerCase().equals(lowerName))) {
                return i;
            }
        }

        return -1;
    }

    /**
     * Searches for a header field. The method returns the index of the first
     * header line that contains the given field name, and it starts searching
     * from the first header line. If the header field can't be found, -1 is
     * returned instead.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public int getHeaderIndex(String name) {
        return getHeaderIndex(name, 0);
    }

    /**
     * Returns the value of the first occurence of the given header field.
     * If the field doesn't exist, null is returned instead.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public String getHeaderValue(String name) {
        return getHeaderValue(name, null);
    }

    /**
     * Returns the value of the first occurence of the given header field.
     * If the field does not exist at all, the specified default value is
     * returned instead.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getAllHeaderValues
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public String getHeaderValue(String name, String def) {
        int i = getHeaderIndex(name);

        if (i == -1) {
            return def;
        }
        else {
            return getHeaderValue(i);
        }
    }

    /**
     * Returns the values of all occurences of the given header field in the
     * this message. The results are stored in a string array. If the given
     * field doesn't exist at all, an empty array is returned.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #setHeaderValue
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public String[] getAllHeaderValues(String name) {
        Vector lines = new Vector();

        int i = getHeaderIndex(name);
        while (i != -1) {
            lines.addElement(getHeaderValue(i));
            i = getHeaderIndex(name, i + 1);
        }

        String[] result = new String[lines.size()];
        for (int j = 0; j < lines.size(); j++) {
            result[j] = (String)lines.elementAt(j);
        }

        return result;
    }

    /**
     * Replaces the given header line by a new one.
     *
     * @see #addHeaderLine
     * @see #getHeaderLineCount
     * @see #getHeaderLine
     * @see #insertHeaderLine
     * @see #removeHeaderLine
     */
    public void setHeaderLine(int index, String line) {
        if ((index < 0) || (index >= headerSize)) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.setElementAt(line, index);
    }

    /**
     * Returns the number of available header lines.
     *
     * @see #addHeaderLine
     * @see #getHeaderLine
     * @see #setHeaderLine
     * @see #insertHeaderLine
     * @see #removeHeaderLine
     */
    public int getHeaderLineCount() {
        return headerSize;
    }

    /**
     * Adds a new line to the end of the message's header, returning its index.
     *
     * @see #getHeaderLineCount
     * @see #getHeaderLine
     * @see #setHeaderLine
     * @see #insertHeaderLine
     * @see #removeHeaderLine
     */
    public int addHeaderLine(String line) {
        lines.insertElementAt(line, headerSize);
        return headerSize++;
    }

    /**
     * Inserts a new line at a given position into the message's header.
     *
     * @see #addHeaderLine
     * @see #getHeaderLineCount
     * @see #getHeaderLine
     * @see #setHeaderLine
     * @see #removeHeaderLine
     */
    public void insertHeaderLine(int index, String line) {
        if ((index < 0) || (index > headerSize)) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.insertElementAt(line, index);
        headerSize++;
    }

    /**
     * Removes a complete header line.
     *
     * @see #addHeaderLine
     * @see #getHeaderLineCount
     * @see #getHeaderLine
     * @see #setHeaderLine
     * @see #insertHeaderLine
     */
    public void removeHeaderLine(int index) {
        if ((index < 0) || (index >= headerSize)) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.removeElementAt(index);
        headerSize--;
    }

    /**
     * Sets a header field to a given value. This method changes the first
     * occurence of the given header field to the given value. If the field
     * doesn't exist in the message's header, it is appended as a new header
     * line. If the value parameter is null, an attempt is made to remove the
     * given field from the header.
     *
     * @see #getHeaderName
     * @see #getHeaderValue(int)
     * @see #getHeaderValue(java.lang.String)
     * @see #getHeaderValue(java.lang.String, java.lang.String)
     * @see #getAllHeaderValues
     * @see #getHeaderIndex(java.lang.String)
     * @see #getHeaderIndex(java.lang.String, int)
     */
    public void setHeaderValue(String name, String value) {
        int i = getHeaderIndex(name);
        if (i == -1) {
            if (value != null) {
                addHeaderLine(name + ": " + value);
            }
        }
        else {
            if (value != null) {
                setHeaderLine(i, name + ": " + value);
            }
            else {
                removeHeaderLine(i);
            }
        }
    }

    /**
     * Returns a line of the message's body by its index.
     *
     * @see #getBodyLineCount
     * @see #setBodyLine
     * @see #addBodyLine
     * @see #removeBodyLine
     * @see #insertBodyLine
     */
    public String getBodyLine(int index) {
        if ((index < 0) || (index >= getBodyLineCount())) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        return (String)lines.elementAt(headerSize + index + 1);
    }

    /**
     * Replaces the line of the message's body at the given index.
     *
     * @see #getBodyLine
     * @see #getBodyLineCount
     * @see #addBodyLine
     * @see #removeBodyLine
     * @see #insertBodyLine
     */
    public void setBodyLine(int index, String line) {
        if ((index < 0) || (index >= getBodyLineCount())) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.setElementAt(line, headerSize + index + 1);
    }

    /**
     * Returns the number of lines in the message's body.
     *
     * @see #getBodyLine
     * @see #setBodyLine
     * @see #addBodyLine
     * @see #removeBodyLine
     * @see #insertBodyLine
     */
    public int getBodyLineCount() {
        return lines.size() - headerSize - 1;
    }

    /**
     * Adds a line to the message body, returning its index.
     *
     * @see #getBodyLine
     * @see #getBodyLineCount
     * @see #setBodyLine
     * @see #removeBodyLine
     * @see #insertBodyLine
     */
    public int addBodyLine(String line) {
        lines.addElement(line);
        return getBodyLineCount() - 1;
    }

    /**
     * Inserts a new line into the message's body, at the given index.
     *
     * @see #getBodyLine
     * @see #getBodyLineCount
     * @see #setBodyLine
     * @see #addBodyLine
     * @see #removeBodyLine
     */
    public void insertBodyLine(int index, String line) {
        if ((index < 0) || (index > getBodyLineCount())) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.insertElementAt(line, headerSize + index + 1);
    }

    /**
     * Removes a line from the message's body.
     *
     * @see #getBodyLine
     * @see #getBodyLineCount
     * @see #setBodyLine
     * @see #addBodyLine
     * @see #insertBodyLine
     */
    public void removeBodyLine(int index) {
        if ((index < 0) || (index >= getBodyLineCount())) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        lines.removeElementAt(headerSize + index + 1);
    }

    /**
     * Returns the machine-readable part of an address, that is, the part
     * that is actually used in delivering the message to a recipient.
     * For an address like, say,
     * <pre>
     *   "Joerg Pleumann" &lt;joerg@pleumann.de&gt;
     * </pre>
     * this would be "joerg@pleumann.de" (without the quotes).
     */
    public static String getMachineAddress(String address) {
        int p, q;

        p = address.indexOf('<');
        q =  address.indexOf('>', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(p + 1, q);

        p = address.indexOf('(');
        q = address.indexOf(')', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(0, p).trim();

        p = address.indexOf('"');
        q = address.indexOf('"', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(0, p).trim();

        return address;
    }

  /*
         Some additional requirements from the RFC that have to be addressed
         by a *real* address parser:
         - Several addresses in one line, like:   addr1, addr2, addr3
         - A named list of addresses like:        friends: a, b, c;
         - Everything in (...) is a comment
         - Everything in "..." is a literal
         - Everything in <...> is a machine-readable part
         - Using escape sequences \... is also allowed.
   */

    /**
     * Returns the human-readable part of an address, that is, the part
     * that usually holds the real-life name of a user. For an address
     * like, say,
     * <pre>
     *   "Joerg Pleumann" &lt;joerg@pleumann.de&gt;
     * </pre>
     * this would be "Joerg Pleumann" (without the quotes).
     */
    public static String getDisplayAddress(String address) {
        int p, q;

        p = address.indexOf('"');
        q = address.indexOf('"', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(p + 1, q);

        p = address.indexOf('(');
        q = address.indexOf(')', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(p + 1, q);

        p = address.indexOf('<');
        q =  address.indexOf('>', p + 1);
        if ((p != -1) && (q != -1)) return address.substring(0, p).trim();

        return "";
    }

    /**
     * Returns a canonical representation of an e-mail address. It basically
     * consists of the username and domain parts enclosed in angular brackets.
     * It does not contain the user's realname.
     * [May be removed in favour of getMachineAddress or expanded to hold
     * display *and* machine address.]
     */
        /*
        public static String getCanonicalAddress(String address) {
                int p = address.indexOf('<');
                int q =  address.indexOf('>', p + 1);
                if ((p != -1) && (q != -1)) return address.substring(p, q + 1);

                p = address.indexOf('(');
                q = address.indexOf(')', p + 1);
                if ((p != -1) && (q != -1)) return "<" + address.substring(0, p) + ">";

                p = address.indexOf('"');
                q = address.indexOf('"', p + 1);
                if ((p != -1) && (q != -1)) return "<" + address.substring(0, p) + ">";

                return "<" + address + ">";
        }
         */

    /**
     * Converts an integer to a string. If the result has less digits than the
     * specified length, it is left-padded with zeroes. This is a helper
     * method required by getCanonicalDate(). The length parameter must not
     * exceed 4, the number must not exceed 9999. Since the method is used
     * only internally, this should be easy to guarantee.
     */
    private static String intToStr(int value, int length) {
        String result = Integer.toString(value);
        result = "0000".substring(result.length(), length) + result;
        return result;
    }

    /**
     * Returns a formatted date. This method converts the given date/time/zone
     * information into the textual format defined by RFC 822, that is, into a
     * string looking like this:
     * <pre>
     *   Mon, 01 Jan 1970 23:59:59 GMT+1000
     * </pre>
     * If the timezone parameter is null, no zone information is appended to
     * the result.
     */
    public static String getCanonicalDate(Calendar calendar, TimeZone timezone) {
        String monthNames = "JanFebMarAprMayJunJulAugSepOctNovDec";
        String dayNames = "SunMonTueWedThuFriSatSun";

        /**
         * Get the various fields from the calendar parameter.
         */
        int year = calendar.get(Calendar.YEAR);
        int month = calendar.get(Calendar.MONTH);
        int day = calendar.get(Calendar.DAY_OF_MONTH);
        int weekday = calendar.get(Calendar.DAY_OF_WEEK) - 1;
        int hour = calendar.get(Calendar.HOUR_OF_DAY);
        int minute = calendar.get(Calendar.MINUTE);
        int second = calendar.get(Calendar.SECOND);

        /**
         * Format the first part of the result string.
         */
        String result = dayNames.substring(3 * weekday, 3 * weekday + 3) + ", "
        + intToStr(day, 2) + " "
        + monthNames.substring(3 * month, 3 * month + 3) + " "
        + intToStr(year, 4) + " "
        + intToStr(hour, 2) + ":"
        + intToStr(minute, 2) + ":"
        + intToStr(second, 2);

        /**
         * If we have a timezone parameter, append the difference to GMT (=UCT)
         * including possible daylight savings to the string. The first two digits
         * hold hours, the next two digits hold minutes. So, GMT+0130 means
         * that local time is GMT plus one and a half hours.
         */
        if (timezone != null) {
            int offset = timezone.getRawOffset() / 1000;
            if (timezone.useDaylightTime()) offset = offset + 3600;

            String name;
            if (offset >= 0) {
                name = " GMT+";
            }
            else {
                name = " GMT-";
                offset = -offset;
            }

            result = result + name + intToStr(offset / 3600, 2) + intToStr(offset % 3600, 2);
        }

        return result;
    }

    /**
     * Splits a list of elements into a String array. The method takes a String
     * holding a list of elements as well as a separator character. It splits the
     * list with regard to the given separator and puts each element into a newly
     * created String array. Occurences of the separator character enclosed in
     * double quotes are not treated as separators. The method can be used to
     * separate those message headers that hold several sub-attributes in one
     * value (for example a list of recipients in one "From:" field or the
     * various pieces of information stored in the "Content-Type:" field of a
     * MIME message.
     *
     * @see #getStringName
     * @see #getStringValue
     */
    public static String[] getStringElements(String list, char separator){

        Vector temp = new Vector();

        list = list + separator;
        int len = list.length();
        int p = 0;

        /**
         * Walk through the string as long as we're not done. From the
         * current starting position p, look for the next semicolon using a
         * second counter q. If a semicolon is enclosed in double quotes,
         * it's not the one we're looking for.
         */
        while (p < len) {
            int q = p;
            boolean quote = false;

            while ((q < len) && ((list.charAt(q) != separator) || quote)) {
                if (list.charAt(q) == '"') quote = !quote;
                q++;
            }

            String element = list.substring(p, q).trim();
            if (element.length() != 0) {
                temp.addElement(element);
            }
            p = q + 1;
        }

        /**
         * Create resulting String array from temporary Vector.
         */
        String[] result = new String[temp.size()];
        for (int i = 0; i < temp.size(); i++) {
            result[i] = (String)(temp.elementAt(i));
        }

        return result;
    }

    /**
     * Returns the name contained in a name/value pair string. The name is
     * everything up to, but not including, the first ":" sign in the string. If
     * no ":" can be found, null is returned, assuming the string doesn't hold a
     * name at all.
     *
     * @see #getStringElements
     * @see #getStringValue
     */
    public static String getStringName(String s) {
        int p = s.indexOf(':');

        if (p == -1) {
            return null;
        }
        else {
            return s.substring(0, p);
        }
    }

    /**
     * Returns the value contained in a name/value pair string. The value is
     * everything following, but not including, the first ":" sign in the string.
     * If no ":" can be found, the whole string is returned, assuming it holds
     * only a value, but not name at all. The method unquotes values enclosed in
     * double quotes automatically.
     *
     * @see #getStringElements
     * @see #getStringName
     */
    public static String getStringValue(String s) {
        String value;

        int p = s.indexOf(':');

        if (p == -1) {
            value = s;
        }
        else {
            value = s.substring(p + 1);
        }

        /**
         * Get rid of leading and trailing spaces.
         */
        value = value.trim();

        /**
         * Unquote result, if necessary.
         */
        if ((value.length() > 1) && (value.charAt(0) == '"') && ((value.charAt(value.length() - 1) == '"'))) {
            value = value.substring(1, value.length() - 1);
        }

        return value;
    }

    /**
     * Returns a random string. The method uses the system's millisecond timer
     * and an additional random number to create a string that can serve as a
     * basis for (hopefully) unique identifiers in messages or MIME parts. It is
     * recommended to use this string in conjunction with another string that
     * identifies the user or the user's system, for example an e-mail address
     * or a hostname, to ensure uniqueness across machine boundaries.
     */
    public static String getRandomString() {
        return Long.toString(System.currentTimeMillis(), 36) + "." +
        Integer.toString(Math.abs(new Random().nextInt()), 36);
    }

    /**
     * Returns the message's internal list of lines. This method grants access to
     * the Vector that holds the message's header and body lines as well as the
     * empty line separating them. It is used for package-internal purposes only
     * (and thus has package visibility). As an example, the MimeDecoder class
     * needs to see this Vector to find out about all the MIME parts contained in
     * the message. The alternative would be to duplicate the whole Vector inside
     * a MimeDecoder, but since messages can become quite large, this would waste
     * a lot of memory. Another class that makes use of this method is SmtpClient.
     *
     * @see MimeDecoder
     * @see SmtpClient
     */
    Vector getLines() {
        return lines;
    }
}
