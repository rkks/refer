/**
 * SnapType.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class SnapType implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected SnapType(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Normal = "Normal";
    public static final java.lang.String _City = "City";
    public static final SnapType Normal = new SnapType(_Normal);
    public static final SnapType City = new SnapType(_City);
    public java.lang.String getValue() { return _value_;}
    public static SnapType fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        SnapType enum = (SnapType)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static SnapType fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
