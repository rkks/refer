/**
 * ArrayOfCountryRegionInfo.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class ArrayOfCountryRegionInfo  implements java.io.Serializable {
    private net.mappoint.s.CountryRegionInfo[] countryRegionInfo;

    public ArrayOfCountryRegionInfo() {
    }

    public net.mappoint.s.CountryRegionInfo[] getCountryRegionInfo() {
        return countryRegionInfo;
    }

    public void setCountryRegionInfo(net.mappoint.s.CountryRegionInfo[] countryRegionInfo) {
        this.countryRegionInfo = countryRegionInfo;
    }

    public net.mappoint.s.CountryRegionInfo getCountryRegionInfo(int i) {
        return countryRegionInfo[i];
    }

    public void setCountryRegionInfo(int i, net.mappoint.s.CountryRegionInfo value) {
        this.countryRegionInfo[i] = value;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof ArrayOfCountryRegionInfo)) return false;
        ArrayOfCountryRegionInfo other = (ArrayOfCountryRegionInfo) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((countryRegionInfo==null && other.getCountryRegionInfo()==null) || 
             (countryRegionInfo!=null &&
              java.util.Arrays.equals(countryRegionInfo, other.getCountryRegionInfo())));
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
        if (getCountryRegionInfo() != null) {
            for (int i=0;
                 i<java.lang.reflect.Array.getLength(getCountryRegionInfo());
                 i++) {
                java.lang.Object obj = java.lang.reflect.Array.get(getCountryRegionInfo(), i);
                if (obj != null &&
                    !obj.getClass().isArray()) {
                    _hashCode += obj.hashCode();
                }
            }
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(ArrayOfCountryRegionInfo.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("countryRegionInfo");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegionInfo"));
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
