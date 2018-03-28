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

public class GenerateAllKeys {

  private static String outdir;
  private static FileOutputStream out;

  public static void main(String[] args) throws Exception {
    outdir = args[0];

    System.out.println("Start RSA key generation");
    generateRSAKeys();
    System.out.println("RSA keys generated");

    System.out.println("Start RSA key generation");
    generateDSAKeys();
    System.out.println("DSA keys generated");

    System.out.println("Start AES key generation");
    generateAESKey();
    System.out.println("AES key generated");

  }

  private static void generateRSAKeys () throws Exception {
    SecureRandom sr = new SecureRandom();
    BigInteger pubExp = new BigInteger("10001", 16);
    RSAKeyGenerationParameters RSAKeyGenPara =
         new RSAKeyGenerationParameters(pubExp, sr, 1024, 80);
    RSAKeyPairGenerator RSAKeyPairGen = new RSAKeyPairGenerator();
    RSAKeyPairGen.init(RSAKeyGenPara);
    AsymmetricCipherKeyPair keyPair = RSAKeyPairGen.generateKeyPair();
 
    RSAPrivateCrtKeyParameters RSAprivKey = 
           (RSAPrivateCrtKeyParameters) keyPair.getPrivate();
    RSAKeyParameters RSApubKey = 
           (RSAKeyParameters) keyPair.getPublic();

    BigInteger mod = RSAprivKey.getModulus();
    out = new FileOutputStream(outdir + "RSAmod.dat");
    out.write(mod.toByteArray());
    out.flush();
    out.close();

    BigInteger privExp = RSAprivKey.getExponent();
    out = new FileOutputStream(outdir + "RSAprivExp.dat");
    out.write(privExp.toByteArray());
    out.flush();
    out.close();

    pubExp = RSAprivKey.getPublicExponent();
    if ( !pubExp.equals(new BigInteger("10001", 16)) ) 
      throw new Exception("public exponent does not match");
    out = new FileOutputStream(outdir + "RSApubExp.dat");
    out.write(pubExp.toByteArray());
    out.flush();
    out.close();

    BigInteger dp = RSAprivKey.getDP();
    out = new FileOutputStream(outdir + "RSAdp.dat");
    out.write(dp.toByteArray());
    out.flush();
    out.close();

    BigInteger dq = RSAprivKey.getDQ();
    out = new FileOutputStream(outdir + "RSAdq.dat");
    out.write(dq.toByteArray());
    out.flush();
    out.close();

    BigInteger p = RSAprivKey.getP();
    out = new FileOutputStream(outdir + "RSAp.dat");
    out.write(p.toByteArray());
    out.flush();
    out.close();

    BigInteger q = RSAprivKey.getQ();
    out = new FileOutputStream(outdir + "RSAq.dat");
    out.write(q.toByteArray());
    out.flush();
    out.close();

    BigInteger qInv = RSAprivKey.getQInv();
    out = new FileOutputStream(outdir + "RSAqInv.dat");
    out.write(qInv.toByteArray());
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
