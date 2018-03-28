/**
 * UserInfoHeader.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class UserInfoHeader  implements java.io.Serializable {
    private net.mappoint.s.CultureInfo culture;
    private net.mappoint.s.DistanceUnit defaultDistanceUnit;
    private net.mappoint.s.CountryRegionContext context;

    public UserInfoHeader() {
    }

    public net.mappoint.s.CultureInfo getCulture() {
        return culture;
    }

    public void setCulture(net.mappoint.s.CultureInfo culture) {
        this.culture = culture;
    }

    public net.mappoint.s.DistanceUnit getDefaultDistanceUnit() {
        return defaultDistanceUnit;
    }

    public void setDefaultDistanceUnit(net.mappoint.s.DistanceUnit defaultDistanceUnit) {
        this.defaultDistanceUnit = defaultDistanceUnit;
    }

    public net.mappoint.s.CountryRegionContext getContext() {
        return context;
    }

    public void setContext(net.mappoint.s.CountryRegionContext context) {
        this.context = context;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof UserInfoHeader)) return false;
        UserInfoHeader other = (UserInfoHeader) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((culture==null && other.getCulture()==null) || 
             (culture!=null &&
              culture.equals(other.getCulture()))) &&
            ((defaultDistanceUnit==null && other.getDefaultDistanceUnit()==null) || 
             (defaultDistanceUnit!=null &&
              defaultDistanceUnit.equals(other.getDefaultDistanceUnit()))) &&
            ((context==null && other.getContext()==null) || 
             (context!=null &&
              context.equals(other.getContext())));
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
        if (getCulture() != null) {
            _hashCode += getCulture().hashCode();
        }
        if (getDefaultDistanceUnit() != null) {
            _hashCode += getDefaultDistanceUnit().hashCode();
        }
        if (getContext() != null) {
            _hashCode += getContext().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(UserInfoHeader.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("culture");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Culture"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CultureInfo"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("defaultDistanceUnit");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DefaultDistanceUnit"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DistanceUnit"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("context");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Context"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegionContext"));
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
