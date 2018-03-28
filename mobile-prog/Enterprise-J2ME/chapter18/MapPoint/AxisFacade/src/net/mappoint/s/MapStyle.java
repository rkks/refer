/**
 * MapStyle.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapStyle implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected MapStyle(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _DefaultStyle = "DefaultStyle";
    public static final java.lang.String _Locator = "Locator";
    public static final java.lang.String _Terrain = "Terrain";
    public static final java.lang.String _Road = "Road";
    public static final java.lang.String _Political = "Political";
    public static final java.lang.String _Small = "Small";
    public static final java.lang.String _LocatorBW = "LocatorBW";
    public static final java.lang.String _TerrainBW = "TerrainBW";
    public static final java.lang.String _RoadBW = "RoadBW";
    public static final java.lang.String _PoliticalBW = "PoliticalBW";
    public static final java.lang.String _SmallBW = "SmallBW";
    public static final java.lang.String _Comprehensive = "Comprehensive";
    public static final java.lang.String _PhysicalFeatures = "PhysicalFeatures";
    public static final java.lang.String _Tectonic = "Tectonic";
    public static final java.lang.String _EarthByDay = "EarthByDay";
    public static final java.lang.String _EarthByNight = "EarthByNight";
    public static final java.lang.String _Ecoregions = "Ecoregions";
    public static final java.lang.String _Climate = "Climate";
    public static final java.lang.String _TemperatureJanuary = "TemperatureJanuary";
    public static final java.lang.String _TemperatureJuly = "TemperatureJuly";
    public static final java.lang.String _PrecipitationJanuary = "PrecipitationJanuary";
    public static final java.lang.String _PrecipitationJuly = "PrecipitationJuly";
    public static final java.lang.String _PrecipitationAnnual = "PrecipitationAnnual";
    public static final java.lang.String _Languages = "Languages";
    public static final java.lang.String _Religions = "Religions";
    public static final java.lang.String _PopulationDensity = "PopulationDensity";
    public static final java.lang.String _Parks = "Parks";
    public static final java.lang.String _TimeZones = "TimeZones";
    public static final java.lang.String _Outline = "Outline";
    public static final MapStyle DefaultStyle = new MapStyle(_DefaultStyle);
    public static final MapStyle Locator = new MapStyle(_Locator);
    public static final MapStyle Terrain = new MapStyle(_Terrain);
    public static final MapStyle Road = new MapStyle(_Road);
    public static final MapStyle Political = new MapStyle(_Political);
    public static final MapStyle Small = new MapStyle(_Small);
    public static final MapStyle LocatorBW = new MapStyle(_LocatorBW);
    public static final MapStyle TerrainBW = new MapStyle(_TerrainBW);
    public static final MapStyle RoadBW = new MapStyle(_RoadBW);
    public static final MapStyle PoliticalBW = new MapStyle(_PoliticalBW);
    public static final MapStyle SmallBW = new MapStyle(_SmallBW);
    public static final MapStyle Comprehensive = new MapStyle(_Comprehensive);
    public static final MapStyle PhysicalFeatures = new MapStyle(_PhysicalFeatures);
    public static final MapStyle Tectonic = new MapStyle(_Tectonic);
    public static final MapStyle EarthByDay = new MapStyle(_EarthByDay);
    public static final MapStyle EarthByNight = new MapStyle(_EarthByNight);
    public static final MapStyle Ecoregions = new MapStyle(_Ecoregions);
    public static final MapStyle Climate = new MapStyle(_Climate);
    public static final MapStyle TemperatureJanuary = new MapStyle(_TemperatureJanuary);
    public static final MapStyle TemperatureJuly = new MapStyle(_TemperatureJuly);
    public static final MapStyle PrecipitationJanuary = new MapStyle(_PrecipitationJanuary);
    public static final MapStyle PrecipitationJuly = new MapStyle(_PrecipitationJuly);
    public static final MapStyle PrecipitationAnnual = new MapStyle(_PrecipitationAnnual);
    public static final MapStyle Languages = new MapStyle(_Languages);
    public static final MapStyle Religions = new MapStyle(_Religions);
    public static final MapStyle PopulationDensity = new MapStyle(_PopulationDensity);
    public static final MapStyle Parks = new MapStyle(_Parks);
    public static final MapStyle TimeZones = new MapStyle(_TimeZones);
    public static final MapStyle Outline = new MapStyle(_Outline);
    public java.lang.String getValue() { return _value_;}
    public static MapStyle fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        MapStyle enum = (MapStyle)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static MapStyle fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
