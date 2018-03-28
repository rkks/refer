/**
 * DataSourceCapabilityFlag.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class DataSourceCapabilityFlag implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected DataSourceCapabilityFlag(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _CanDrawMaps = "CanDrawMaps";
    public static final java.lang.String _CanFindPlaces = "CanFindPlaces";
    public static final java.lang.String _CanFindNearby = "CanFindNearby";
    public static final java.lang.String _CanRoute = "CanRoute";
    public static final java.lang.String _CanFindAddress = "CanFindAddress";
    public static final java.lang.String _HasIcons = "HasIcons";
    public static final java.lang.String _DataServiceQuery = "DataServiceQuery";
    public static final DataSourceCapabilityFlag CanDrawMaps = new DataSourceCapabilityFlag(_CanDrawMaps);
    public static final DataSourceCapabilityFlag CanFindPlaces = new DataSourceCapabilityFlag(_CanFindPlaces);
    public static final DataSourceCapabilityFlag CanFindNearby = new DataSourceCapabilityFlag(_CanFindNearby);
    public static final DataSourceCapabilityFlag CanRoute = new DataSourceCapabilityFlag(_CanRoute);
    public static final DataSourceCapabilityFlag CanFindAddress = new DataSourceCapabilityFlag(_CanFindAddress);
    public static final DataSourceCapabilityFlag HasIcons = new DataSourceCapabilityFlag(_HasIcons);
    public static final DataSourceCapabilityFlag DataServiceQuery = new DataSourceCapabilityFlag(_DataServiceQuery);
    public java.lang.String getValue() { return _value_;}
    public static DataSourceCapabilityFlag fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        DataSourceCapabilityFlag enum = (DataSourceCapabilityFlag)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static DataSourceCapabilityFlag fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
