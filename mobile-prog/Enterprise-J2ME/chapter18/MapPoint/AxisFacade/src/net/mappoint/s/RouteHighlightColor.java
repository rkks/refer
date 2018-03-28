/**
 * RouteHighlightColor.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class RouteHighlightColor implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected RouteHighlightColor(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _DefaultColor = "DefaultColor";
    public static final java.lang.String _Green = "Green";
    public static final java.lang.String _Yellow = "Yellow";
    public static final java.lang.String _Cyan = "Cyan";
    public static final java.lang.String _Red = "Red";
    public static final RouteHighlightColor DefaultColor = new RouteHighlightColor(_DefaultColor);
    public static final RouteHighlightColor Green = new RouteHighlightColor(_Green);
    public static final RouteHighlightColor Yellow = new RouteHighlightColor(_Yellow);
    public static final RouteHighlightColor Cyan = new RouteHighlightColor(_Cyan);
    public static final RouteHighlightColor Red = new RouteHighlightColor(_Red);
    public java.lang.String getValue() { return _value_;}
    public static RouteHighlightColor fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        RouteHighlightColor enum = (RouteHighlightColor)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static RouteHighlightColor fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
