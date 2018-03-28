/**
 * MapImage.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapImage  implements java.io.Serializable {
    private net.mappoint.s.ArrayOfHotArea hotAreas;
    private net.mappoint.s.MimeData mimeData;
    private net.mappoint.s.MapViewRepresentations view;
    private java.lang.String url;

    public MapImage() {
    }

    public net.mappoint.s.ArrayOfHotArea getHotAreas() {
        return hotAreas;
    }

    public void setHotAreas(net.mappoint.s.ArrayOfHotArea hotAreas) {
        this.hotAreas = hotAreas;
    }

    public net.mappoint.s.MimeData getMimeData() {
        return mimeData;
    }

    public void setMimeData(net.mappoint.s.MimeData mimeData) {
        this.mimeData = mimeData;
    }

    public net.mappoint.s.MapViewRepresentations getView() {
        return view;
    }

    public void setView(net.mappoint.s.MapViewRepresentations view) {
        this.view = view;
    }

    public java.lang.String getUrl() {
        return url;
    }

    public void setUrl(java.lang.String url) {
        this.url = url;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof MapImage)) return false;
        MapImage other = (MapImage) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((hotAreas==null && other.getHotAreas()==null) || 
             (hotAreas!=null &&
              hotAreas.equals(other.getHotAreas()))) &&
            ((mimeData==null && other.getMimeData()==null) || 
             (mimeData!=null &&
              mimeData.equals(other.getMimeData()))) &&
            ((view==null && other.getView()==null) || 
             (view!=null &&
              view.equals(other.getView()))) &&
            ((url==null && other.getUrl()==null) || 
             (url!=null &&
              url.equals(other.getUrl())));
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
        if (getHotAreas() != null) {
            _hashCode += getHotAreas().hashCode();
        }
        if (getMimeData() != null) {
            _hashCode += getMimeData().hashCode();
        }
        if (getView() != null) {
            _hashCode += getView().hashCode();
        }
        if (getUrl() != null) {
            _hashCode += getUrl().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(MapImage.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("hotAreas");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "HotAreas"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfHotArea"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("mimeData");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MimeData"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MimeData"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("view");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "View"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapViewRepresentations"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("url");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Url"));
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
