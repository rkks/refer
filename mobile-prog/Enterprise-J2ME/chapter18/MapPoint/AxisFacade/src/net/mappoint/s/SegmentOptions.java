/**
 * SegmentOptions.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class SegmentOptions  implements java.io.Serializable {
    private net.mappoint.s.SegmentPreference preference;
    private java.lang.Boolean calculateSegmentMapView;
    private java.lang.Boolean calculateDirectionMapView;

    public SegmentOptions() {
    }

    public net.mappoint.s.SegmentPreference getPreference() {
        return preference;
    }

    public void setPreference(net.mappoint.s.SegmentPreference preference) {
        this.preference = preference;
    }

    public java.lang.Boolean getCalculateSegmentMapView() {
        return calculateSegmentMapView;
    }

    public void setCalculateSegmentMapView(java.lang.Boolean calculateSegmentMapView) {
        this.calculateSegmentMapView = calculateSegmentMapView;
    }

    public java.lang.Boolean getCalculateDirectionMapView() {
        return calculateDirectionMapView;
    }

    public void setCalculateDirectionMapView(java.lang.Boolean calculateDirectionMapView) {
        this.calculateDirectionMapView = calculateDirectionMapView;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof SegmentOptions)) return false;
        SegmentOptions other = (SegmentOptions) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((preference==null && other.getPreference()==null) || 
             (preference!=null &&
              preference.equals(other.getPreference()))) &&
            ((calculateSegmentMapView==null && other.getCalculateSegmentMapView()==null) || 
             (calculateSegmentMapView!=null &&
              calculateSegmentMapView.equals(other.getCalculateSegmentMapView()))) &&
            ((calculateDirectionMapView==null && other.getCalculateDirectionMapView()==null) || 
             (calculateDirectionMapView!=null &&
              calculateDirectionMapView.equals(other.getCalculateDirectionMapView())));
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
        if (getPreference() != null) {
            _hashCode += getPreference().hashCode();
        }
        if (getCalculateSegmentMapView() != null) {
            _hashCode += getCalculateSegmentMapView().hashCode();
        }
        if (getCalculateDirectionMapView() != null) {
            _hashCode += getCalculateDirectionMapView().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(SegmentOptions.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("preference");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Preference"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SegmentPreference"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("calculateSegmentMapView");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateSegmentMapView"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "boolean"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("calculateDirectionMapView");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateDirectionMapView"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "boolean"));
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
