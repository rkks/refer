import com.phaos.micro.util.*;
import com.phaos.micro.asn1.*;
import com.phaos.micro.math.*;
import com.phaos.micro.crypto.*;
import com.phaos.micro.crypto.pkcs1.*;
import com.phaos.micro.crypto.cipher.*;
import com.phaos.micro.crypto.dsa.*;
import com.phaos.micro.crypto.digest.*;
import com.phaos.micro.crypto.signature.*;
import java.io.*;

public class GenerateAllKeys {

  private static String outdir;

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
    RSAKeyGenParams params = new RSAKeyGenParams(1024);
    KeyPairGenerator kpg = KeyPairGenerator.getInstance(OIDList.RSA_ENCRYPTION, params);
    KeyPair kp = kpg.generateKeyPair();
    RSAPublicKey pubKey = (RSAPublicKey)kp.publicKey;
    RSAPrivateKey privKey = (RSAPrivateKey)kp.privateKey;
    pubKey.output(new FileOutputStream(outdir + "RSApubKey.der"));
    privKey.output(new FileOutputStream(outdir + "RSAprivKey.der"));
    return;
  }

  private static void generateDSAKeys () throws Exception {
    DSAKeyGenParams params = new DSAKeyGenParams(1024, RandomBitsSource.getDefault());
    KeyPairGenerator kpg = KeyPairGenerator.getInstance(OIDList.DSA, params);
    KeyPair kp = kpg.generateKeyPair();
    DSAPublicKey pubKey = (DSAPublicKey)kp.publicKey;
    DSAPrivateKey privKey = (DSAPrivateKey)kp.privateKey;
    pubKey.output(new FileOutputStream(outdir + "DSApubKey.der"));
    privKey.output(new FileOutputStream(outdir + "DSAprivKey.der"));
    DSAParams dsaParams = new DSAParams(new ByteArrayInputStream(privKey.createAlgID(true).encodeParameters()));
    dsaParams.output(new FileOutputStream(outdir + "DSAparams.der"));
    return;
  }

  private static void generateAESKey () throws Exception {
    AlgorithmIdentifier algID = AlgIDList.AES_128_CBC;
    SymmetricKeyGenerator generator =
           new SymmetricKeyGenerator(algID,
                             RandomBitsSource.getDefault());
    SymmetricKey AESkey = generator.generateKey();
    FileOutputStream out = new FileOutputStream(outdir + "AESkey.der");
    out.write(AESkey.keyMaterial);
    out.flush();
    out.close();
 
    // Create a new cipher, initialize it and then get
    // its init vector embedded in AESalgID.
    Cipher cipher = BlockCipher.getInstance(algID, AESkey,
                                     BlockCipher.PADDING_PKCS5);
    CBCAlgID AESalgID = (CBCAlgID) cipher.createAlgID();
    out = new FileOutputStream(outdir + "AESinitV.der");
    out.write(AESalgID.iv);
    out.flush();
    out.close();

    return;
  }
}
