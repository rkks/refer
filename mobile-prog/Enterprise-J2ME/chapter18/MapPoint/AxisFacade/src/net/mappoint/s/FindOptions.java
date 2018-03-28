/**
 * FindOptions.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindOptions  implements java.io.Serializable {
    private net.mappoint.s.FindRange range;
    private int searchContext;
    private net.mappoint.s.FindResultMask resultMask;
    private java.lang.Double thresholdScore;

    public FindOptions() {
    }

    public net.mappoint.s.FindRange getRange() {
        return range;
    }

    public void setRange(net.mappoint.s.FindRange range) {
        this.range = range;
    }

    public int getSearchContext() {
        return searchContext;
    }

    public void setSearchContext(int searchContext) {
        this.searchContext = searchContext;
    }

    public net.mappoint.s.FindResultMask getResultMask() {
        return resultMask;
    }

    public void setResultMask(net.mappoint.s.FindResultMask resultMask) {
        this.resultMask = resultMask;
    }

    public java.lang.Double getThresholdScore() {
        return thresholdScore;
    }

    public void setThresholdScore(java.lang.Double thresholdScore) {
        this.thresholdScore = thresholdScore;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof FindOptions)) return false;
        FindOptions other = (FindOptions) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            ((range==null && other.getRange()==null) || 
             (range!=null &&
              range.equals(other.getRange()))) &&
            searchContext == other.getSearchContext() &&
            ((resultMask==null && other.getResultMask()==null) || 
             (resultMask!=null &&
              resultMask.equals(other.getResultMask()))) &&
            ((thresholdScore==null && other.getThresholdScore()==null) || 
             (thresholdScore!=null &&
              thresholdScore.equals(other.getThresholdScore())));
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
        if (getRange() != null) {
            _hashCode += getRange().hashCode();
        }
        _hashCode += getSearchContext();
        if (getResultMask() != null) {
            _hashCode += getResultMask().hashCode();
        }
        if (getThresholdScore() != null) {
            _hashCode += getThresholdScore().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(FindOptions.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("range");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Range"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindRange"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("searchContext");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SearchContext"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("resultMask");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ResultMask"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResultMask"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("thresholdScore");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ThresholdScore"));
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
