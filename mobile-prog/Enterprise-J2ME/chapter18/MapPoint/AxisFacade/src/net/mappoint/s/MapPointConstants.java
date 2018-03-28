/**
 * MapPointConstants.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapPointConstants  implements java.io.Serializable {
    private java.lang.Double earthRadiusInKilometers;
    private java.lang.Double pi;
    private java.lang.Double radiansPerDegree;
    private java.lang.Double degreesPerRadian;
    private java.lang.Double milesPerKilometer;
    private java.lang.Double kilometersPerMile;
    private java.lang.Double kilometersPerFoot;
    private java.lang.Double feetPerKilometer;
    private java.lang.Double findScoreThreshold;
    private java.lang.Integer assumedDpi;
    private java.lang.Double feetPerMeter;
    private java.lang.Double metersPerFoot;

    public MapPointConstants() {
    }

    public java.lang.Double getEarthRadiusInKilometers() {
        return earthRadiusInKilometers;
    }

    public void setEarthRadiusInKilometers(java.lang.Double earthRadiusInKilometers) {
        this.earthRadiusInKilometers = earthRadiusInKilometers;
    }

    public java.lang.Double getPi() {
        return pi;
    }

    public void setPi(java.lang.Double pi) {
        this.pi = pi;
    }

    public java.lang.Double getRadiansPerDegree() {
        return radiansPerDegree;
    }

    public void setRadiansPerDegree(java.lang.Double radiansPerDegree) {
        this.radiansPerDegree = radiansPerDegree;
    }

    public java.lang.Double getDegreesPerRadian() {
        return degreesPerRadian;
    }

    public void setDegreesPerRadian(java.lang.Double degreesPerRadian) {
        this.degreesPerRadian = degreesPerRadian;
    }

    public java.lang.Double getMilesPerKilometer() {
        return milesPerKilometer;
    }

    public void setMilesPerKilometer(java.lang.Double milesPerKilometer) {
        this.milesPerKilometer = milesPerKilometer;
    }

    public java.lang.Double getKilometersPerMile() {
        return kilometersPerMile;
    }

    public void setKilometersPerMile(java.lang.Double kilometersPerMile) {
        this.kilometersPerMile = kilometersPerMile;
    }

    public java.lang.Double getKilometersPerFoot() {
        return kilometersPerFoot;
    }

    public void setKilometersPerFoot(java.lang.Double kilometersPerFoot) {
        this.kilometersPerFoot = kilometersPerFoot;
    }

    public java.lang.Double getFeetPerKilometer() {
        return feetPerKilometer;
    }

    public void setFeetPerKilometer(java.lang.Double feetPerKilometer) {
        this.feetPerKilometer = feetPerKilometer;
    }

    public java.lang.Double getFindScoreThreshold() {
        return findScoreThreshold;
    }

    public void setFindScoreThreshold(java.lang.Double findScoreThreshold) {
        this.findScoreThreshold = findScoreThreshold;
    }

    public java.lang.Integer getAssumedDpi() {
        return assumedDpi;
    }

    public void setAssumedDpi(java.lang.Integer assumedDpi) {
        this.assumedDpi = assumedDpi;
    }

    public java.lang.Double getFeetPerMeter() {
        return feetPerMeter;
    }

    public void setFeetPerMeter(java.lang.Double feetPerMeter) {
        this.feetPerMeter = feetPerMeter;
    }

    public java.lang.Double getMetersPerFoot() {
        return metersPerFoot;
    }

    public void setMetersPerFoot(java.lang.Double metersPerFoot) {
        this.metersPerFoot = metersPerFoot;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof MapPointConstants)) return false;
        MapPointConstants other = (MapPointConstants) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((earthRadiusInKilometers==null && other.getEarthRadiusInKilometers()==null) || 
             (earthRadiusInKilometers!=null &&
              earthRadiusInKilometers.equals(other.getEarthRadiusInKilometers()))) &&
            ((pi==null && other.getPi()==null) || 
             (pi!=null &&
              pi.equals(other.getPi()))) &&
            ((radiansPerDegree==null && other.getRadiansPerDegree()==null) || 
             (radiansPerDegree!=null &&
              radiansPerDegree.equals(other.getRadiansPerDegree()))) &&
            ((degreesPerRadian==null && other.getDegreesPerRadian()==null) || 
             (degreesPerRadian!=null &&
              degreesPerRadian.equals(other.getDegreesPerRadian()))) &&
            ((milesPerKilometer==null && other.getMilesPerKilometer()==null) || 
             (milesPerKilometer!=null &&
              milesPerKilometer.equals(other.getMilesPerKilometer()))) &&
            ((kilometersPerMile==null && other.getKilometersPerMile()==null) || 
             (kilometersPerMile!=null &&
              kilometersPerMile.equals(other.getKilometersPerMile()))) &&
            ((kilometersPerFoot==null && other.getKilometersPerFoot()==null) || 
             (kilometersPerFoot!=null &&
              kilometersPerFoot.equals(other.getKilometersPerFoot()))) &&
            ((feetPerKilometer==null && other.getFeetPerKilometer()==null) || 
             (feetPerKilometer!=null &&
              feetPerKilometer.equals(other.getFeetPerKilometer()))) &&
            ((findScoreThreshold==null && other.getFindScoreThreshold()==null) || 
             (findScoreThreshold!=null &&
              findScoreThreshold.equals(other.getFindScoreThreshold()))) &&
            ((assumedDpi==null && other.getAssumedDpi()==null) || 
             (assumedDpi!=null &&
              assumedDpi.equals(other.getAssumedDpi()))) &&
            ((feetPerMeter==null && other.getFeetPerMeter()==null) || 
             (feetPerMeter!=null &&
              feetPerMeter.equals(other.getFeetPerMeter()))) &&
            ((metersPerFoot==null && other.getMetersPerFoot()==null) || 
             (metersPerFoot!=null &&
              metersPerFoot.equals(other.getMetersPerFoot())));
        __equalsCalc = null;
        return _equals;
    }

    private boolean __hashCodeCalc = false;
    public synchronized int hashCode() {
        if (__hashCodeCalc) {
            return 0;
        }
        __hashCodeCalc = true;
        int _hashCode = 1;
        if (getEarthRadiusInKilometers() != null) {
            _hashCode += getEarthRadiusInKilometers().hashCode();
        }
        if (getPi() != null) {
            _hashCode += getPi().hashCode();
        }
        if (getRadiansPerDegree() != null) {
            _hashCode += getRadiansPerDegree().hashCode();
        }
        if (getDegreesPerRadian() != null) {
            _hashCode += getDegreesPerRadian().hashCode();
        }
        if (getMilesPerKilometer() != null) {
            _hashCode += getMilesPerKilometer().hashCode();
        }
        if (getKilometersPerMile() != null) {
            _hashCode += getKilometersPerMile().hashCode();
        }
        if (getKilometersPerFoot() != null) {
            _hashCode += getKilometersPerFoot().hashCode();
        }
        if (getFeetPerKilometer() != null) {
            _hashCode += getFeetPerKilometer().hashCode();
        }
        if (getFindScoreThreshold() != null) {
            _hashCode += getFindScoreThreshold().hashCode();
        }
        if (getAssumedDpi() != null) {
            _hashCode += getAssumedDpi().hashCode();
        }
        if (getFeetPerMeter() != null) {
            _hashCode += getFeetPerMeter().hashCode();
        }
        if (getMetersPerFoot() != null) {
            _hashCode += getMetersPerFoot().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(MapPointConstants.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("earthRadiusInKilometers");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EarthRadiusInKilometers"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pi");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Pi"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("radiansPerDegree");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RadiansPerDegree"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("degreesPerRadian");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DegreesPerRadian"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("milesPerKilometer");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MilesPerKilometer"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("kilometersPerMile");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "KilometersPerMile"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("kilometersPerFoot");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "KilometersPerFoot"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("feetPerKilometer");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FeetPerKilometer"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("findScoreThreshold");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindScoreThreshold"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("assumedDpi");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "AssumedDpi"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "int"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("feetPerMeter");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FeetPerMeter"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("metersPerFoot");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MetersPerFoot"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
    };

    /**
     * Return type metadata object
     */
    public static org.apache.axis.description.TypeDesc getTypeDesc() {
        return typeDesc;
    }

    /**
     * Get Custom Serializer
     */
    public static org.apache.axis.encoding.Serializer getSerializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanSerializer(
            _javaType, _xmlType, typeDesc);
    }

    /**
     * Get Custom Deserializer
     */
    public static org.apache.axis.encoding.Deserializer getDeserializer(
           java.lang.String mechType, 
           java.lang.Class _javaType,  
           javax.xml.namespace.QName _xmlType) {
        return 
          new  org.apache.axis.encoding.ser.BeanDeserializer(
            _javaType, _xmlType, typeDesc);
    }

}
