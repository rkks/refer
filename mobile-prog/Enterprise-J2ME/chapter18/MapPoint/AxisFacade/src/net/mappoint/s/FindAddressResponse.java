/**
 * FindAddressResponse.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindAddressResponse  implements java.io.Serializable {
    private net.mappoint.s.FindResults findAddressResult;

    public FindAddressResponse() {
    }

    public net.mappoint.s.FindResults getFindAddressResult() {
        return findAddressResult;
    }

    public void setFindAddressResult(net.mappoint.s.FindResults findAddressResult) {
        this.findAddressResult = findAddressResult;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof FindAddressResponse)) return false;
        FindAddressResponse other = (FindAddressResponse) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((findAddressResult==null && other.getFindAddressResult()==null) || 
             (findAddressResult!=null &&
              findAddressResult.equals(other.getFindAddressResult())));
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
        if (getFindAddressResult() != null) {
            _hashCode += getFindAddressResult().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(FindAddressResponse.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("findAddressResult");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindAddressResult"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResults"));
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
