/**
 * MapOptions.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class MapOptions  implements java.io.Serializable {
    private net.mappoint.s.RouteHighlightColor routeHighlightColor;
    private net.mappoint.s.RouteHighlightColor constructionDelayHighlightColor;
    private net.mappoint.s.RouteHighlightColor constructionClosureHighlightColor;
    private net.mappoint.s.MapFontSize fontSize;
    private net.mappoint.s.ImageFormat format;
    private java.lang.Boolean isOverviewMap;
    private net.mappoint.s.MapReturnType returnType;
    private double panHorizontal;
    private double panVertical;
    private net.mappoint.s.MapStyle style;
    private java.lang.Double zoom;

    public MapOptions() {
    }

    public net.mappoint.s.RouteHighlightColor getRouteHighlightColor() {
        return routeHighlightColor;
    }

    public void setRouteHighlightColor(net.mappoint.s.RouteHighlightColor routeHighlightColor) {
        this.routeHighlightColor = routeHighlightColor;
    }

    public net.mappoint.s.RouteHighlightColor getConstructionDelayHighlightColor() {
        return constructionDelayHighlightColor;
    }

    public void setConstructionDelayHighlightColor(net.mappoint.s.RouteHighlightColor constructionDelayHighlightColor) {
        this.constructionDelayHighlightColor = constructionDelayHighlightColor;
    }

    public net.mappoint.s.RouteHighlightColor getConstructionClosureHighlightColor() {
        return constructionClosureHighlightColor;
    }

    public void setConstructionClosureHighlightColor(net.mappoint.s.RouteHighlightColor constructionClosureHighlightColor) {
        this.constructionClosureHighlightColor = constructionClosureHighlightColor;
    }

    public net.mappoint.s.MapFontSize getFontSize() {
        return fontSize;
    }

    public void setFontSize(net.mappoint.s.MapFontSize fontSize) {
        this.fontSize = fontSize;
    }

    public net.mappoint.s.ImageFormat getFormat() {
        return format;
    }

    public void setFormat(net.mappoint.s.ImageFormat format) {
        this.format = format;
    }

    public java.lang.Boolean getIsOverviewMap() {
        return isOverviewMap;
    }

    public void setIsOverviewMap(java.lang.Boolean isOverviewMap) {
        this.isOverviewMap = isOverviewMap;
    }

    public net.mappoint.s.MapReturnType getReturnType() {
        return returnType;
    }

    public void setReturnType(net.mappoint.s.MapReturnType returnType) {
        this.returnType = returnType;
    }

    public double getPanHorizontal() {
        return panHorizontal;
    }

    public void setPanHorizontal(double panHorizontal) {
        this.panHorizontal = panHorizontal;
    }

    public double getPanVertical() {
        return panVertical;
    }

    public void setPanVertical(double panVertical) {
        this.panVertical = panVertical;
    }

    public net.mappoint.s.MapStyle getStyle() {
        return style;
    }

    public void setStyle(net.mappoint.s.MapStyle style) {
        this.style = style;
    }

    public java.lang.Double getZoom() {
        return zoom;
    }

    public void setZoom(java.lang.Double zoom) {
        this.zoom = zoom;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof MapOptions)) return false;
        MapOptions other = (MapOptions) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((routeHighlightColor==null && other.getRouteHighlightColor()==null) || 
             (routeHighlightColor!=null &&
              routeHighlightColor.equals(other.getRouteHighlightColor()))) &&
            ((constructionDelayHighlightColor==null && other.getConstructionDelayHighlightColor()==null) || 
             (constructionDelayHighlightColor!=null &&
              constructionDelayHighlightColor.equals(other.getConstructionDelayHighlightColor()))) &&
            ((constructionClosureHighlightColor==null && other.getConstructionClosureHighlightColor()==null) || 
             (constructionClosureHighlightColor!=null &&
              constructionClosureHighlightColor.equals(other.getConstructionClosureHighlightColor()))) &&
            ((fontSize==null && other.getFontSize()==null) || 
             (fontSize!=null &&
              fontSize.equals(other.getFontSize()))) &&
            ((format==null && other.getFormat()==null) || 
             (format!=null &&
              format.equals(other.getFormat()))) &&
            ((isOverviewMap==null && other.getIsOverviewMap()==null) || 
             (isOverviewMap!=null &&
              isOverviewMap.equals(other.getIsOverviewMap()))) &&
            ((returnType==null && other.getReturnType()==null) || 
             (returnType!=null &&
              returnType.equals(other.getReturnType()))) &&
            panHorizontal == other.getPanHorizontal() &&
            panVertical == other.getPanVertical() &&
            ((style==null && other.getStyle()==null) || 
             (style!=null &&
              style.equals(other.getStyle()))) &&
            ((zoom==null && other.getZoom()==null) || 
             (zoom!=null &&
              zoom.equals(other.getZoom())));
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
        if (getRouteHighlightColor() != null) {
            _hashCode += getRouteHighlightColor().hashCode();
        }
        if (getConstructionDelayHighlightColor() != null) {
            _hashCode += getConstructionDelayHighlightColor().hashCode();
        }
        if (getConstructionClosureHighlightColor() != null) {
            _hashCode += getConstructionClosureHighlightColor().hashCode();
        }
        if (getFontSize() != null) {
            _hashCode += getFontSize().hashCode();
        }
        if (getFormat() != null) {
            _hashCode += getFormat().hashCode();
        }
        if (getIsOverviewMap() != null) {
            _hashCode += getIsOverviewMap().hashCode();
        }
        if (getReturnType() != null) {
            _hashCode += getReturnType().hashCode();
        }
        _hashCode += new Double(getPanHorizontal()).hashCode();
        _hashCode += new Double(getPanVertical()).hashCode();
        if (getStyle() != null) {
            _hashCode += getStyle().hashCode();
        }
        if (getZoom() != null) {
            _hashCode += getZoom().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(MapOptions.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("routeHighlightColor");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteHighlightColor"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteHighlightColor"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("constructionDelayHighlightColor");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ConstructionDelayHighlightColor"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteHighlightColor"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("constructionClosureHighlightColor");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ConstructionClosureHighlightColor"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteHighlightColor"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("fontSize");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FontSize"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapFontSize"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("format");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Format"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ImageFormat"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("isOverviewMap");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "IsOverviewMap"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "boolean"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("returnType");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ReturnType"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapReturnType"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("panHorizontal");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PanHorizontal"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "double"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("panVertical");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PanVertical"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "double"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("style");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Style"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapStyle"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("zoom");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Zoom"));
        field.setXmlType(new javax.xml.namespace.QName("http://schemas.xmlsoap.org/soap/encoding/", "double"));
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
