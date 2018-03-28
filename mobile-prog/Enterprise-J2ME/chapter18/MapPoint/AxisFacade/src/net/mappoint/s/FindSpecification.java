/**
 * FindSpecification.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindSpecification  implements java.io.Serializable {
    private java.lang.String dataSourceName;
    private java.lang.String inputPlace;
    private net.mappoint.s.ArrayOfString entityTypeNames;
    private net.mappoint.s.FindOptions options;

    public FindSpecification() {
    }

    public java.lang.String getDataSourceName() {
        return dataSourceName;
    }

    public void setDataSourceName(java.lang.String dataSourceName) {
        this.dataSourceName = dataSourceName;
    }

    public java.lang.String getInputPlace() {
        return inputPlace;
    }

    public void setInputPlace(java.lang.String inputPlace) {
        this.inputPlace = inputPlace;
    }

    public net.mappoint.s.ArrayOfString getEntityTypeNames() {
        return entityTypeNames;
    }

    public void setEntityTypeNames(net.mappoint.s.ArrayOfString entityTypeNames) {
        this.entityTypeNames = entityTypeNames;
    }

    public net.mappoint.s.FindOptions getOptions() {
        return options;
    }

    public void setOptions(net.mappoint.s.FindOptions options) {
        this.options = options;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof FindSpecification)) return false;
        FindSpecification other = (FindSpecification) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((dataSourceName==null && other.getDataSourceName()==null) || 
             (dataSourceName!=null &&
              dataSourceName.equals(other.getDataSourceName()))) &&
            ((inputPlace==null && other.getInputPlace()==null) || 
             (inputPlace!=null &&
              inputPlace.equals(other.getInputPlace()))) &&
            ((entityTypeNames==null && other.getEntityTypeNames()==null) || 
             (entityTypeNames!=null &&
              entityTypeNames.equals(other.getEntityTypeNames()))) &&
            ((options==null && other.getOptions()==null) || 
             (options!=null &&
              options.equals(other.getOptions())));
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
        if (getDataSourceName() != null) {
            _hashCode += getDataSourceName().hashCode();
        }
        if (getInputPlace() != null) {
            _hashCode += getInputPlace().hashCode();
        }
        if (getEntityTypeNames() != null) {
            _hashCode += getEntityTypeNames().hashCode();
        }
        if (getOptions() != null) {
            _hashCode += getOptions().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(FindSpecification.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("dataSourceName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("inputPlace");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "InputPlace"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("entityTypeNames");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityTypeNames"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("options");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Options"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindOptions"));
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
