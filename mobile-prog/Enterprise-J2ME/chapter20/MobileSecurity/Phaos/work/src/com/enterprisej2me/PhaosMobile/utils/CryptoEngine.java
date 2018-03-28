package com.enterprisej2me.PhaosMobile.utils;

import com.phaos.micro.util.*;
import com.phaos.micro.asn1.*;
import com.phaos.micro.math.*;
import com.phaos.micro.crypto.*;
import com.phaos.micro.crypto.pkcs1.*;
import com.phaos.micro.crypto.pkcs5.*;
import com.phaos.micro.crypto.pkcs8.*;
import com.phaos.micro.crypto.pkcs12.*;
import com.phaos.micro.crypto.cipher.*;
import com.phaos.micro.crypto.dsa.*;
import com.phaos.micro.crypto.digest.*;
import com.phaos.micro.crypto.signature.*;
import java.io.*;
import com.enterprisej2me.PhaosMobile.security.*;

public class CryptoEngine {

  // To encrypt/decrypt a message using AES, we need both
  // the key and the init vector for the cipher
  //
  // 192 bit AES key
  private SymmetricKey AESkey;
  // CBC cipher init vector
  private byte [] AESinitV;

  // AES algorithm ID with embedded cipher init vector
  // private AlgorithmIdentifier AESalgID;

  // 1024 bit RSA keys
  private RSAPublicKey RSApubKey;
  private RSAPrivateKey RSAprivKey;

  // To generate a signature with DSA, we need a private key.
  // We send the signature together with serialized public key data.
  //
  // On the verifying end, the client builds a DSA public
  // key from the signature (algID) and the public key data.
  // Then it uses the public key to verify the signature.
  //
  // 1024 bit DSA key
  private DSAPrivateKey DSAprivKey;
  // This is the DSA public key data you can serialize
  private byte [] DSApubKeyDer;

  // salt to mix in with user supplied password in 
  // passwd based schemes
  private byte [] salt;

  public CryptoEngine () {
    // Default keys are null
    AESkey = null;
    AESinitV = null;
    RSAprivKey = null;
    RSApubKey = null;
    DSAprivKey = null;
    DSApubKeyDer = null;

    // try to populate pre-generated keys from resources
    try {
      Class c = this.getClass();
      InputStream is;

      // The AES init vector
      is = c.getResourceAsStream("/keys/AESinitV.der");
      ByteArrayOutputStream baos = new ByteArrayOutputStream();
      byte [] b = new byte[1];
      while ( is.read(b) != -1 ) {
         baos.write(b);
      }
      is.close();
      AESinitV = baos.toByteArray();
      baos.close();

      // The AES key
      is = c.getResourceAsStream("/keys/AESkey.der");
      baos = new ByteArrayOutputStream();
      b = new byte[1];
      while ( is.read(b) != -1 ) {
         baos.write(b);
      }
      is.close();
      byte[] keyMaterial = baos.toByteArray();
      baos.close();
      AESkey = new SymmetricKey(keyMaterial, 0, keyMaterial.length);

      // The RSA key pair
      is = c.getResourceAsStream("/keys/RSApubKey.der");
      RSApubKey = new RSAPublicKey(is);
      is.close();
      is = c.getResourceAsStream("/keys/RSAprivKey.der");
      RSAprivKey = new RSAPrivateKey(is);
      is.close();

      // The DSA private key
      is = c.getResourceAsStream("/keys/DSAparams.der");
      DSAParams params = new DSAParams(is);
      AlgorithmIdentifier algID = getDSAalgID(params);
      is.close();
      is = c.getResourceAsStream("/keys/DSAprivKey.der");
      DSAprivKey = new DSAPrivateKey(algID, is);
      is.close();

      // The DSA public key byte array
      is = c.getResourceAsStream("/keys/DSApubKey.der");
      baos = new ByteArrayOutputStream();
      b = new byte[1];
      while ( is.read(b) != -1 ) {
         baos.write(b);
      }
      is.close();
      DSApubKeyDer = baos.toByteArray();
      baos.close();

    } catch (Exception e) {
      System.out.println("Problem with pre-generated keys.");
      e.printStackTrace();
    }

    RandomBitsSource.setDefault(new SecureRBS());
    RandomBitsSource.getDefault().seed();

    salt = new byte[PBES2AlgID.DEFAULT_SALT_LENGTH];
    RandomBitsSource.getDefault().randomBytes(salt);

    return;
  }

  // Get the algID from DSA's key parameters.
  //
  // The key parameter can come from the private key, signed stream
  // or a pre-generated DER file.
  private AlgorithmIdentifier getDSAalgID (DSAParams params) throws Exception {
    ByteArrayOutputStream paramsOut = new ByteArrayOutputStream();
    params.output(paramsOut);
    paramsOut.close();
    return new AlgorithmIdentifier(OIDList.DSA, paramsOut.toByteArray());
  }

  public void generateAESKey () throws Exception {
    AlgorithmIdentifier algID = AlgIDList.AES_128_CBC;
    SymmetricKeyGenerator generator = 
           new SymmetricKeyGenerator(algID, 
                             RandomBitsSource.getDefault());
    AESkey = generator.generateKey();

    // Create a new cipher, initialize it and then get
    // CBC init vector.
    Cipher cipher = BlockCipher.getInstance(algID, AESkey, 
                                     BlockCipher.PADDING_PKCS5);
    CBCAlgID AESalgID = (CBCAlgID) cipher.createAlgID();
    AESinitV = AESalgID.iv;
    return;
  }

  public void generateRSAKeyPair () throws Exception {
    RSAKeyGenParams params = new RSAKeyGenParams(1024);
    KeyPairGenerator kpg = KeyPairGenerator.getInstance(OIDList.RSA_ENCRYPTION, params);
    KeyPair kp = kpg.generateKeyPair();
    RSApubKey = (RSAPublicKey)kp.publicKey;
    RSAprivKey = (RSAPrivateKey)kp.privateKey;
    return;
  }

  public void generateDSAKeyPair () throws Exception {
    DSAKeyGenParams params = new DSAKeyGenParams(1024, RandomBitsSource.getDefault());
    KeyPairGenerator kpg = KeyPairGenerator.getInstance(OIDList.DSA, params);
    KeyPair kp = kpg.generateKeyPair();

    DSAprivKey = (DSAPrivateKey)kp.privateKey;

    // DSA public DER is sent out with the signature.
    ByteArrayOutputStream baos = new ByteArrayOutputStream ();
    ((DSAPublicKey)kp.publicKey).output(baos);
    DSApubKeyDer = baos.toByteArray();

    return;
  }

  public byte [] AESEncrypt (byte [] toEncrypt) throws Exception {
    if ( AESkey == null || AESinitV == null ) 
      throw new Exception("Generate AES key first!");
    CBCAlgID AESalgID = new CBCAlgID(OIDList.AES_128_CBC, AESinitV);
    Cipher cipher = BlockCipher.getInstance(AESalgID, AESkey, 
                                     BlockCipher.PADDING_PKCS5);
    PooledArray ciphertext = 
        ((BlockCipher)cipher).encryptFinal(toEncrypt, 0, toEncrypt.length);
    return ciphertext.toByteArray(true);
  }

  public byte [] AESDecrypt (byte [] toDecrypt) throws Exception {
    if ( AESkey == null ) 
      throw new Exception("Generate AES key first!");
    CBCAlgID AESalgID = new CBCAlgID(OIDList.AES_128_CBC, AESinitV);
    Cipher cipher = BlockCipher.getInstance(AESalgID, AESkey, 
                                   BlockCipher.PADDING_PKCS5);
    PooledArray plaintext = 
       ((BlockCipher)cipher).decryptFinal(toDecrypt, 0, toDecrypt.length);
    return plaintext.toByteArray(true);
  }

  // PKCS #5 password scheme 2 using AES as underlying cipher
  public byte [] AESPasswdEncrypt(byte [] toEncrypt, String password) 
                                                   throws Exception {
    // Use SHA1 with AES
    AlgorithmIdentifier algID = new PBES2AlgID(salt,
                                 PBES2AlgID.DEFAULT_ITERATION_COUNT,
                                 PBES2AlgID.KEY_LENGTH_NOT_PRESENT,
                                 AlgIDList.HMAC_WITH_SHA1,
                           new CBCAlgID(OIDList.AES_128_CBC, AESinitV));
    ByteArrayInputStream in = new ByteArrayInputStream(toEncrypt);
    PooledArray plaintext = ByteArrayPool.getArray(in.available());
    in.read(plaintext.buffer, 0, plaintext.length);
    in.close();

    // PKCS #5 only uses lower 8 bit of each password char
    PasswordBasedEncryptionScheme pbes =
              PasswordBasedEncryptionScheme.getInstance(algID, 
                               password.getBytes());
    PooledArray ciphertext = 
       pbes.encryptFinal(plaintext.buffer, 0, plaintext.length);

    ByteArrayOutputStream out = new ByteArrayOutputStream ();
    algID.output(out);
    out.write(ciphertext.buffer, 0, ciphertext.length);
    byte [] encrypted = out.toByteArray();
    out.close();
 
    plaintext.release();
    ciphertext.release();
    return encrypted;
  }

  // PKCS #5 password scheme 2 decryption
  // 
  // Since the algID is embedded inside the encrypted 
  // byte array, this method can decrypt any Phaos PBES
  // messages with any underlying ciphers.
  public byte [] AESPasswdDecrypt(byte [] toDecrypt, String password) 
                                                   throws Exception {
    ByteArrayInputStream in = new ByteArrayInputStream(toDecrypt);
    AlgorithmIdentifier algID = new AlgorithmIdentifier(in);
    PooledArray ciphertext = ByteArrayPool.getArray(in.available());
    in.read(ciphertext.buffer, 0, ciphertext.length);
    in.close();

    PasswordBasedEncryptionScheme pbes =
          PasswordBasedEncryptionScheme.getInstance(algID, 
                password.getBytes());
    PooledArray plaintext = 
        pbes.decryptFinal(ciphertext.buffer, 0, ciphertext.length);

    ByteArrayOutputStream out = new ByteArrayOutputStream ();
    out.write(plaintext.buffer, 0, plaintext.length);
    byte [] decrypted = out.toByteArray();
    out.close();
 
    plaintext.release();
    ciphertext.release();
    return decrypted;
  }

  public byte [] RSAEncrypt (byte [] toEncrypt) throws Exception {
    if (RSApubKey == null) 
      throw new Exception("Generate RSA keys first!");

    byte [] encrypted;

    PooledArray plaintext = ByteArrayPool.getArray(toEncrypt);
    AlgorithmIdentifier algID = new OAEPAlgID();
    Cipher cipher = Cipher.getInstance(algID, RSApubKey);
    PooledArray ciphertext = cipher.encrypt(plaintext.buffer, 0, plaintext.length);

    ByteArrayOutputStream out = new ByteArrayOutputStream ();
    algID.output(out);
    out.write(ciphertext.buffer, 0, ciphertext.length);
    encrypted = out.toByteArray();
    out.close();    

    plaintext.release();
    ciphertext.release();
    return encrypted;
  }

  public byte [] RSADecrypt (byte [] toDecrypt) throws Exception {
    if (RSAprivKey == null) 
      throw new Exception("Generate RSA keys first!");

    byte [] decrypted;

    ByteArrayInputStream in = new ByteArrayInputStream(toDecrypt);
    AlgorithmIdentifier algID = new AlgorithmIdentifier(in);
    PooledArray ciphertext = ByteArrayPool.getArray(in.available());
    in.read(ciphertext.buffer, 0, ciphertext.length);
    in.close();

    Cipher cipher = Cipher.getInstance(algID, RSAprivKey);
    PooledArray plaintext = cipher.decrypt(ciphertext.buffer, 0, ciphertext.length);
    decrypted = plaintext.toByteArray();

    plaintext.release();
    ciphertext.release();
    return decrypted; 
  }

  public byte [] DSASign (byte [] toSign) throws Exception {
    if (DSAprivKey == null) 
      throw new Exception("Generate DSA keys first!");
    Signature signature = Signature.getInstance(AlgIDList.SHA1_WITH_DSA, 
                                                DSAprivKey);
    byte [] result = signature.sign(toSign, 0, toSign.length).toByteArray(true);

    DSAParams params = new DSAParams(new ByteArrayInputStream(DSAprivKey.createAlgID(true).encodeParameters()));
    AlgorithmIdentifier algID = getDSAalgID (params);

    ByteArrayOutputStream baos = new ByteArrayOutputStream ();
    algID.output(baos);
    baos.write(result, 0, result.length);
    baos.flush();
    baos.close();

    return baos.toByteArray();
  }

  // Use the signature and DSApubKeyDer to construct the 
  // public key to use.
  public boolean DSAVerify (byte [] mesg, byte [] sig) throws Exception {
    InputStream is = new ByteArrayInputStream(sig);
    AlgorithmIdentifier algID = new AlgorithmIdentifier(is);
    PooledArray sigBytes = ByteArrayPool.getArray(is.available());
    is.read(sigBytes.buffer, 0, sigBytes.length);
    is.close();

    DSAPublicKey DSApubKey = new DSAPublicKey(algID, 
                                 new ByteArrayInputStream(DSApubKeyDer));

    Signature signature = Signature.getInstance(AlgIDList.SHA1_WITH_DSA, 
                                                DSApubKey);

    return signature.verify(mesg, 0, mesg.length, 
                            sigBytes.buffer, 0, sigBytes.length);
  }

  public byte [] PKCS8EncryptRSAKey (RSAPrivateKey privateKey,
                                     String passwd) throws Exception {

    byte[] password = PKCS12Utils.passwordStringToPKCS12(passwd);
    AlgorithmIdentifier algID = 
           new PBES2AlgID(new CBCAlgID(OIDList.AES_128_CBC, AESinitV));
    PrivateKeyInfo pki = 
           new PrivateKeyInfo(AlgIDList.RSA_ENCRYPTION, privateKey);
    EncryptedPrivateKeyInfo epki = 
           new EncryptedPrivateKeyInfo(pki, password, algID);

    ByteArrayOutputStream baos = new ByteArrayOutputStream ();
    epki.output(baos);
    baos.flush();
    baos.close();
    return baos.toByteArray();
  }

  public RSAPrivateKey PKCS8DecryptRSAKey (byte [] toDecrypt, String passwd)
                                               throws Exception {

    byte[] password = PKCS12Utils.passwordStringToPKCS12(passwd);

    InputStream is = new ByteArrayInputStream(toDecrypt);
    EncryptedPrivateKeyInfo epki = 
             new EncryptedPrivateKeyInfo(password, is);
    is.close();

    PrivateKeyInfo pki = epki.privateKeyInfo;
    RSAPrivateKey key = (RSAPrivateKey)pki.privateKey;
    return key;
  }
}
