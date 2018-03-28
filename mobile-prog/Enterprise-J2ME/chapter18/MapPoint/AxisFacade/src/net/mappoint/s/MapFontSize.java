/**
 * MapFontSize.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapFontSize implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected MapFontSize(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Smallest = "Smallest";
    public static final java.lang.String _Smaller = "Smaller";
    public static final java.lang.String _Medium = "Medium";
    public static final java.lang.String _Larger = "Larger";
    public static final java.lang.String _Largest = "Largest";
    public static final MapFontSize Smallest = new MapFontSize(_Smallest);
    public static final MapFontSize Smaller = new MapFontSize(_Smaller);
    public static final MapFontSize Medium = new MapFontSize(_Medium);
    public static final MapFontSize Larger = new MapFontSize(_Larger);
    public static final MapFontSize Largest = new MapFontSize(_Largest);
    public java.lang.String getValue() { return _value_;}
    public static MapFontSize fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        MapFontSize enum = (MapFontSize)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static MapFontSize fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
