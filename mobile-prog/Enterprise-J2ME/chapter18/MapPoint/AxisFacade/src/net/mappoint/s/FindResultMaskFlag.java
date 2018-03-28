/**
 * FindResultMaskFlag.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindResultMaskFlag implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected FindResultMaskFlag(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _LatLongFlag = "LatLongFlag";
    public static final java.lang.String _EntityFlag = "EntityFlag";
    public static final java.lang.String _AddressFlag = "AddressFlag";
    public static final java.lang.String _BestMapViewFlag = "BestMapViewFlag";
    public static final FindResultMaskFlag LatLongFlag = new FindResultMaskFlag(_LatLongFlag);
    public static final FindResultMaskFlag EntityFlag = new FindResultMaskFlag(_EntityFlag);
    public static final FindResultMaskFlag AddressFlag = new FindResultMaskFlag(_AddressFlag);
    public static final FindResultMaskFlag BestMapViewFlag = new FindResultMaskFlag(_BestMapViewFlag);
    public java.lang.String getValue() { return _value_;}
    public static FindResultMaskFlag fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        FindResultMaskFlag enum = (FindResultMaskFlag)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static FindResultMaskFlag fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
