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

import java.io.ByteArrayOutputStream;
import java.util.Vector;

/**
 * Is a helper class for decoding MIME data contained in e-mail messages. As
 * the name implies, MimeDecoder allows a kind of read-only view of the MIME
 * parts contained in a message, giving access to textual as well as binary
 * content.
 * <p>
 * Every MimeDecoder works on and thus represents exactly one MIME part, which
 * according to RFC 2045 - RFC 2049 may be either of the following two:
 * <ul>
 *   <li> A single-part, meaning that it's a leaf in the tree-like hierarchy of
 *        MIME parts. In this case content may be retrieved directly using the
 *        getBodyLine() or getBodyBytes() methods.
 *   <li> A multi-part, meaning that it's an inner node in the hierarchy. In
 *        that case subordinate parts should be investigated. These parts may
 *        again be single-part as well as multi-part.
 * </ul>
 * To start working with a MimeDecoder, a new instance has to be created for
 * a given message, which results in a top-level MIME view of the message.
 * Subordinate parts can then be accessed using the getPart() method that serves
 * as a kind of factory method for MimeDecoders representing the lower MIME
 * levels of the message.
 * <p>
 * Please note that a MimeDecoder is not notified of any changes to the
 * underlying message it is working on, so changes to the message will make the
 * MimeDecoder invalid. A new MimeDecoder should be created after changes have
 * been applied. Otherwise the results are unpredictable and will most probably
 * result in some kind of Exception.
 * <p>
 * One could argue that there should be a closer integration between the Message
 * and the MimeDecoder classes that is able to handle these changes. The
 * rationale for the given implementation is - again - memory savings. The
 * MimeDecoder is actually an optional component that *can* be used on a Message
 * if the application demands it. The Message class, on the other hand, is not
 * dependent on the MimeDecoder at all, which means that the Message class can
 * be deployed without having to deploy the MimeDecoder class, too.
 * <p>
 * A short note for implementators that want to understand the internal
 * structure of the MimeDecoder class: The basic idea is that the MimeDecoder
 * holds a direct reference to the message's Vector of lines (both header and
 * body) as well as two integer fields telling where the part represented by
 * the MimeDecoder starts and ends. In case of multi-parts, an additonal
 * Vector holds all the occurences of the part boundary string, which makes
 * it quite easy to create additional MimeDecoders for the subordinate MIME
 * parts.
 *
 * @see Message
 * @see #MimeDecoder(Message)
 * @see #getBodyLine(int)
 * @see #getBodyBytes()
 * @see #getPart(int)
 */
public class MimeDecoder {
    /**
     * Holds the header and body parts of the message this MimeDecoder is working
     * on. This is a direct reference to the Vector used inside the corresponding
     * message, not a copy of it.
     */
    private Vector lines;

    /**
     * Holds the line number where the body of the MIME part represented by this
     * MimeDecoder instance begins (in the underlying message).
     */
    private int begin;

    /**
     * Holds the line number where the body of the MIME part represented by this
     * MimeDecoder instance ends. Note that this is exclusive, so we're talking
     * about the first line that does not belong to the body any more.
     */
    private int end;

    /**
     * Holds the type of the MIME part represented by this MimeDecoder instance.
     * The type is deduced from the "Content-Type:" header field.
     */
    private String type;

    /**
     * Holds the name of the MIME part represented by this MimeDecoder instance.
     * The name is deduced from the "name=" sub-field of the "Content-Type:"
     * header field. Is is mainly used for binary attachments like pictures etc.
     */
    private String name;

    /**
     * Holds the encoding of the MIME part represented by this MimeDecoder
     * instance. The encoding is deduced from the "Content-Transfer-Encoding:"
     * header field.
     */
    private String encoding;

    /**
     * Holds all the line numbers where separators belonging to this MIME part are
     * found, in case this is a multipart body. The line numbers are stored in
     * Integer instances. Note that the actual subordinate MIME parts lie between
     * the given lines (exclusive!), so if we have n values in this Vector, we
     * actually have n-1 body parts.
     */
    private Vector parts;

    /**
     * Creates a new MimeDecoder for a given message. This constructor returns
     * a new instance of the MimeDecoder class that grants access to a top-level
     * MIME view of the message. Depending on the value returned by the getType()
     * method, the message contains only this part -- in which case it can be
     * accessed by the getBodyLine() or getBodyBytes() methods --, or it is
     * constructed from a number of subordinate MIME parts which can be accessed
     * one by one using the getPart() method.
     *
     * @see #getBodyLine(int)
     * @see #getBodyBytes()
     */
    public MimeDecoder(Message message) {
        init(message.getLines(), 0, message.getLines().size());
    }

    /**
     * Creates a new MimeDecoder instance for a subordinate part of the part or
     * message represented by the given MimeDecoder. The new part ranges from
     * the given beginning to the given end. This is an internal constructor
     * required by the getPart() method.
     */
    private MimeDecoder(MimeDecoder parent, int begin, int end) {
        init(parent.lines, begin, end);
    }

    /**
     * Initializes a newly created MimeDecoder. This is the most tricky method
     * in the whole class, because it has to fetch all MIME information from
     * the part's header and find all the occurences of the part boundary, in
     * case it is a multipart body.
     * <p>
     * The method expects the messages line vector and the beginning of the
     * part's header as well as the end (being the first line after the body).
     * Note that during its operation the method advances the begin counter to
     * the beginning of the part body, before it assigns the two counters to
     * the private fields of the same names.
     */
    private void init(Vector lines, int begin, int end) {
        this.lines = lines;

        /**
         * Walk through the whole header and look for the MIME-relevant fields.
         * The end of the header is denoted by the usual empty line. Note that
         * the begin counter advances while we're doing this, so that it points
         * to the first line of the body later.
         */
        String s = (String)lines.elementAt(begin);
        while (!s.equals("")) {
            begin++;

            /**
             * Undo header folding, that is, put logical header lines that span
             * multiple physical ones into one String. While this is superfluous
             * for the main message header (because the Pop3Client class already
             * does this) it is necessary for the headers of the various MIME parts
             * (which the Pop3Client doesn't know about).
             */
            while (begin < end) {
                String t = (String)lines.elementAt(begin);
                if (t.startsWith(" ") || t.startsWith("\t")) {
                    s = s + t;
                    begin++;
                }
                else break;
            }

            /**
             * Fetch a lower-case representation of the field name found in the
             * current header line. Check if it is a MIME-relevant field. If so,
             * extract additional information from the field and store it in the
             * corresponding attributes of the MIME decoder.
             */
            String name = Message.getStringName(s).toLowerCase();

            if (name.equals("content-type")) {
                /**
                 * The "Content-Type:" field usually contains several sub-fields, so
                 * the first thing we do is split the list of sub-fields into a
                 * String array. The first array entry must be the basic content type.
                 * The other elements may contain additional information, so we walk
                 * through the array starting from index 1.
                 */
                String[] elements = Message.getStringElements(Message.getStringValue(s), ';');

                type = elements[0].toLowerCase();

                for (int i = 1; i < elements.length; i++) {
                    /**
                     * If the MimeDecoder represents a multipart body *and* the part
                     * boundary string is found, iterate through the whole body to find
                     * all the occurences of the boundary string.
                     */
                    if (type.startsWith("multipart/") && getStringName(elements[i]).toLowerCase().equals("boundary")) {
                        /**
                         * The occurences of the boundary inside the body begin with two
                         * additional hyphens.
                         */
                        String boundary = "--" + getStringValue(elements[i]);

                        /**
                         * Initialize and fill the part list.
                         */
                        parts = new Vector();
                        for (int j = begin; j < end; j++) {
                            if (getLine(j).startsWith(boundary)) {
                                parts.addElement(new Integer(j));
                            }
                        }
                    }

                    /**
                     * If the MimeDecoder represents a singlepart body, that is, a leaf
                     * in the tree-like hierarchy of MIME parts, it may have a filename.
                     */
                    else if (getStringName(elements[i]).toLowerCase().equals("name")) {
                        this.name = getStringValue(elements[i]);
                    }
                }
            }

            /**
             * The "Content-Transfer-Encoding" attribute tells us how the MIME part
             * is encoded. A common encoding that doesn't require any additional work
             * is "text/plain", which is also the default encoding if none is
             * specified explicitly. If the encoding is "base64", the part's data
             * should be retrieved using the getBodyBytes() method.
             */
            else if (name.equals("content-transfer-encoding")) {
                encoding = Message.getStringValue(s);
            }

            if (begin < end) {
                s = (String)lines.elementAt(begin);
            }
            else {
                /**
                 * Emergency stop.
                 */
                s = "";
            }
        }

        /**
         * Finally, store the updated values of the begin and end counters. At this
         * point they denote the beginning and end of the part's body, respectively.
         */
        this.begin = begin;
        this.end = end;
    }

    /**
     * Gets a line from the message that this MimeDecoder is working on. This
     * is an internal method that was introduced to reduce the number of type
     * casts necessary when accessing the underlying message.
     */
    private String getLine(int index) {
        return (String)lines.elementAt(index);
    }

    /**
     * Returns the number of lines in this MIME part's body.
     *
     * @see #getBodyLine(int)
     * @see #getBodyBytes()
     */
    public int getBodyLineCount() {
        return end - begin;
    }

    /**
     * Returns a line of this MIME part's body its index.
     *
     * @see #getBodyLineCount()
     * @see #getBodyBytes()
     */
    public String getBodyLine(int index) throws ArrayIndexOutOfBoundsException {
        if ((index < 0) || (index >= end - begin)) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        return (String)lines.elementAt(begin + index);
    }

    /**
     * Returns the binary data contained in this MIME part. This method should
     * only be invoked on MIME parts whose encoding is "base64", otherwise the
     * result will be more or less arbitrary.
     */
    public byte[] getBodyBytes() {
        /**
         * The implementation is rather straightforward: Walk through the body
         * lines one by one, have them decoded and append the result the an
         * ByteArrayOutputStream that creates the resulting array.
         */
        ByteArrayOutputStream bos = new ByteArrayOutputStream();

        for (int i = 0; i < getBodyLineCount(); i++) {
            decode(getBodyLine(i), bos);
        }

        return bos.toByteArray();
    }

    /**
     * Returns the number of MIME parts that constitute this MIME part. If the
     * result is zero, this MIME part is a leaf in the tree-like hierarchy of
     * MIME parts, and its contents can be retrieved by either the getBodyLine()
     * or the getBodyBytes() methods. Otherwise the part is an inner node in the
     * part hierarchy, and contains one or more subordinate MIME parts which can
     * be accessed using the getPart() method.
     */
    public int getPartCount() {
        if (parts == null) {
            return 0;
        }
        else {
            return parts.size() - 1;
        }
    }

    /**
     * Returns a subordinate part of this MIME part. This method should only be
     * called if the getPartCount() method returns a value greater than zero,
     * meaning that this MIME part is an inner node in the hierarchy of parts
     * and has one ore more subordinate MIME parts. Note that this method acts
     * as some kind of factory method for MimeDecoders that do not work on the
     * top-level of a message, so the MimeDecoder objects are created on-demand
     * and do not pollute the limited J2ME heap if they're not really needed.
     */
    public MimeDecoder getPart(int index) {
        if ((index < 0) || (index >= getPartCount())) {
            throw new ArrayIndexOutOfBoundsException(index);
        }

        int i = ((Integer)parts.elementAt(index)).intValue();
        int j = ((Integer)parts.elementAt(index + 1)).intValue();

        return new MimeDecoder(this, i + 1, j);
    }

    /**
     * Returns the type of this MIME part, which is deduced from the
     * "Content-Type:" header field. Frequent header types that do work
     * well in a J2ME environment are "text/plain", which means plain ASCII
     * text, and "image/png" which means that the part holds a PNG image,
     * probably encoded in base64 (the latter information can be retrieved
     * by the getEncoding() method).
     *
     * @see #getEncoding()
     * @see #getName()
     */
    public String getType() {
        return type;
    }

    /**
     * Returns the filename of this MIME part, which is deduced from the "name="
     * sub-field of the "Content-Type:" header field. This value will probably
     * only hold a non-null value for binary attachments, for example images.
     *
     * @see #getType()
     * @see #getEncoding()
     */
    public String getName() {
        return name;
    }

    /**
     * Returns the encoding of this MIME part, which is deduced from the
     * "Content-Transfer-Encoding:" header field. The encoding tells us how data
     * is to be retrieved from the MIME part. If the value is null, the part's
     * contents can be retrieved line by line using the getBodyLine() method.
     * If the encoding is "base64", the part's data should be retrieved using the
     * getBodyBytes() method. Other encodings are currently not supported by the
     * MimeDecoder itself.
     */
    public String getEncoding() {
        return encoding;
    }

    /**
     * Returns the name contained in a name/value pair string. The name is
     * everything up to, but not including, the first "=" sign in the string. If
     * no "=" can be found, null is returned, assuming the string doesn't hold a
     * name at all.
     * <p>
     * Note that this method is different from the getStringName() method
     * of the Message class due to the different separator character.
     *
     * @see #getStringValue(java.lang.String)
     * @see Message#getStringName(java.lang.String)
     */
    public static String getStringName(String s) {
        int p = s.indexOf('=');

        if (p == -1) {
            return null;
        }
        else {
            return s.substring(0, p);
        }
    }

    /**
     * Returns the value contained in a name/value pair string. The value is
     * everything following, but not including, the first "=" sign in the string.
     * If no "=" can be found, the whole string is returned, assuming it holds
     * only a value, but not name at all. The method unquotes values enclosed in
     * double quotes automatically.
     * <p>
     * Note that this method is different from the getStringValue() method
     * of the Message class due to the different separator character.
     *
     * @see #getStringName(java.lang.String)
     * @see Message#getStringValue(java.lang.String)
     */
    public static String getStringValue(String s) {
        String value;

        int p = s.indexOf('=');

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
     * Does a base64 decoding for a single character.
     * <p>
     * Contains code from Stefan Haustein's KObjects library (www.kobjects.org)
     * used by permission.
     */
    private static int decode(char c) {
        if (c >= 'A' && c <= 'Z')
            return ((int) c) - 65;
        else if (c >= 'a' && c <= 'z')
            return ((int) c) - 97 + 26;
        else if (c >= '0' && c <= '9')
            return ((int) c) - 48 + 26 + 26;
        else switch (c) {
            case '+': return 62;
            case '/': return 63;
            case '=': return 0;
            default:  throw new RuntimeException("Illegal MIME character '" + c + "'");
        }
    }

    /**
     * Does a base64 decoding for a whole String and writes the resulting bytes
     * into a stream.
     * <p>
     * Contains code from Stefan Haustein's KObjects library (www.kobjects.org)
     * used by permission.
     */
    private static void decode(String s, ByteArrayOutputStream bos) {
        int i = 0;
        int len = s.length();

        while (true) {
            while (i < len && s.charAt(i) <= ' ') i++;

            if (i == len) break;

            int tri = (decode(s.charAt(i)) << 18)
                + (decode(s.charAt(i+1)) << 12)
                + (decode(s.charAt(i+2)) << 6)
                + (decode(s.charAt(i+3)));

            bos.write((tri >> 16) & 255);
            if (s.charAt(i+2) == '=') break;
            bos.write((tri >> 8) & 255);
            if (s.charAt(i+3) == '=') break;
            bos.write(tri & 255);

            i += 4;
        }
    }
}
