/**
 * SegmentPreference.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class SegmentPreference implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected SegmentPreference(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Quickest = "Quickest";
    public static final java.lang.String _Shortest = "Shortest";
    public static final SegmentPreference Quickest = new SegmentPreference(_Quickest);
    public static final SegmentPreference Shortest = new SegmentPreference(_Shortest);
    public java.lang.String getValue() { return _value_;}
    public static SegmentPreference fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        SegmentPreference enum = (SegmentPreference)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static SegmentPreference fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
