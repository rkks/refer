/**
 * CountryRegionInfo.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class CountryRegionInfo  implements java.io.Serializable {
    private int entityID;
    private net.mappoint.s.LatLong latLong;
    private java.lang.String iso2;
    private java.lang.String iso3;
    private java.lang.String friendlyName;
    private java.lang.String officialName;

    public CountryRegionInfo() {
    }

    public int getEntityID() {
        return entityID;
    }

    public void setEntityID(int entityID) {
        this.entityID = entityID;
    }

    public net.mappoint.s.LatLong getLatLong() {
        return latLong;
    }

    public void setLatLong(net.mappoint.s.LatLong latLong) {
        this.latLong = latLong;
    }

    public java.lang.String getIso2() {
        return iso2;
    }

    public void setIso2(java.lang.String iso2) {
        this.iso2 = iso2;
    }

    public java.lang.String getIso3() {
        return iso3;
    }

    public void setIso3(java.lang.String iso3) {
        this.iso3 = iso3;
    }

    public java.lang.String getFriendlyName() {
        return friendlyName;
    }

    public void setFriendlyName(java.lang.String friendlyName) {
        this.friendlyName = friendlyName;
    }

    public java.lang.String getOfficialName() {
        return officialName;
    }

    public void setOfficialName(java.lang.String officialName) {
        this.officialName = officialName;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof CountryRegionInfo)) return false;
        CountryRegionInfo other = (CountryRegionInfo) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            entityID == other.getEntityID() &&
            ((latLong==null && other.getLatLong()==null) || 
             (latLong!=null &&
              latLong.equals(other.getLatLong()))) &&
            ((iso2==null && other.getIso2()==null) || 
             (iso2!=null &&
              iso2.equals(other.getIso2()))) &&
            ((iso3==null && other.getIso3()==null) || 
             (iso3!=null &&
              iso3.equals(other.getIso3()))) &&
            ((friendlyName==null && other.getFriendlyName()==null) || 
             (friendlyName!=null &&
              friendlyName.equals(other.getFriendlyName()))) &&
            ((officialName==null && other.getOfficialName()==null) || 
             (officialName!=null &&
              officialName.equals(other.getOfficialName())));
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
        _hashCode += getEntityID();
        if (getLatLong() != null) {
            _hashCode += getLatLong().hashCode();
        }
        if (getIso2() != null) {
            _hashCode += getIso2().hashCode();
        }
        if (getIso3() != null) {
            _hashCode += getIso3().hashCode();
        }
        if (getFriendlyName() != null) {
            _hashCode += getFriendlyName().hashCode();
        }
        if (getOfficialName() != null) {
            _hashCode += getOfficialName().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(CountryRegionInfo.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("entityID");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityID"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("latLong");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("iso2");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Iso2"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("iso3");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Iso3"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("friendlyName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FriendlyName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("officialName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "OfficialName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
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
