/**
 * MapViewRepresentations.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapViewRepresentations  implements java.io.Serializable {
    private net.mappoint.s.ViewByScale byScale;
    private net.mappoint.s.ViewByHeightWidth byHeightWidth;
    private net.mappoint.s.ViewByBoundingRectangle byBoundingRectangle;

    public MapViewRepresentations() {
    }

    public net.mappoint.s.ViewByScale getByScale() {
        return byScale;
    }

    public void setByScale(net.mappoint.s.ViewByScale byScale) {
        this.byScale = byScale;
    }

    public net.mappoint.s.ViewByHeightWidth getByHeightWidth() {
        return byHeightWidth;
    }

    public void setByHeightWidth(net.mappoint.s.ViewByHeightWidth byHeightWidth) {
        this.byHeightWidth = byHeightWidth;
    }

    public net.mappoint.s.ViewByBoundingRectangle getByBoundingRectangle() {
        return byBoundingRectangle;
    }

    public void setByBoundingRectangle(net.mappoint.s.ViewByBoundingRectangle byBoundingRectangle) {
        this.byBoundingRectangle = byBoundingRectangle;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof MapViewRepresentations)) return false;
        MapViewRepresentations other = (MapViewRepresentations) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((byScale==null && other.getByScale()==null) || 
             (byScale!=null &&
              byScale.equals(other.getByScale()))) &&
            ((byHeightWidth==null && other.getByHeightWidth()==null) || 
             (byHeightWidth!=null &&
              byHeightWidth.equals(other.getByHeightWidth()))) &&
            ((byBoundingRectangle==null && other.getByBoundingRectangle()==null) || 
             (byBoundingRectangle!=null &&
              byBoundingRectangle.equals(other.getByBoundingRectangle())));
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
        if (getByScale() != null) {
            _hashCode += getByScale().hashCode();
        }
        if (getByHeightWidth() != null) {
            _hashCode += getByHeightWidth().hashCode();
        }
        if (getByBoundingRectangle() != null) {
            _hashCode += getByBoundingRectangle().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(MapViewRepresentations.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("byScale");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ByScale"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByScale"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("byHeightWidth");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ByHeightWidth"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByHeightWidth"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("byBoundingRectangle");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ByBoundingRectangle"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByBoundingRectangle"));
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
