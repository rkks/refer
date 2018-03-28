/**
 * RouteResultMaskFlag.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class RouteResultMaskFlag implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected RouteResultMaskFlag(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Itinerary = "Itinerary";
    public static final java.lang.String _CalculatedRouteRepresentation = "CalculatedRouteRepresentation";
    public static final RouteResultMaskFlag Itinerary = new RouteResultMaskFlag(_Itinerary);
    public static final RouteResultMaskFlag CalculatedRouteRepresentation = new RouteResultMaskFlag(_CalculatedRouteRepresentation);
    public java.lang.String getValue() { return _value_;}
    public static RouteResultMaskFlag fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        RouteResultMaskFlag enum = (RouteResultMaskFlag)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static RouteResultMaskFlag fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
