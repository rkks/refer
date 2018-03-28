import javax.microedition.midlet.MIDlet;
import javax.microedition.lcdui.*;

public class LoginMidlet extends MIDlet implements CommandListener {
   private Display display;
   private TextField userName;
   private TextField password;
   private Form form;
   private Command cancel;
   private Command login;

   public LoginMidlet() {
      userName = new TextField("LoginID:", "", 10, TextField.ANY);
      password = new TextField("Password:", "", 10, TextField.PASSWORD);
      form = new Form("Sign in");
      cancel = new Command("Cancel", Command.CANCEL, 2);
      login = new Command("Login", Command.OK, 2);
   }

   public void startApp() {
      display = Display.getDisplay(this);
      form.append(userName);
      form.append(password);
      form.addCommand(cancel);
      form.addCommand(login);
      form.setCommandListener(this);
      display.setCurrent(form);
   }

   public void pauseApp() {
   }

   public void destroyApp(boolean unconditional) {
      notifyDestroyed();
   }

   public void validateUser(String name, String password) {
     if (name.equals("qm") && password.equals("j2")) {
       menu();
     } else {
       tryAgain();
     }
   }     

   public void menu() {
     List services = new List("Choose one", Choice.EXCLUSIVE);
     services.append("Check Mail", null);
     services.append("Compose", null);
     services.append("Addresses", null);
     services.append("Options", null);
     services.append("Sign Out", null);
     display.setCurrent(services);
   }

   public void tryAgain() {
     Alert error = new Alert("Login Incorrect", "Please try again", null, AlertType.ERROR);
     error.setTimeout(Alert.FOREVER);
     userName.setString("");
     password.setString("");
     display.setCurrent(error, form);
   }

   public void commandAction(Command c, Displayable d) {
      String label = c.getLabel();
      if(label.equals("Cancel")) {
        destroyApp(true);
      } else if(label.equals("Login")) {
         validateUser(userName.getString(), password.getString());
      }
   }
}
