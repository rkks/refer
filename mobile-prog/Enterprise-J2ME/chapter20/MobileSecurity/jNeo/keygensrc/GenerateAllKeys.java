import java.io.*;
import com.ntru.jNeo.*;

public class GenerateAllKeys {

  private static String outdir;
  private static RandomNumber rn;
  private static Context ctx;
  private static FileOutputStream out;
 
  public static void main(String[] args) throws Exception {
    outdir = args[0];
    rn = new RandomNumber(NTRUConst.NTRU_SHA1_HASH);
    ctx = new Context(rn);

    System.out.println("Start Enc key generation");
    generateNTRUencKeys();
    System.out.println("Enc keys generated");

    System.out.println("Start Sgn key generation");
    generateNTRUsgnKeys();
    System.out.println("Sgn keys generated");

    System.out.println("Start AES key generation");
    generateAESKey();
    System.out.println("AES key generated");

  }

  private static void generateNTRUencKeys () throws Exception {
    EncKeys NTRUencKeys = new EncKeys(ctx, NTRUConst.NTRU_KEYSTRENGTH_251,
                                      NTRUConst.NTRU_SHA1_HASH);
    
    byte [] pubKey = NTRUencKeys.exportPubKey(null, 0, true);
    out = new FileOutputStream(outdir + "EncPubKey.dat");
    out.write(pubKey);
    out.flush();
    out.close();
 
    byte [] privKey = NTRUencKeys.exportPrivKey(null, 0, true);
    out = new FileOutputStream(outdir + "EncPrivKey.dat");
    out.write(privKey);
    out.flush();
    out.close();
 
    return;
  }

  private static void generateNTRUsgnKeys () throws Exception {
    SgnKeys NTRUsgnKeys = new SgnKeys(ctx, NTRUConst.NTRU_KEYSTRENGTH_251,
                                      NTRUConst.NTRU_SHA1_HASH);

    byte [] pubKey = NTRUsgnKeys.exportPubKey(null, 0);
    out = new FileOutputStream(outdir + "SgnPubKey.dat");
    out.write(pubKey);
    out.flush();
    out.close();
 
    byte [] privKey = NTRUsgnKeys.exportPrivKey(null, 0);
    out = new FileOutputStream(outdir + "SgnPrivKey.dat");
    out.write(privKey);
    out.flush();
    out.close();

    return;
  }

  private static void generateAESKey () throws Exception {
    byte [] AESinitV = new byte [RijndaelKey.BLOCK_SIZE];
    rn.getRandom(AESinitV, 0, AESinitV.length);
    out = new FileOutputStream(outdir + "AESinitV.dat");
    out.write(AESinitV);
    out.flush();
    out.close();
 
    byte [] keydata = new byte [RijndaelKey.BLOCK_SIZE];
    rn.getRandom(keydata, 0, keydata.length);
    out = new FileOutputStream(outdir + "AESkeydata.dat");
    out.write(keydata);
    out.flush();
    out.close();

    return;
  }
}
