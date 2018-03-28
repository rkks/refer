/**
 * ArrayOfPushpin.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class ArrayOfPushpin  implements java.io.Serializable {
    private net.mappoint.s.Pushpin[] pushpin;

    public ArrayOfPushpin() {
    }

    public net.mappoint.s.Pushpin[] getPushpin() {
        return pushpin;
    }

    public void setPushpin(net.mappoint.s.Pushpin[] pushpin) {
        this.pushpin = pushpin;
    }

    public net.mappoint.s.Pushpin getPushpin(int i) {
        return pushpin[i];
    }

    public void setPushpin(int i, net.mappoint.s.Pushpin value) {
        this.pushpin[i] = value;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof ArrayOfPushpin)) return false;
        ArrayOfPushpin other = (ArrayOfPushpin) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((pushpin==null && other.getPushpin()==null) || 
             (pushpin!=null &&
              java.util.Arrays.equals(pushpin, other.getPushpin())));
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
        if (getPushpin() != null) {
            for (int i=0;
                 i<java.lang.reflect.Array.getLength(getPushpin());
                 i++) {
                java.lang.Object obj = java.lang.reflect.Array.get(getPushpin(), i);
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
        new org.apache.axis.description.TypeDesc(ArrayOfPushpin.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pushpin");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Pushpin"));
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
