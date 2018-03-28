/*-----------------------------------------------------------------------

  P O I N T B A S E,	 I N C .

  -----------------------------------------------------------------------

 (C) Copyright 2002, 2003 PointBase, Inc.	All rights reserved.

 NOTICE:  All information contained herein or attendant hereto is,
          and remains, the property of PointBase, Inc.  Many of the
          intellectual and technical concepts contained herein are
          proprietary to PointBase, Incorporated and may be covered
          by U.S. and Foreign Patents or Patents Pending, or are
          protected as trade secrets.  Any dissemination of this
          information or reproduction of this material is strictly
          forbidden unless prior written permission is obtained
          from PointBase, Inc.

 -----------------------------------------------------------------------
*/

import java.awt.*;
import java.awt.image.*;
import java.io.*;

// Serializable Image
public class Picture extends Component implements Serializable {
    // Fields
    private Image image;
    private byte data[];

    // Constructors
    public Picture() {
        image = null;
    }

    public Picture(String filename) {
        loadImage(filename);
    }

    // Methods
    public int getWidth() {
        if (image == null) {
            return 0;
        } else {
            return image.getWidth(this);
        }
    }

    public int getHeight() {
        if (image == null) {
            return 0;
        } else {
            return image.getHeight(this);
        }
    }

    Image getImage() {
        return image;
    }

    byte[] getBytes() {
        return data;
    }

    void setBytes(byte b[]) {
        try {
            data=b;
            Toolkit toolkit = Toolkit.getDefaultToolkit();
            image = toolkit.createImage(b);
            MediaTracker tracker = new MediaTracker(this);
            tracker.addImage(image, 0);
            tracker.waitForAll();
            if (tracker.isErrorAny()) {
                image = null;
            }
        } catch (Exception e) {
            image = null;
        }
    }

    private void loadImage(String filename) {
        try {
            File f=new File(filename);
            int len=(int)f.length();
            byte b[]=new byte[len];
            FileInputStream fin=new FileInputStream(f);
            int offset=0;
            while(offset<len) {
                offset+=fin.read(b,offset,len-offset);
            }
            fin.close();
            setBytes(b);
        } catch(IOException e) {
            e.printStackTrace();
        }
    }
}
