/**
 * Address.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class Address  implements java.io.Serializable {
    private java.lang.String addressLine;
    private java.lang.String primaryCity;
    private java.lang.String secondaryCity;
    private java.lang.String subdivision;
    private java.lang.String postalCode;
    private java.lang.String countryRegion;
    private java.lang.String formattedAddress;

    public Address() {
    }

    public java.lang.String getAddressLine() {
        return addressLine;
    }

    public void setAddressLine(java.lang.String addressLine) {
        this.addressLine = addressLine;
    }

    public java.lang.String getPrimaryCity() {
        return primaryCity;
    }

    public void setPrimaryCity(java.lang.String primaryCity) {
        this.primaryCity = primaryCity;
    }

    public java.lang.String getSecondaryCity() {
        return secondaryCity;
    }

    public void setSecondaryCity(java.lang.String secondaryCity) {
        this.secondaryCity = secondaryCity;
    }

    public java.lang.String getSubdivision() {
        return subdivision;
    }

    public void setSubdivision(java.lang.String subdivision) {
        this.subdivision = subdivision;
    }

    public java.lang.String getPostalCode() {
        return postalCode;
    }

    public void setPostalCode(java.lang.String postalCode) {
        this.postalCode = postalCode;
    }

    public java.lang.String getCountryRegion() {
        return countryRegion;
    }

    public void setCountryRegion(java.lang.String countryRegion) {
        this.countryRegion = countryRegion;
    }

    public java.lang.String getFormattedAddress() {
        return formattedAddress;
    }

    public void setFormattedAddress(java.lang.String formattedAddress) {
        this.formattedAddress = formattedAddress;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof Address)) return false;
        Address other = (Address) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((addressLine==null && other.getAddressLine()==null) || 
             (addressLine!=null &&
              addressLine.equals(other.getAddressLine()))) &&
            ((primaryCity==null && other.getPrimaryCity()==null) || 
             (primaryCity!=null &&
              primaryCity.equals(other.getPrimaryCity()))) &&
            ((secondaryCity==null && other.getSecondaryCity()==null) || 
             (secondaryCity!=null &&
              secondaryCity.equals(other.getSecondaryCity()))) &&
            ((subdivision==null && other.getSubdivision()==null) || 
             (subdivision!=null &&
              subdivision.equals(other.getSubdivision()))) &&
            ((postalCode==null && other.getPostalCode()==null) || 
             (postalCode!=null &&
              postalCode.equals(other.getPostalCode()))) &&
            ((countryRegion==null && other.getCountryRegion()==null) || 
             (countryRegion!=null &&
              countryRegion.equals(other.getCountryRegion()))) &&
            ((formattedAddress==null && other.getFormattedAddress()==null) || 
             (formattedAddress!=null &&
              formattedAddress.equals(other.getFormattedAddress())));
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
        if (getAddressLine() != null) {
            _hashCode += getAddressLine().hashCode();
        }
        if (getPrimaryCity() != null) {
            _hashCode += getPrimaryCity().hashCode();
        }
        if (getSecondaryCity() != null) {
            _hashCode += getSecondaryCity().hashCode();
        }
        if (getSubdivision() != null) {
            _hashCode += getSubdivision().hashCode();
        }
        if (getPostalCode() != null) {
            _hashCode += getPostalCode().hashCode();
        }
        if (getCountryRegion() != null) {
            _hashCode += getCountryRegion().hashCode();
        }
        if (getFormattedAddress() != null) {
            _hashCode += getFormattedAddress().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(Address.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("addressLine");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "AddressLine"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("primaryCity");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PrimaryCity"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("secondaryCity");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SecondaryCity"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("subdivision");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Subdivision"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("postalCode");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PostalCode"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("countryRegion");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegion"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("formattedAddress");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FormattedAddress"));
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
