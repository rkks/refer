package com.enterprisej2me.IAIKTest.utils;

import java.io.*;
import java.util.*;
import iaik.me.asn1.*;
import iaik.me.security.*;
import iaik.me.security.cipher.*;
import iaik.me.security.rsa.*;
import iaik.me.utils.*;

public class CryptoEngine {

  private byte [] salt;
  private byte [] AESkey;
  private byte [] AESinitV;

  private PrivateKey RSAprivKey;
  private PublicKey RSApubKey;

  public CryptoEngine () { 
    try { 
      Class c = this.getClass();
      InputStream is;

      SecureRandom sr = new SecureRandom();
      salt = new byte [16];
      sr.nextBytes(salt);
 
      is = c.getResourceAsStream("/keys/AESkey.dat");
      AESkey = readFromStream(is);
      is.close();
      is = c.getResourceAsStream("/keys/AESinitV.dat");
      AESinitV = readFromStream(is);
      is.close();

      byte [] asnArray;
      is = c.getResourceAsStream("/keys/RSApub.dat");
      asnArray = readFromStream(is);
      is.close();
      RSApubKey = new PublicKey(new ASN1(asnArray));

      is = c.getResourceAsStream("/keys/RSApriv.dat");
      asnArray = readFromStream(is);
      is.close();
      RSAprivKey = new PrivateKey(new ASN1(asnArray));
    
    } catch (Exception e) {
      e.printStackTrace();
    }
    return;
  }

  public void generateAESKey () throws Exception {
    SecureRandom sr = new SecureRandom();
    AESkey = new byte [16];
    sr.nextBytes(AESkey);
    AESinitV = new byte [16];
    sr.nextBytes(AESinitV);
  }

  public void generateRSAKeyPair () throws Exception { 
    RSAKeyPairGenerator rsaKeyPairGenerator = new RSAKeyPairGenerator();
    rsaKeyPairGenerator.initialize(1024, null, null);
    CryptoBag cryptoBag = rsaKeyPairGenerator.generateKeyPair();
 
    RSApubKey = (PublicKey) cryptoBag.getCryptoBag(cryptoBag.V_KEY_PUBLIC);
    RSAprivKey = (PrivateKey) cryptoBag.getCryptoBag(cryptoBag.V_KEY_PRIVATE);
    return;
  }

  public byte [] AESEncrypt (byte [] toEncrypt) throws Exception {
    CryptoBag cipherKey = CryptoBag.makeSecretKey(AESkey);
    CryptoBag ivparam = CryptoBag.makeIV(AESinitV);
    Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
    cipher.init(Cipher.ENCRYPT_MODE, cipherKey, ivparam, null);
    return cipher.doFinal(toEncrypt);
  }

  public byte [] AESDecrypt (byte [] toDecrypt) throws Exception {
    CryptoBag cipherKey = CryptoBag.makeSecretKey(AESkey);
    CryptoBag ivparam = CryptoBag.makeIV(AESinitV);
    Cipher cipher = Cipher.getInstance("AES/ECB/PKCS5Padding");
    cipher.init(Cipher.DECRYPT_MODE, cipherKey, ivparam, null);
    return cipher.doFinal(toDecrypt);
  }

  public byte [] PBEEncrypt (byte [] toEncrypt, char [] passwd) 
                                              throws Exception {
    // SecureRandom secRnd = new SecureRandom();
    PBE pbe = PBE.getInstance(PBE.OID_PKCS12_RC2_40_SHA);
    Cipher c = pbe.getCipher(Cipher.ENCRYPT_MODE, passwd, salt, 1024, null);
    return c.doFinal(toEncrypt);
  }

  public byte [] PBEDecrypt (byte [] toDecrypt, char [] passwd) 
                                              throws Exception {
    PBE pbe = PBE.getInstance(PBE.OID_PKCS12_RC2_40_SHA);
    Cipher c = pbe.getCipher(Cipher.DECRYPT_MODE, passwd, salt, 1024, null);
    return c.doFinal(toDecrypt);
  }

  // Public key encrypt using RSA
  public byte [] RSAEncrypt (byte [] toEncrypt) throws Exception {
    if (RSApubKey == null)
      throw new Exception("Generate RSA keys first!");

    Cipher rsa = Cipher.getInstance("RSA/ECB/PKCS1Padding");
    rsa.init(Cipher.ENCRYPT_MODE, RSApubKey, null, null);
    return rsa.doFinal(toEncrypt);
  }

  // private key decrypt
  public byte [] RSADecrypt (byte [] toDecrypt) throws Exception {
    if (RSAprivKey == null)
      throw new Exception("Generate RSA keys first!");

    Cipher rsa = Cipher.getInstance("RSA/ECB/PKCS1Padding");
    rsa.init(Cipher.DECRYPT_MODE, RSAprivKey, null, null);
    return rsa.doFinal(toDecrypt);
  }
  
  // RSA signature
  public byte [] RSASign (byte [] toSign) throws Exception {
    if (RSAprivKey == null)
      throw new Exception("Generate RSA keys first!");
    
    Signature sharsa = Signature.getInstance("SHA1withRSA");
    sharsa.initSign(RSAprivKey, null);
    sharsa.update(toSign);
    return sharsa.sign();
  }
  
  // RSA signature verification
  public boolean RSAVerify (byte [] mesg, byte [] sig) throws Exception {
    if (RSApubKey == null)
      throw new Exception("Generate RSA keys first!");
 
    Signature sharsa = Signature.getInstance("SHA1withRSA");
    sharsa.initVerify(RSApubKey);
    sharsa.update(mesg);
    return sharsa.verify(sig);
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
