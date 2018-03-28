/**
 * MapSpecification.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapSpecification  implements java.io.Serializable {
    private java.lang.String dataSourceName;
    private net.mappoint.s.ArrayOfInt highlightedEntityIDs;
    private net.mappoint.s.MapOptions options;
    private net.mappoint.s.ArrayOfPushpin pushpins;
    private net.mappoint.s.Route route;
    private net.mappoint.s.ArrayOfMapView views;

    public MapSpecification() {
    }

    public java.lang.String getDataSourceName() {
        return dataSourceName;
    }

    public void setDataSourceName(java.lang.String dataSourceName) {
        this.dataSourceName = dataSourceName;
    }

    public net.mappoint.s.ArrayOfInt getHighlightedEntityIDs() {
        return highlightedEntityIDs;
    }

    public void setHighlightedEntityIDs(net.mappoint.s.ArrayOfInt highlightedEntityIDs) {
        this.highlightedEntityIDs = highlightedEntityIDs;
    }

    public net.mappoint.s.MapOptions getOptions() {
        return options;
    }

    public void setOptions(net.mappoint.s.MapOptions options) {
        this.options = options;
    }

    public net.mappoint.s.ArrayOfPushpin getPushpins() {
        return pushpins;
    }

    public void setPushpins(net.mappoint.s.ArrayOfPushpin pushpins) {
        this.pushpins = pushpins;
    }

    public net.mappoint.s.Route getRoute() {
        return route;
    }

    public void setRoute(net.mappoint.s.Route route) {
        this.route = route;
    }

    public net.mappoint.s.ArrayOfMapView getViews() {
        return views;
    }

    public void setViews(net.mappoint.s.ArrayOfMapView views) {
        this.views = views;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof MapSpecification)) return false;
        MapSpecification other = (MapSpecification) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((dataSourceName==null && other.getDataSourceName()==null) || 
             (dataSourceName!=null &&
              dataSourceName.equals(other.getDataSourceName()))) &&
            ((highlightedEntityIDs==null && other.getHighlightedEntityIDs()==null) || 
             (highlightedEntityIDs!=null &&
              highlightedEntityIDs.equals(other.getHighlightedEntityIDs()))) &&
            ((options==null && other.getOptions()==null) || 
             (options!=null &&
              options.equals(other.getOptions()))) &&
            ((pushpins==null && other.getPushpins()==null) || 
             (pushpins!=null &&
              pushpins.equals(other.getPushpins()))) &&
            ((route==null && other.getRoute()==null) || 
             (route!=null &&
              route.equals(other.getRoute()))) &&
            ((views==null && other.getViews()==null) || 
             (views!=null &&
              views.equals(other.getViews())));
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
        if (getDataSourceName() != null) {
            _hashCode += getDataSourceName().hashCode();
        }
        if (getHighlightedEntityIDs() != null) {
            _hashCode += getHighlightedEntityIDs().hashCode();
        }
        if (getOptions() != null) {
            _hashCode += getOptions().hashCode();
        }
        if (getPushpins() != null) {
            _hashCode += getPushpins().hashCode();
        }
        if (getRoute() != null) {
            _hashCode += getRoute().hashCode();
        }
        if (getViews() != null) {
            _hashCode += getViews().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(MapSpecification.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("dataSourceName");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceName"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("highlightedEntityIDs");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "HighlightedEntityIDs"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfInt"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("options");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Options"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapOptions"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("pushpins");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Pushpins"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfPushpin"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("route");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Route"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Route"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("views");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Views"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfMapView"));
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
