package com.enterprisej2me.PhaosMobile.security;

import com.phaos.micro.crypto.*;

public class SecureRBS extends RandomBitsSource {

  private SecureRandom sr;

  public SecureRBS() {
    sr = new SecureRandom ();
  }

  public boolean isSecure () {
    // We assume that the SecureRandom wrapped in this class
    // is a secure random source. 
    //
    // It is probably the case if it is provided by device vendors.
    return true;
  }

  public void seed () {
    sr.setSeed(System.currentTimeMillis());
  }

  public void seed (byte[] seed) {
    sr.setSeed (seed);
  }

  public byte[] randomBytes(byte[] bytes, int off, int len) {
    byte [] rBytes = new byte[len];
    sr.nextBytes (rBytes);
    for (int i=0; i<len; i++) {
      bytes[off+i] = rBytes[i];
    }
    // return the input array. -- Phaos tech support
    return bytes;
  }

}

