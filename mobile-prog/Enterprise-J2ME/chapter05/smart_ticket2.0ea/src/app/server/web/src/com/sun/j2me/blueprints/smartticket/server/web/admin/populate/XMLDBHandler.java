

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
 * $Id: XMLDBHandler.java,v 1.3 2003/03/24 22:43:11 zull Exp $
 */
package com.sun.j2me.blueprints.smartticket.server.web.admin.populate;

import javax.xml.parsers.*;
import java.io.*;
import java.util.*;
import org.xml.sax.*;
import org.xml.sax.helpers.XMLFilterImpl;

public abstract class XMLDBHandler extends XMLFilterImpl {
    private static final int OFF = 0;
    private static final int READY = 1;
    private static final int PARSING = 2;
    private int state = OFF;
    private StringBuffer buffer = new StringBuffer();
    private boolean parsing = false;
    private HashMap context = new HashMap();
    private HashMap values = new HashMap();
    private String rootElementTag;
    private String elementTag;
    private boolean lazyInstantiation;

    public XMLDBHandler(XMLReader parent, String rootElementTag, 
                        String elementTag) {
        this(parent, rootElementTag, elementTag, false);

        return;
    }

    public XMLDBHandler(XMLReader parent, String rootElementTag, 
                        String elementTag, boolean lazyInstantiation) {
        super(parent);

        this.rootElementTag = rootElementTag;
        this.elementTag = elementTag;
        this.lazyInstantiation = lazyInstantiation;

        return;
    }

    public void startDocument() throws SAXException {
        state = OFF;

        values.clear();
        buffer.setLength(0);
        super.startDocument();

        return;
    } 

    public void startElement(String uri, String localName, 
                             String qualifiedName, 
                             Attributes attributes) throws SAXException {
        if (state == READY) {
            if (qualifiedName.equals(elementTag)) {
                state = PARSING;

                values.clear();

                for (int i = 0; i < attributes.getLength(); i++) {
                    setValue(qualifiedName + "/@" 
                             + attributes.getQName(i),    // To be optimized
                    normalizeValue(attributes.getQName(i), attributes.getValue(i)), 
                    state == PARSING ? values : context);
                } 

                try {
                    if (lazyInstantiation) {
                        create();
                    } 
                } catch (PopulateException exception) {
                    throw new SAXException(exception.getMessage(), 
                                           exception.getRootCause());
                } 

                return;
            } 
        } else if (state == OFF) {
            if (qualifiedName.equals(rootElementTag)) {
                state = READY;

                context.clear();
            } 
        } 

        if (state == READY) {
            for (int i = 0; i < attributes.getLength(); i++) {
                setValue(qualifiedName + "/@" 
                         + attributes.getQName(i),    // To be optimized
                normalizeValue(attributes.getQName(i), attributes.getValue(i)), 
                state == PARSING ? values : context);
            } 
        } 

        if (state == READY || state == OFF) {
            super.startElement(uri, localName, qualifiedName, attributes);
        } 

        return;
    } 

    public void characters(char[] chars, int start, 
                           int length) throws SAXException {
        if (state == PARSING) {
            buffer.append(new String(chars, start, length));
        } else if (state == READY) {
            buffer.append(new String(chars, start, length));
            super.characters(chars, start, length);
        } else {
            super.characters(chars, start, length);
        } 

        return;
    } 

    public void endElement(String uri, String localName, 
                           String qualifiedName) throws SAXException {
        if (state == READY) {
            if (qualifiedName.equals(rootElementTag)) {
                state = OFF;
            } else {
                setValue(qualifiedName, buffer.toString(), context);
                buffer.setLength(0);
            } 
        } 

        if (state == PARSING) {
            if (qualifiedName.equals(elementTag)) {
                try {
                    if (!lazyInstantiation) {
                        create();
                    } else {
                        update();
                    } 
                } catch (PopulateException exception) {
                    throw new SAXException(exception.getMessage(), 
                                           exception.getRootCause());
                } 

                state = READY;
            } else {
                setValue(qualifiedName, buffer.toString(), values);
            } 

            buffer.setLength(0);
        } else {
            super.endElement(uri, localName, qualifiedName);
        } 

        return;
    } 

    public void warning(SAXParseException exception) {
        System.err.println("[Warning]: " + exception.getMessage());

        return;
    } 

    public void error(SAXParseException exception) {
        System.err.println("[Error]: " + exception.getMessage());

        return;
    } 

    public void fatalError(SAXParseException exception) throws SAXException {
        System.err.println("[Fatal Error]: " + exception.getMessage());

        throw exception;
    } 

    private String normalizeValue(String attributeName, 
                                  String attributeValue) {
        if (attributeName.equals("xml:lang")) {
            return attributeValue.replace('-', '_');
        } 

        return attributeValue;
    } 

    private void setValue(String name, String value, Map map) {
        String key = name;

        if (values.get(key) == null) {
            map.put(key, value);

            key = name + "[" + 0 + "]";        // To be optimized

            map.put(key, value);
        } else {
            for (int i = 1; ; i++) {
                key = name + "[" + i + "]";    // To be optimized

                if (map.get(key) == null) {
                    map.put(key, value);

                    break;
                } 
            } 
        } 

        return;
    } 

    public String getValue(String name) {
        String value = (String) values.get(name);

        if (value == null) {
            value = (String) context.get(name);
        } 

        return value;
    } 

    public String getValue(String name, String defaultValue) {
        String value = getValue(name);

        return value != null ? value : defaultValue;
    } 

    public boolean getValue(String name, boolean defaultValue) {
        String value = getValue(name);

        return value != null ? Boolean.valueOf(value).booleanValue() 
               : defaultValue;
    } 

    public int getValue(String name, int defaultValue) {
        String value = getValue(name);

        try {
            if (value != null) {
                return Integer.valueOf(value).intValue();
            } 
        } catch (NumberFormatException exception) {}

        return defaultValue;
    } 

    public abstract void create() throws PopulateException;
    public abstract void update() throws PopulateException;
}

