import java.io.*;
import java.util.*;
import iaik.me.asn1.*;
import iaik.me.security.*;
import iaik.me.security.cipher.*;
import iaik.me.security.rsa.*;
import iaik.me.utils.*;
 
public class GenerateAllKeys {

  private static String outdir;
  private static FileOutputStream out;

  public static void main(String[] args) throws Exception {
    outdir = args[0];

    System.out.println("Start RSA key generation");
    generateRSAKeys();
    System.out.println("RSA keys generated");

    System.out.println("Start DSA key generation");
    generateDSAKeys();
    System.out.println("DSA keys generated");

    System.out.println("Start AES key generation");
    generateAESKey();
    System.out.println("AES key generated");

  }

  private static void generateRSAKeys () throws Exception {
    RSAKeyPairGenerator rsaKeyPairGenerator = new RSAKeyPairGenerator();
    rsaKeyPairGenerator.initialize(1024, null, null);
    CryptoBag cryptoBag = rsaKeyPairGenerator.generateKeyPair();

    PublicKey rsa_pub = 
      (PublicKey) cryptoBag.getCryptoBag(cryptoBag.V_KEY_PUBLIC);
    out = new FileOutputStream(outdir + "RSApub.dat");
    out.write(rsa_pub.getEncoded());
    out.flush();
    out.close();

    PrivateKey rsa_priv = 
      (PrivateKey) cryptoBag.getCryptoBag(cryptoBag.V_KEY_PRIVATE);
    out = new FileOutputStream(outdir + "RSApriv.dat");
    out.write(rsa_priv.getEncoded());
    out.flush();
    out.close();

    return;
  }

  private static void generateDSAKeys () throws Exception {
  }

  private static void generateAESKey () throws Exception {
    SecureRandom sr = new SecureRandom();
    byte [] AESkey = new byte [16];
    sr.nextBytes(AESkey);
    out = new FileOutputStream(outdir + "AESkey.dat");
    out.write(AESkey);
    out.flush();
    out.close();

    byte [] AESinitV = new byte [16];
    sr.nextBytes(AESinitV);
    out = new FileOutputStream(outdir + "AESinitV.dat");
    out.write(AESinitV);
    out.flush();
    out.close();
 
    return;
  }
}
