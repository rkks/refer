/**
 * FindResults.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindResults  implements java.io.Serializable {
    private int numberFound;
    private int startIndex;
    private net.mappoint.s.ArrayOfFindResult results;
    private double topScore;

    public FindResults() {
    }

    public int getNumberFound() {
        return numberFound;
    }

    public void setNumberFound(int numberFound) {
        this.numberFound = numberFound;
    }

    public int getStartIndex() {
        return startIndex;
    }

    public void setStartIndex(int startIndex) {
        this.startIndex = startIndex;
    }

    public net.mappoint.s.ArrayOfFindResult getResults() {
        return results;
    }

    public void setResults(net.mappoint.s.ArrayOfFindResult results) {
        this.results = results;
    }

    public double getTopScore() {
        return topScore;
    }

    public void setTopScore(double topScore) {
        this.topScore = topScore;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof FindResults)) return false;
        FindResults other = (FindResults) obj;
        if (obj == null) return false;
        if (this == obj) return true;
        if (__equalsCalc != null) {
            return (__equalsCalc == obj);
        }
        __equalsCalc = obj;
        boolean _equals;
        _equals = true && 
            numberFound == other.getNumberFound() &&
            startIndex == other.getStartIndex() &&
            ((results==null && other.getResults()==null) || 
             (results!=null &&
              results.equals(other.getResults()))) &&
            topScore == other.getTopScore();
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
        _hashCode += getNumberFound();
        _hashCode += getStartIndex();
        if (getResults() != null) {
            _hashCode += getResults().hashCode();
        }
        _hashCode += new Double(getTopScore()).hashCode();
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(FindResults.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("numberFound");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "NumberFound"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("startIndex");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "StartIndex"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("results");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Results"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfFindResult"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("topScore");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "TopScore"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "double"));
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
