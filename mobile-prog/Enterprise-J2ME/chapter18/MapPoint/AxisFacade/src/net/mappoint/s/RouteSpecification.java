/**
 * RouteSpecification.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class RouteSpecification  implements java.io.Serializable {
    private net.mappoint.s.ArrayOfSegmentSpecification segments;
    private net.mappoint.s.RouteResultMask resultMask;
    private java.lang.String dataSourceName;
    private net.mappoint.s.DriverProfile driverProfile;

    public RouteSpecification() {
    }

    public net.mappoint.s.ArrayOfSegmentSpecification getSegments() {
        return segments;
    }

    public void setSegments(net.mappoint.s.ArrayOfSegmentSpecification segments) {
        this.segments = segments;
    }

    public net.mappoint.s.RouteResultMask getResultMask() {
        return resultMask;
    }

    public void setResultMask(net.mappoint.s.RouteResultMask resultMask) {
        this.resultMask = resultMask;
    }

    public java.lang.String getDataSourceName() {
        return dataSourceName;
    }

    public void setDataSourceName(java.lang.String dataSourceName) {
        this.dataSourceName = dataSourceName;
    }

    public net.mappoint.s.DriverProfile getDriverProfile() {
        return driverProfile;
    }

    public void setDriverProfile(net.mappoint.s.DriverProfile driverProfile) {
        this.driverProfile = driverProfile;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof RouteSpecification)) return false;
        RouteSpecification other = (RouteSpecification) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((segments==null && other.getSegments()==null) || 
             (segments!=null &&
              segments.equals(other.getSegments()))) &&
            ((resultMask==null && other.getResultMask()==null) || 
             (resultMask!=null &&
              resultMask.equals(other.getResultMask()))) &&
            ((dataSourceName==null && other.getDataSourceName()==null) || 
             (dataSourceName!=null &&
              dataSourceName.equals(other.getDataSourceName()))) &&
            ((driverProfile==null && other.getDriverProfile()==null) || 
             (driverProfile!=null &&
              driverProfile.equals(other.getDriverProfile())));
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
        if (getSegments() != null) {
            _hashCode += getSegments().hashCode();
        }
        if (getResultMask() != null) {
            _hashCode += getResultMask().hashCode();
        }
        if (getDataSourceName() != null) {
            _hashCode += getDataSourceName().hashCode();
        }
        if (getDriverProfile() != null) {
            _hashCode += getDriverProfile().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(RouteSpecification.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("segments");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Segments"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfSegmentSpecification"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("resultMask");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ResultMask"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteResultMask"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("dataSourceName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("driverProfile");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DriverProfile"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DriverProfile"));
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
