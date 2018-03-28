package com.enterprisej2me.BouncyCastleTest.utils;

import java.io.*;
import java.math.*;
import java.security.*;

import org.bouncycastle.crypto.*;
import org.bouncycastle.crypto.params.*;
import org.bouncycastle.crypto.engines.*;
import org.bouncycastle.crypto.encodings.*;
import org.bouncycastle.crypto.generators.*;
import org.bouncycastle.crypto.modes.*;
import org.bouncycastle.crypto.paddings.*;
import org.bouncycastle.crypto.digests.*;
import org.bouncycastle.crypto.signers.*;
 
public class CryptoEngine {

  private byte [] AESkey;
  private byte [] AESinitV;

  private byte [] salt;

  private RSAKeyParameters RSAprivKey;
  private RSAKeyParameters RSApubKey;

  public CryptoEngine () { 
    try { 
      Class c = this.getClass();
      InputStream is;
 
      is = c.getResourceAsStream("/keys/AESkey.dat");
      AESkey = readFromStream(is);
      is.close();
      is = c.getResourceAsStream("/keys/AESinitV.dat");
      AESinitV = readFromStream(is);
      is.close();

      SecureRandom sr = new SecureRandom();
      salt = new byte [16];
      sr.nextBytes(salt);

      is = c.getResourceAsStream("/keys/RSAmod.dat");
      BigInteger RSAmod = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAprivExp.dat");
      BigInteger RSAprivExp = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSApubExp.dat");
      BigInteger RSApubExp = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAdp.dat");
      BigInteger RSAdp = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAdq.dat");
      BigInteger RSAdq = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAp.dat");
      BigInteger RSAp = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAq.dat");
      BigInteger RSAq = new BigInteger(readFromStream(is));
      is.close();
      is = c.getResourceAsStream("/keys/RSAqInv.dat");
      BigInteger RSAqInv = new BigInteger(readFromStream(is));
      is.close();
    
      RSAprivKey = new RSAPrivateCrtKeyParameters(RSAmod, RSApubExp,
                        RSAprivExp, RSAp, RSAq, RSAdp, RSAdq, RSAqInv);
      RSApubKey = new RSAKeyParameters(false, RSAmod, RSApubExp);
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
    SecureRandom sr = new SecureRandom();
    BigInteger pubExp = new BigInteger("10001", 16);
    RSAKeyGenerationParameters RSAKeyGenPara = 
         new RSAKeyGenerationParameters(pubExp, sr, 1024, 80);
    RSAKeyPairGenerator RSAKeyPairGen = new RSAKeyPairGenerator();
    RSAKeyPairGen.init(RSAKeyGenPara);
    AsymmetricCipherKeyPair keyPair = RSAKeyPairGen.generateKeyPair();
 
    RSAprivKey = (RSAPrivateCrtKeyParameters) keyPair.getPrivate();
    RSApubKey = (RSAKeyParameters) keyPair.getPublic();
  }

  public byte [] AESLiteEncrypt (byte [] toEncrypt) throws Exception {
    BufferedBlockCipher cipher = 
             new PaddedBufferedBlockCipher( 
               new CBCBlockCipher(new AESLightEngine()));
    // If initV is not given, the program will assume all zeros
    ParametersWithIV piv = new ParametersWithIV (
                      (new KeyParameter(AESkey)), AESinitV);
    cipher.init(true, piv);
    byte[] result = new byte[cipher.getOutputSize(toEncrypt.length)];
    int len = cipher.processBytes(toEncrypt, 0, toEncrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  public byte [] AESLiteDecrypt (byte [] toDecrypt) throws Exception {
    BufferedBlockCipher cipher = 
             new PaddedBufferedBlockCipher( 
               new CBCBlockCipher(new AESLightEngine()));
    ParametersWithIV piv = new ParametersWithIV (
                      (new KeyParameter(AESkey)), AESinitV);
    cipher.init(false, piv);
    byte[] result = new byte[cipher.getOutputSize(toDecrypt.length)];
    int len = cipher.processBytes(toDecrypt, 0, toDecrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  public byte [] AESFastEncrypt (byte [] toEncrypt) throws Exception {
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new AESFastEngine()));
    // If initV is not given, the program will assume all zeros
    ParametersWithIV piv = new ParametersWithIV (
                      (new KeyParameter(AESkey)), AESinitV);
    cipher.init(true, piv);
    byte[] result = new byte[cipher.getOutputSize(toEncrypt.length)];
    int len = cipher.processBytes(toEncrypt, 0, toEncrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  public byte [] AESFastDecrypt (byte [] toDecrypt) throws Exception {
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new AESFastEngine()));
    ParametersWithIV piv = new ParametersWithIV (
                      (new KeyParameter(AESkey)), AESinitV);
    cipher.init(false, piv);
    byte[] result = new byte[cipher.getOutputSize(toDecrypt.length)];
    int len = cipher.processBytes(toDecrypt, 0, toDecrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  // Get use password to generate symmetric key with (or without IV)
  // To be used in an AES underlying cipher
  private CipherParameters getAESPasswdKey (char [] passwd) throws Exception {
    PBEParametersGenerator  generator = 
        new PKCS12ParametersGenerator(new SHA1Digest());
    generator.init(
        PBEParametersGenerator.PKCS12PasswordToBytes(passwd),
        salt, 1024);
    // Generate a 128 bit key w/ 128 bit IV
    ParametersWithIV key = 
      (ParametersWithIV)generator.generateDerivedParameters(128, 128);
    // Generate a 128 kit key
    // CipherParameters key = generator.generateDerivedParameters(128);
    return key;
  }

  // Password based encryption using AES
  public byte [] AESPasswdEncrypt (byte [] toEncrypt, char [] passwd)
                                       throws Exception {
    ParametersWithIV key = (ParametersWithIV) getAESPasswdKey(passwd);
    // The following code uses an AES cipher to
    // encrypt the message
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new AESFastEngine()));
    cipher.init(true, key);
    byte[] result = new byte[cipher.getOutputSize(toEncrypt.length)];
    int len = cipher.processBytes(toEncrypt, 0, toEncrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  // Password based decryption using AES
  public byte [] AESPasswdDecrypt (byte [] toDecrypt, char [] passwd)
                                       throws Exception {
    ParametersWithIV key = (ParametersWithIV) getAESPasswdKey(passwd);
    // The following code uses an AES cipher to
    // decrypt the message
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new AESFastEngine()));
    cipher.init(false, key);
    byte[] result = new byte[cipher.getOutputSize(toDecrypt.length)];
    int len = cipher.processBytes(toDecrypt, 0, toDecrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  // Get use password to generate a triple DES symmetric key
  private DESedeParameters getDESedePasswdKey (char [] passwd)
                                              throws Exception {
    PBEParametersGenerator  generator =
        new PKCS12ParametersGenerator(new SHA1Digest());
    generator.init(
        PBEParametersGenerator.PKCS12PasswordToBytes(passwd),
        salt, 1);
    KeyParameter rawKey =
      (KeyParameter) generator.generateDerivedParameters(128);
    byte [] keyBytes = rawKey.getKey();
    DESedeParameters.setOddParity(keyBytes);
    DESedeParameters key = new DESedeParameters(keyBytes);
    return key;
  }

  // Password based encryption using DESede
  public byte [] DESedePasswdEncrypt (byte [] toEncrypt, char [] passwd)
                                       throws Exception {
    DESedeParameters key = (DESedeParameters) getDESedePasswdKey(passwd);
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new DESedeEngine()));
    cipher.init(true, key);
    byte[] result = new byte[cipher.getOutputSize(toEncrypt.length)];
    int len = cipher.processBytes(toEncrypt, 0, toEncrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  // Password based decryption using DESede
  public byte [] DESedePasswdDecrypt (byte [] toDecrypt, char [] passwd)
                                       throws Exception {
    DESedeParameters key = (DESedeParameters) getDESedePasswdKey(passwd);
    BufferedBlockCipher cipher =
             new PaddedBufferedBlockCipher(
               new CBCBlockCipher(new DESedeEngine()));
    cipher.init(false, key);
    byte[] result = new byte[cipher.getOutputSize(toDecrypt.length)];
    int len = cipher.processBytes(toDecrypt, 0, toDecrypt.length, result, 0);
    try {
      cipher.doFinal(result, len);
    } catch (CryptoException ce) {
      result = (new String("Cipher error")).getBytes();
      ce.printStackTrace();
    }
    return result;
  }

  // Public key encrypt using RSA
  public byte [] RSAEncrypt (byte [] toEncrypt) throws Exception {
    if (RSApubKey == null)
      throw new Exception("Generate RSA keys first!");
 
    AsymmetricBlockCipher eng = new RSAEngine();
    eng = new PKCS1Encoding(eng);
    eng.init(true, RSApubKey);
    return eng.processBlock(toEncrypt, 0, toEncrypt.length);
  }

  // private key decrypt
  public byte [] RSADecrypt (byte [] toDecrypt) throws Exception {
    if (RSAprivKey == null)
      throw new Exception("Generate RSA keys first!");

    AsymmetricBlockCipher eng = new RSAEngine();
    eng = new PKCS1Encoding(eng);
    eng.init(false, RSAprivKey);
    return eng.processBlock(toDecrypt, 0, toDecrypt.length);
  }
  
  // RSA signature
  public byte [] RSASign (byte [] toSign) throws Exception {
    if (RSAprivKey == null)
      throw new Exception("Generate RSA keys first!");
    SHA1Digest dig = new SHA1Digest();
    RSAEngine eng = new RSAEngine();
    
    PSSSigner signer = new PSSSigner(eng, dig, 64);
    signer.init(true, RSAprivKey);
    signer.update(toSign, 0, toSign.length);
    return signer.generateSignature();
  }
  
  // RSA signature verification
  public boolean RSAVerify (byte [] mesg, byte [] sig) throws Exception {
    if (RSApubKey == null)
      throw new Exception("Generate RSA keys first!");
    SHA1Digest dig = new SHA1Digest();
    RSAEngine eng = new RSAEngine();
    
    PSSSigner signer = new PSSSigner(eng, dig, 64);
    signer.init(false, RSApubKey);
    signer.update(mesg, 0, mesg.length);
    return signer.verifySignature(sig);
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
