/**
 * MapReturnType.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapReturnType implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected MapReturnType(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _ReturnImage = "ReturnImage";
    public static final java.lang.String _ReturnUrl = "ReturnUrl";
    public static final java.lang.String _ReturnSecureUrl = "ReturnSecureUrl";
    public static final MapReturnType ReturnImage = new MapReturnType(_ReturnImage);
    public static final MapReturnType ReturnUrl = new MapReturnType(_ReturnUrl);
    public static final MapReturnType ReturnSecureUrl = new MapReturnType(_ReturnSecureUrl);
    public java.lang.String getValue() { return _value_;}
    public static MapReturnType fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        MapReturnType enum = (MapReturnType)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static MapReturnType fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
