/**
 * Route.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class Route  implements java.io.Serializable {
    private net.mappoint.s.RouteSpecification specification;
    private net.mappoint.s.RouteItinerary itinerary;
    private net.mappoint.s.CalculatedRouteRepresentation calculatedRepresentation;

    public Route() {
    }

    public net.mappoint.s.RouteSpecification getSpecification() {
        return specification;
    }

    public void setSpecification(net.mappoint.s.RouteSpecification specification) {
        this.specification = specification;
    }

    public net.mappoint.s.RouteItinerary getItinerary() {
        return itinerary;
    }

    public void setItinerary(net.mappoint.s.RouteItinerary itinerary) {
        this.itinerary = itinerary;
    }

    public net.mappoint.s.CalculatedRouteRepresentation getCalculatedRepresentation() {
        return calculatedRepresentation;
    }

    public void setCalculatedRepresentation(net.mappoint.s.CalculatedRouteRepresentation calculatedRepresentation) {
        this.calculatedRepresentation = calculatedRepresentation;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof Route)) return false;
        Route other = (Route) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((specification==null && other.getSpecification()==null) || 
             (specification!=null &&
              specification.equals(other.getSpecification()))) &&
            ((itinerary==null && other.getItinerary()==null) || 
             (itinerary!=null &&
              itinerary.equals(other.getItinerary()))) &&
            ((calculatedRepresentation==null && other.getCalculatedRepresentation()==null) || 
             (calculatedRepresentation!=null &&
              calculatedRepresentation.equals(other.getCalculatedRepresentation())));
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
        if (getSpecification() != null) {
            _hashCode += getSpecification().hashCode();
        }
        if (getItinerary() != null) {
            _hashCode += getItinerary().hashCode();
        }
        if (getCalculatedRepresentation() != null) {
            _hashCode += getCalculatedRepresentation().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(Route.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("specification");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Specification"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteSpecification"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("itinerary");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Itinerary"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteItinerary"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("calculatedRepresentation");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculatedRepresentation"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculatedRouteRepresentation"));
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
