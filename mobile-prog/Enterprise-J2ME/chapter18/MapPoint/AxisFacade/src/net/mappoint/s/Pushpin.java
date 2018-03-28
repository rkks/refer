/**
 * Pushpin.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class Pushpin  implements java.io.Serializable {
    private java.lang.String iconDataSource;
    private java.lang.String iconName;
    private java.lang.String label;
    private net.mappoint.s.LatLong latLong;
    private java.lang.String pinID;
    private net.mappoint.s.PixelCoord pixel;
    private boolean returnsHotArea;
    private boolean labelNearbyRoads;

    public Pushpin() {
    }

    public java.lang.String getIconDataSource() {
        return iconDataSource;
    }

    public void setIconDataSource(java.lang.String iconDataSource) {
        this.iconDataSource = iconDataSource;
    }

    public java.lang.String getIconName() {
        return iconName;
    }

    public void setIconName(java.lang.String iconName) {
        this.iconName = iconName;
    }

    public java.lang.String getLabel() {
        return label;
    }

    public void setLabel(java.lang.String label) {
        this.label = label;
    }

    public net.mappoint.s.LatLong getLatLong() {
        return latLong;
    }

    public void setLatLong(net.mappoint.s.LatLong latLong) {
        this.latLong = latLong;
    }

    public java.lang.String getPinID() {
        return pinID;
    }

    public void setPinID(java.lang.String pinID) {
        this.pinID = pinID;
    }

    public net.mappoint.s.PixelCoord getPixel() {
        return pixel;
    }

    public void setPixel(net.mappoint.s.PixelCoord pixel) {
        this.pixel = pixel;
    }

    public boolean isReturnsHotArea() {
        return returnsHotArea;
    }

    public void setReturnsHotArea(boolean returnsHotArea) {
        this.returnsHotArea = returnsHotArea;
    }

    public boolean isLabelNearbyRoads() {
        return labelNearbyRoads;
    }

    public void setLabelNearbyRoads(boolean labelNearbyRoads) {
        this.labelNearbyRoads = labelNearbyRoads;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof Pushpin)) return false;
        Pushpin other = (Pushpin) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((iconDataSource==null && other.getIconDataSource()==null) || 
             (iconDataSource!=null &&
              iconDataSource.equals(other.getIconDataSource()))) &&
            ((iconName==null && other.getIconName()==null) || 
             (iconName!=null &&
              iconName.equals(other.getIconName()))) &&
            ((label==null && other.getLabel()==null) || 
             (label!=null &&
              label.equals(other.getLabel()))) &&
            ((latLong==null && other.getLatLong()==null) || 
             (latLong!=null &&
              latLong.equals(other.getLatLong()))) &&
            ((pinID==null && other.getPinID()==null) || 
             (pinID!=null &&
              pinID.equals(other.getPinID()))) &&
            ((pixel==null && other.getPixel()==null) || 
             (pixel!=null &&
              pixel.equals(other.getPixel()))) &&
            returnsHotArea == other.isReturnsHotArea() &&
            labelNearbyRoads == other.isLabelNearbyRoads();
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
        if (getIconDataSource() != null) {
            _hashCode += getIconDataSource().hashCode();
        }
        if (getIconName() != null) {
            _hashCode += getIconName().hashCode();
        }
        if (getLabel() != null) {
            _hashCode += getLabel().hashCode();
        }
        if (getLatLong() != null) {
            _hashCode += getLatLong().hashCode();
        }
        if (getPinID() != null) {
            _hashCode += getPinID().hashCode();
        }
        if (getPixel() != null) {
            _hashCode += getPixel().hashCode();
        }
        _hashCode += new Boolean(isReturnsHotArea()).hashCode();
        _hashCode += new Boolean(isLabelNearbyRoads()).hashCode();
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(Pushpin.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("iconDataSource");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IconDataSource"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("iconName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IconName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("label");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Label"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("latLong");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pinID");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PinID"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pixel");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Pixel"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelCoord"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("returnsHotArea");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ReturnsHotArea"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "boolean"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("labelNearbyRoads");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LabelNearbyRoads"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "boolean"));
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
