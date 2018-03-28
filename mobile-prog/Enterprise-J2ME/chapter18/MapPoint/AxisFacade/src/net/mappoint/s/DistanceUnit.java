/**
 * DistanceUnit.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class DistanceUnit implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected DistanceUnit(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Kilometer = "Kilometer";
    public static final java.lang.String _Mile = "Mile";
    public static final DistanceUnit Kilometer = new DistanceUnit(_Kilometer);
    public static final DistanceUnit Mile = new DistanceUnit(_Mile);
    public java.lang.String getValue() { return _value_;}
    public static DistanceUnit fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        DistanceUnit enum = (DistanceUnit)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static DistanceUnit fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
