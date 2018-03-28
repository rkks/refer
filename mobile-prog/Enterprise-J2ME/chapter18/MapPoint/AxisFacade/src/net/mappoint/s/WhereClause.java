/**
 * WhereClause.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class WhereClause  implements java.io.Serializable {
    private net.mappoint.s.ArrayOfEntityPropertyValue searchProperties;
    private net.mappoint.s.SearchOperatorFlag searchOperator;

    public WhereClause() {
    }

    public net.mappoint.s.ArrayOfEntityPropertyValue getSearchProperties() {
        return searchProperties;
    }

    public void setSearchProperties(net.mappoint.s.ArrayOfEntityPropertyValue searchProperties) {
        this.searchProperties = searchProperties;
    }

    public net.mappoint.s.SearchOperatorFlag getSearchOperator() {
        return searchOperator;
    }

    public void setSearchOperator(net.mappoint.s.SearchOperatorFlag searchOperator) {
        this.searchOperator = searchOperator;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof WhereClause)) return false;
        WhereClause other = (WhereClause) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((searchProperties==null && other.getSearchProperties()==null) || 
             (searchProperties!=null &&
              searchProperties.equals(other.getSearchProperties()))) &&
            ((searchOperator==null && other.getSearchOperator()==null) || 
             (searchOperator!=null &&
              searchOperator.equals(other.getSearchOperator())));
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
        if (getSearchProperties() != null) {
            _hashCode += getSearchProperties().hashCode();
        }
        if (getSearchOperator() != null) {
            _hashCode += getSearchOperator().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(WhereClause.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("searchProperties");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SearchProperties"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityPropertyValue"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("searchOperator");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SearchOperator"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SearchOperatorFlag"));
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
