import javax.servlet.jsp.tagext.*;

public class LinkIterationTEI extends TagExtraInfo {
  public VariableInfo[] getVariableInfo(TagData data) {
    // create a VariableInfo
    VariableInfo info = new VariableInfo("link",
                                         "String",
                                         true,
                                         VariableInfo.NESTED );
    VariableInfo[] vi = { info };
    return vi;
  }
}
