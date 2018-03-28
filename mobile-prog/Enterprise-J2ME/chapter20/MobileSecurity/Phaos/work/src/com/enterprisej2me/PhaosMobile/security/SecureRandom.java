package com.enterprisej2me.PhaosMobile.security;

import com.phaos.micro.crypto.*;
import com.phaos.micro.crypto.digest.*;
import com.phaos.micro.crypto.pkcs1.*;
import com.phaos.micro.util.*;

public class SecureRandom extends java.util.Random {
  private static SecureRandom rand = new SecureRandom();
  private byte[] seed;

  private long counter = 1;
  private MessageDigest digest;
  private byte[] state;
//  private SHA1Digest  digest = new SHA1Digest();
//  private byte[] state = new byte[digest.getDigestSize()];
//  private byte[] state = new byte[digest.digestLength];

  public SecureRandom() {
    super(0);
    try {
      digest = MessageDigest.getInstance(AlgIDList.SHA1);
      state = new byte[digest.digestLength];
      setSeed(System.currentTimeMillis());
    } catch (Exception e) {
      System.out.println("Cannot init SecureRandom ");
      e.printStackTrace();
    }
  }

  public SecureRandom(byte[] inSeed) {
    try {
      digest = MessageDigest.getInstance(AlgIDList.SHA1);
      setSeed(inSeed);
    } catch (Exception e) {
      System.out.println("Cannot init SecureRandom ");
      e.printStackTrace();
    }
  }

  public static SecureRandom getInstance(String algorithm) {
    return new SecureRandom();
  }

  public static SecureRandom getInstance(String algorithm, String provider) {
    return new SecureRandom();
  }

  public static byte[] getSeed(int numBytes) {
    byte[] rv = new byte[numBytes];
    rand.setSeed(System.currentTimeMillis());
    rand.nextBytes(rv);
    return rv;
  }

  public byte[] generateSeed(int numBytes) {
    byte[] rv = new byte[numBytes];
    nextBytes(rv);	
    return rv;
  }

  public void setSeed(byte[] inSeed) {
    digest.update(inSeed, 0, inSeed.length);
  }

  public void nextBytes(byte[] bytes) {
    int stateOff = 0;
//    digest.doFinal(state, 0);
    digest.computeCurrent();
    state = digest.result;
    for (int i = 0; i != bytes.length; i++) {
      if (stateOff == state.length) {
        byte[]  b = longToBytes(counter++);
        digest.update(b, 0, b.length);
        digest.update(state, 0, state.length);
//        digest.doFinal(state, 0);
        digest.computeCurrent();
        state = digest.result;
        stateOff = 0;
      }
      bytes[i] = state[stateOff++];
    }
    byte[]  b = longToBytes(counter++);
    digest.update(b, 0, b.length);
    digest.update(state, 0, state.length);
  }

  public void setSeed(long rSeed) {
    if (rSeed != 0) {
      setSeed(longToBytes(rSeed));
    }
  }

  private byte[]  intBytes = new byte[4];

  public int nextInt() {
    nextBytes(intBytes);
    int result = 0;
    for (int i = 0; i < 4; i++) {
      result = (result << 8) + (intBytes[i] & 0xff);
    }
    return result;
  }

  protected final int next(int numBits) {
    int size = (numBits + 7) / 8;
    byte[]  bytes = new byte[size];
    nextBytes(bytes);
    int result = 0;
    for (int i = 0; i < size; i++) {
      result = (result << 8) + (bytes[i] & 0xff);
    }
    return result & ((1 << numBits) - 1);
  }

  private byte[]  longBytes = new byte[8];

  private byte[] longToBytes(long val) {
    for (int i = 0; i != 8; i++) {
      val >>>= (8 * i);
      longBytes[i] = (byte)val;
    }
    return longBytes;
  }
}

