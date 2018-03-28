/**
 * FindFilter.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindFilter  implements java.io.Serializable {
    private java.lang.String entityTypeName;
    private net.mappoint.s.ArrayOfString propertyNames;
    private net.mappoint.s.WhereClause whereClause;

    public FindFilter() {
    }

    public java.lang.String getEntityTypeName() {
        return entityTypeName;
    }

    public void setEntityTypeName(java.lang.String entityTypeName) {
        this.entityTypeName = entityTypeName;
    }

    public net.mappoint.s.ArrayOfString getPropertyNames() {
        return propertyNames;
    }

    public void setPropertyNames(net.mappoint.s.ArrayOfString propertyNames) {
        this.propertyNames = propertyNames;
    }

    public net.mappoint.s.WhereClause getWhereClause() {
        return whereClause;
    }

    public void setWhereClause(net.mappoint.s.WhereClause whereClause) {
        this.whereClause = whereClause;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof FindFilter)) return false;
        FindFilter other = (FindFilter) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((entityTypeName==null && other.getEntityTypeName()==null) || 
             (entityTypeName!=null &&
              entityTypeName.equals(other.getEntityTypeName()))) &&
            ((propertyNames==null && other.getPropertyNames()==null) || 
             (propertyNames!=null &&
              propertyNames.equals(other.getPropertyNames()))) &&
            ((whereClause==null && other.getWhereClause()==null) || 
             (whereClause!=null &&
              whereClause.equals(other.getWhereClause())));
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
        if (getEntityTypeName() != null) {
            _hashCode += getEntityTypeName().hashCode();
        }
        if (getPropertyNames() != null) {
            _hashCode += getPropertyNames().hashCode();
        }
        if (getWhereClause() != null) {
            _hashCode += getWhereClause().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(FindFilter.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("entityTypeName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityTypeName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("propertyNames");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PropertyNames"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("whereClause");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "WhereClause"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "WhereClause"));
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
