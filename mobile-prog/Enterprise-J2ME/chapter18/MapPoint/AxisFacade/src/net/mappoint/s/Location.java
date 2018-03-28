/**
 * Location.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class Location  implements java.io.Serializable {
    private net.mappoint.s.LatLong latLong;
    private net.mappoint.s.Entity entity;
    private net.mappoint.s.Address address;
    private net.mappoint.s.MapViewRepresentations bestMapView;
    private java.lang.String dataSourceName;

    public Location() {
    }

    public net.mappoint.s.LatLong getLatLong() {
        return latLong;
    }

    public void setLatLong(net.mappoint.s.LatLong latLong) {
        this.latLong = latLong;
    }

    public net.mappoint.s.Entity getEntity() {
        return entity;
    }

    public void setEntity(net.mappoint.s.Entity entity) {
        this.entity = entity;
    }

    public net.mappoint.s.Address getAddress() {
        return address;
    }

    public void setAddress(net.mappoint.s.Address address) {
        this.address = address;
    }

    public net.mappoint.s.MapViewRepresentations getBestMapView() {
        return bestMapView;
    }

    public void setBestMapView(net.mappoint.s.MapViewRepresentations bestMapView) {
        this.bestMapView = bestMapView;
    }

    public java.lang.String getDataSourceName() {
        return dataSourceName;
    }

    public void setDataSourceName(java.lang.String dataSourceName) {
        this.dataSourceName = dataSourceName;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof Location)) return false;
        Location other = (Location) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((latLong==null && other.getLatLong()==null) || 
             (latLong!=null &&
              latLong.equals(other.getLatLong()))) &&
            ((entity==null && other.getEntity()==null) || 
             (entity!=null &&
              entity.equals(other.getEntity()))) &&
            ((address==null && other.getAddress()==null) || 
             (address!=null &&
              address.equals(other.getAddress()))) &&
            ((bestMapView==null && other.getBestMapView()==null) || 
             (bestMapView!=null &&
              bestMapView.equals(other.getBestMapView()))) &&
            ((dataSourceName==null && other.getDataSourceName()==null) || 
             (dataSourceName!=null &&
              dataSourceName.equals(other.getDataSourceName())));
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
        if (getLatLong() != null) {
            _hashCode += getLatLong().hashCode();
        }
        if (getEntity() != null) {
            _hashCode += getEntity().hashCode();
        }
        if (getAddress() != null) {
            _hashCode += getAddress().hashCode();
        }
        if (getBestMapView() != null) {
            _hashCode += getBestMapView().hashCode();
        }
        if (getDataSourceName() != null) {
            _hashCode += getDataSourceName().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(Location.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("latLong");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("entity");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Entity"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Entity"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("address");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Address"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Address"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("bestMapView");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "BestMapView"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapViewRepresentations"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("dataSourceName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceName"));
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
