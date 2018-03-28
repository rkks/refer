/**
 * ArrayOfEntityPropertyValue.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class ArrayOfEntityPropertyValue  implements java.io.Serializable {
    private net.mappoint.s.EntityPropertyValue[] property;

    public ArrayOfEntityPropertyValue() {
    }

    public net.mappoint.s.EntityPropertyValue[] getProperty() {
        return property;
    }

    public void setProperty(net.mappoint.s.EntityPropertyValue[] property) {
        this.property = property;
    }

    public net.mappoint.s.EntityPropertyValue getProperty(int i) {
        return property[i];
    }

    public void setProperty(int i, net.mappoint.s.EntityPropertyValue value) {
        this.property[i] = value;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof ArrayOfEntityPropertyValue)) return false;
        ArrayOfEntityPropertyValue other = (ArrayOfEntityPropertyValue) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((property==null && other.getProperty()==null) || 
             (property!=null &&
              java.util.Arrays.equals(property, other.getProperty())));
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
        if (getProperty() != null) {
            for (int i=0;
                 i<java.lang.reflect.Array.getLength(getProperty());
                 i++) {
                java.lang.Object obj = java.lang.reflect.Array.get(getProperty(), i);
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
        new org.apache.axis.description.TypeDesc(ArrayOfEntityPropertyValue.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("property");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Property"));
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
