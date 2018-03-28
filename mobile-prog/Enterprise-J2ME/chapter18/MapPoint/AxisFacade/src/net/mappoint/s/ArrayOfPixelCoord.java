/**
 * ArrayOfPixelCoord.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class ArrayOfPixelCoord  implements java.io.Serializable {
    private net.mappoint.s.PixelCoord[] pixelCoord;

    public ArrayOfPixelCoord() {
    }

    public net.mappoint.s.PixelCoord[] getPixelCoord() {
        return pixelCoord;
    }

    public void setPixelCoord(net.mappoint.s.PixelCoord[] pixelCoord) {
        this.pixelCoord = pixelCoord;
    }

    public net.mappoint.s.PixelCoord getPixelCoord(int i) {
        return pixelCoord[i];
    }

    public void setPixelCoord(int i, net.mappoint.s.PixelCoord value) {
        this.pixelCoord[i] = value;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof ArrayOfPixelCoord)) return false;
        ArrayOfPixelCoord other = (ArrayOfPixelCoord) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((pixelCoord==null && other.getPixelCoord()==null) || 
             (pixelCoord!=null &&
              java.util.Arrays.equals(pixelCoord, other.getPixelCoord())));
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
        if (getPixelCoord() != null) {
            for (int i=0;
                 i<java.lang.reflect.Array.getLength(getPixelCoord());
                 i++) {
                java.lang.Object obj = java.lang.reflect.Array.get(getPixelCoord(), i);
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
        new org.apache.axis.description.TypeDesc(ArrayOfPixelCoord.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pixelCoord");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelCoord"));
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
