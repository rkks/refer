/**
 * ArrayOfVersionInfo.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class ArrayOfVersionInfo  implements java.io.Serializable {
    private net.mappoint.s.VersionInfo[] versionInfo;

    public ArrayOfVersionInfo() {
    }

    public net.mappoint.s.VersionInfo[] getVersionInfo() {
        return versionInfo;
    }

    public void setVersionInfo(net.mappoint.s.VersionInfo[] versionInfo) {
        this.versionInfo = versionInfo;
    }

    public net.mappoint.s.VersionInfo getVersionInfo(int i) {
        return versionInfo[i];
    }

    public void setVersionInfo(int i, net.mappoint.s.VersionInfo value) {
        this.versionInfo[i] = value;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof ArrayOfVersionInfo)) return false;
        ArrayOfVersionInfo other = (ArrayOfVersionInfo) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((versionInfo==null && other.getVersionInfo()==null) || 
             (versionInfo!=null &&
              java.util.Arrays.equals(versionInfo, other.getVersionInfo())));
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
        if (getVersionInfo() != null) {
            for (int i=0;
                 i<java.lang.reflect.Array.getLength(getVersionInfo());
                 i++) {
                java.lang.Object obj = java.lang.reflect.Array.get(getVersionInfo(), i);
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
        new org.apache.axis.description.TypeDesc(ArrayOfVersionInfo.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("versionInfo");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "VersionInfo"));
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
