package display;

class DisplayItem {
  private String mShortText, mLongText, mExtra;
  
  public DisplayItem(String shortText, String longText,
      String extra) {
    mShortText = shortText;
    mLongText = longText;
    mExtra = extra;
  }
  
  public String getShortText() { return mShortText; }
  public String getLongText() { return mLongText; }
  public String getExtra() { return mExtra; }
}
