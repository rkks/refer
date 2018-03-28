/**
 * GetInfoOptions.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class GetInfoOptions  implements java.io.Serializable {
    private java.lang.Boolean includeAddresses;
    private java.lang.Boolean includeAllEntityTypes;
    private net.mappoint.s.ArrayOfString entityTypesToReturn;

    public GetInfoOptions() {
    }

    public java.lang.Boolean getIncludeAddresses() {
        return includeAddresses;
    }

    public void setIncludeAddresses(java.lang.Boolean includeAddresses) {
        this.includeAddresses = includeAddresses;
    }

    public java.lang.Boolean getIncludeAllEntityTypes() {
        return includeAllEntityTypes;
    }

    public void setIncludeAllEntityTypes(java.lang.Boolean includeAllEntityTypes) {
        this.includeAllEntityTypes = includeAllEntityTypes;
    }

    public net.mappoint.s.ArrayOfString getEntityTypesToReturn() {
        return entityTypesToReturn;
    }

    public void setEntityTypesToReturn(net.mappoint.s.ArrayOfString entityTypesToReturn) {
        this.entityTypesToReturn = entityTypesToReturn;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof GetInfoOptions)) return false;
        GetInfoOptions other = (GetInfoOptions) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((includeAddresses==null && other.getIncludeAddresses()==null) || 
             (includeAddresses!=null &&
              includeAddresses.equals(other.getIncludeAddresses()))) &&
            ((includeAllEntityTypes==null && other.getIncludeAllEntityTypes()==null) || 
             (includeAllEntityTypes!=null &&
              includeAllEntityTypes.equals(other.getIncludeAllEntityTypes()))) &&
            ((entityTypesToReturn==null && other.getEntityTypesToReturn()==null) || 
             (entityTypesToReturn!=null &&
              entityTypesToReturn.equals(other.getEntityTypesToReturn())));
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
        if (getIncludeAddresses() != null) {
            _hashCode += getIncludeAddresses().hashCode();
        }
        if (getIncludeAllEntityTypes() != null) {
            _hashCode += getIncludeAllEntityTypes().hashCode();
        }
        if (getEntityTypesToReturn() != null) {
            _hashCode += getEntityTypesToReturn().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(GetInfoOptions.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("includeAddresses");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IncludeAddresses"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "boolean"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("includeAllEntityTypes");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IncludeAllEntityTypes"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "boolean"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("entityTypesToReturn");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityTypesToReturn"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString"));
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
