/**
 * HotArea.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class HotArea  implements java.io.Serializable {
    private net.mappoint.s.PixelRectangle iconRectangle;
    private net.mappoint.s.PixelRectangle labelRectangle;
    private java.lang.String pinID;

    public HotArea() {
    }

    public net.mappoint.s.PixelRectangle getIconRectangle() {
        return iconRectangle;
    }

    public void setIconRectangle(net.mappoint.s.PixelRectangle iconRectangle) {
        this.iconRectangle = iconRectangle;
    }

    public net.mappoint.s.PixelRectangle getLabelRectangle() {
        return labelRectangle;
    }

    public void setLabelRectangle(net.mappoint.s.PixelRectangle labelRectangle) {
        this.labelRectangle = labelRectangle;
    }

    public java.lang.String getPinID() {
        return pinID;
    }

    public void setPinID(java.lang.String pinID) {
        this.pinID = pinID;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof HotArea)) return false;
        HotArea other = (HotArea) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((iconRectangle==null && other.getIconRectangle()==null) || 
             (iconRectangle!=null &&
              iconRectangle.equals(other.getIconRectangle()))) &&
            ((labelRectangle==null && other.getLabelRectangle()==null) || 
             (labelRectangle!=null &&
              labelRectangle.equals(other.getLabelRectangle()))) &&
            ((pinID==null && other.getPinID()==null) || 
             (pinID!=null &&
              pinID.equals(other.getPinID())));
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
        if (getIconRectangle() != null) {
            _hashCode += getIconRectangle().hashCode();
        }
        if (getLabelRectangle() != null) {
            _hashCode += getLabelRectangle().hashCode();
        }
        if (getPinID() != null) {
            _hashCode += getPinID().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(HotArea.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("iconRectangle");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IconRectangle"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelRectangle"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("labelRectangle");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LabelRectangle"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelRectangle"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pinID");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PinID"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
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
