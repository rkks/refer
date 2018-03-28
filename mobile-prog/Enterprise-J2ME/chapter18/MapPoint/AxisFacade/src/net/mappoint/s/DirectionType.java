/**
 * DirectionType.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class DirectionType implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected DirectionType(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Driving = "Driving";
    public static final java.lang.String _Border = "Border";
    public static final java.lang.String _StartOfDay = "StartOfDay";
    public static final java.lang.String _EndOfDay = "EndOfDay";
    public static final java.lang.String _Warning = "Warning";
    public static final java.lang.String _Waypoint = "Waypoint";
    public static final DirectionType Driving = new DirectionType(_Driving);
    public static final DirectionType Border = new DirectionType(_Border);
    public static final DirectionType StartOfDay = new DirectionType(_StartOfDay);
    public static final DirectionType EndOfDay = new DirectionType(_EndOfDay);
    public static final DirectionType Warning = new DirectionType(_Warning);
    public static final DirectionType Waypoint = new DirectionType(_Waypoint);
    public java.lang.String getValue() { return _value_;}
    public static DirectionType fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        DirectionType enum = (DirectionType)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static DirectionType fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
