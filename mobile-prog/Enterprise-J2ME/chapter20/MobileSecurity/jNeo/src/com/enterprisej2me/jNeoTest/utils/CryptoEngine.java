package com.enterprisej2me.jNeoTest.utils; 

import java.io.*;
import javax.microedition.io.*;

import com.ntru.jNeo.*;

public class CryptoEngine {  

  // AES init vector and 128 bit key
  private byte [] AESinitV;
  private RijndaelKey AESkey;

  // NTRU encryption key pair
  private EncKeys NTRUencKeys;

  // NTRU signature key pair
  private SgnKeys NTRUsgnKeys;

  private RandomNumber rn;
  private Context ctx;

  public CryptoEngine () { 
    try {
      rn = new RandomNumber(NTRUConst.NTRU_SHA1_HASH);
      ctx = new Context(rn);

      Class c = this.getClass();
      InputStream is;
 
      is = c.getResourceAsStream("/keys/AESkeydata.dat");
      byte [] keydata = readFromStream(is);
      AESkey = new RijndaelKey(keydata, RijndaelKey.NTRU_SYM_KEYSTRENGTH_128);
      is.close();

      is = c.getResourceAsStream("/keys/AESinitV.dat");
      AESinitV = readFromStream(is);
      is.close();

      is = c.getResourceAsStream("/keys/SgnPubKey.dat");
      byte [] sgnPubKeyData = readFromStream(is);
      is.close();
      is = c.getResourceAsStream("/keys/SgnPrivKey.dat");
      byte [] sgnPrivKeyData = readFromStream(is);
      is.close();
      NTRUsgnKeys = new SgnKeys (sgnPubKeyData, 0, sgnPubKeyData.length,
                                 sgnPrivKeyData, 0, sgnPrivKeyData.length);

      is = c.getResourceAsStream("/keys/EncPubKey.dat");
      byte [] encPubKeyData = readFromStream(is);
      is.close();
      is = c.getResourceAsStream("/keys/EncPrivKey.dat");
      byte [] encPrivKeyData = readFromStream(is);
      is.close();
      NTRUencKeys = new EncKeys (encPubKeyData, 0, encPubKeyData.length,
                                 encPrivKeyData, 0, encPrivKeyData.length,
                                 true);

    } catch (Exception e) {
      System.out.println("Error in init");
    }
  }

  public void generateAESKey () throws Exception {
    AESinitV = new byte [RijndaelKey.BLOCK_SIZE];
    rn.getRandom(AESinitV, 0, AESinitV.length);

    byte [] keydata = new byte [RijndaelKey.BLOCK_SIZE];
    rn.getRandom(keydata, 0, keydata.length);
    AESkey = new RijndaelKey(keydata, RijndaelKey.NTRU_SYM_KEYSTRENGTH_128);

    return;
  }

  // toEncrypt array length must be a multiple of 16 bytes
  public byte [] AESEncrypt (byte [] toEncrypt) throws Exception {
    if ( AESkey == null || AESinitV == null )
      throw new Exception("Generate AES key first!");
    if ( toEncrypt.length % 16 != 0 )
      throw new Exception("toEncrypt must have length multiple of 16 bytes");

    int len = AESkey.ciphertextLength(RijndaelKey.NTRU_ENC_RIJNDAEL,
                                      RijndaelKey.NTRU_SYM_MODE_CBC,
                                      RijndaelKey.NTRU_SYM_KEYSTRENGTH_128,
                                      toEncrypt.length, false);
    byte [] result = new byte [len];
    AESkey.encrypt(toEncrypt, 0, toEncrypt.length, result, 0, 
                   RijndaelKey.NTRU_SYM_MODE_CBC, AESinitV, 0, false);
    return result;
  }

  public byte [] AESDecrypt (byte [] toDecrypt) throws Exception {
    if ( AESkey == null || AESinitV == null )
      throw new Exception("Generate AES key first!");

    int len = AESkey.plaintextLength(RijndaelKey.NTRU_ENC_RIJNDAEL,
                                     RijndaelKey.NTRU_SYM_MODE_CBC,
                                     RijndaelKey.NTRU_SYM_KEYSTRENGTH_128,
                                     toDecrypt.length, false);
    byte [] result = new byte [len];
    AESkey.decrypt(toDecrypt, 0, toDecrypt.length, result, 0, 
                   RijndaelKey.NTRU_SYM_MODE_CBC, AESinitV, 0, false);
    return result;
  }

  public void generateNTRUencKeys () throws Exception {
    NTRUencKeys = new EncKeys(ctx, NTRUConst.NTRU_KEYSTRENGTH_251,
                              NTRUConst.NTRU_SHA1_HASH);
    return;
  }

  public byte [] NTRUEncrypt (byte [] toEncrypt) throws Exception {
    if ( NTRUencKeys == null )
      throw new Exception("Generate NTRU encryption keys first!");

    int cipherBlockSize = 
           NTRUencKeys.ciphertextSize(NTRUConst.NTRU_KEYSTRENGTH_251);
    int plainBlockSize = 
           NTRUencKeys.blockSize(NTRUConst.NTRU_KEYSTRENGTH_251);

    byte [] cipherBlock = new byte [cipherBlockSize];
    byte [] plainBlock = new byte [plainBlockSize];
    int psize;
    ByteArrayInputStream bais = new ByteArrayInputStream(toEncrypt);
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    while ( (psize = bais.read(plainBlock)) != -1 ) {
      // resize the last plain text block
      if ( psize < plainBlockSize ) {
        byte tmp [] = new byte [psize];
        for (int i = 0; i < psize; i++) {
          tmp[i] = plainBlock[i];
        }
        plainBlock = tmp;
      }

      NTRUencKeys.blockEncrypt(ctx, plainBlock, 0, 
                               plainBlock.length,
                               cipherBlock, 0);
      baos.write(cipherBlock);
    }
    baos.flush();
    byte [] result = baos.toByteArray();
    baos.close();
    bais.close();
    return result;
  }

  public byte [] NTRUDecrypt (byte [] toDecrypt) throws Exception {
    if ( NTRUencKeys == null )
      throw new Exception("Generate NTRU encryption keys first!");
 
    int cipherBlockSize = 
           NTRUencKeys.ciphertextSize(NTRUConst.NTRU_KEYSTRENGTH_251);
    int plainBlockSize = 
           NTRUencKeys.blockSize(NTRUConst.NTRU_KEYSTRENGTH_251);

    byte [] cipherBlock = new byte [cipherBlockSize];
    byte [] plainBlock = new byte [plainBlockSize];
    int psize;
    ByteArrayInputStream bais = new ByteArrayInputStream(toDecrypt);
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    while ( bais.read(cipherBlock) != -1 ) {
      psize = NTRUencKeys.blockDecrypt(ctx, cipherBlock, 
                                       0, cipherBlock.length,
                                       plainBlock, 0);
      // resize the last plain text block
      if ( psize < plainBlockSize ) {
        byte tmp [] = new byte [psize];
        for (int i = 0; i < psize; i++) {
          tmp[i] = plainBlock[i];
        }
        plainBlock = tmp;
      }
      baos.write(plainBlock);
    }
    baos.flush();
    byte [] result = baos.toByteArray();
    baos.close();
    bais.close();
    return result;
  }

  public void generateNTRUsgnKeys () throws Exception {
    NTRUsgnKeys = new SgnKeys(ctx, NTRUConst.NTRU_KEYSTRENGTH_251,
                              NTRUConst.NTRU_SHA1_HASH);
    return;
  }

  public byte [] NTRUSign (byte [] message) throws Exception {
    if ( NTRUsgnKeys == null )
      throw new Exception("Generate NTRU encryption keys first!");

    MessageDigest dig = new MessageDigest(NTRUConst.NTRU_SHA160_HASH);
    Signature sig = new Signature(NTRUConst.NTRU_KEYSTRENGTH_251,
                                  NTRUConst.NTRU_SHA160_HASH);
    dig.updateMessageDigest(message, 0, message.length);
    dig.completeMessageDigest();
    sig.sign(ctx, NTRUsgnKeys, dig);
    return sig.export();
  }
 
  public boolean NTRUVerify (byte [] message, byte [] sigData) 
                              throws Exception {
 
    Signature sig = new Signature(sigData, 0, sigData.length);
    MessageDigest dig = new MessageDigest(sig.getHashAlg());
    dig.updateMessageDigest(message, 0, message.length);
    dig.completeMessageDigest();
    
    try {
      sig.verify(ctx, NTRUsgnKeys, dig);
      return true;
    } catch (NTRUException e) {
      return false;
    }
  }

  private byte [] readFromStream (InputStream is) throws Exception {
    ByteArrayOutputStream baos = new ByteArrayOutputStream();
    byte [] b = new byte[1];
    while ( is.read(b) != -1 ) {
       baos.write(b);
    }
    byte[] result = baos.toByteArray();
    baos.close();
    return result;
  }

}
