/**
 * GetGreatCircleDistancesResponse.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class GetGreatCircleDistancesResponse  implements java.io.Serializable {
    private net.mappoint.s.ArrayOfDouble getGreatCircleDistancesResult;

    public GetGreatCircleDistancesResponse() {
    }

    public net.mappoint.s.ArrayOfDouble getGetGreatCircleDistancesResult() {
        return getGreatCircleDistancesResult;
    }

    public void setGetGreatCircleDistancesResult(net.mappoint.s.ArrayOfDouble getGreatCircleDistancesResult) {
        this.getGreatCircleDistancesResult = getGreatCircleDistancesResult;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof GetGreatCircleDistancesResponse)) return false;
        GetGreatCircleDistancesResponse other = (GetGreatCircleDistancesResponse) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((getGreatCircleDistancesResult==null && other.getGetGreatCircleDistancesResult()==null) || 
             (getGreatCircleDistancesResult!=null &&
              getGreatCircleDistancesResult.equals(other.getGetGreatCircleDistancesResult())));
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
        if (getGetGreatCircleDistancesResult() != null) {
            _hashCode += getGetGreatCircleDistancesResult().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(GetGreatCircleDistancesResponse.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("getGreatCircleDistancesResult");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetGreatCircleDistancesResult"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDouble"));
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
