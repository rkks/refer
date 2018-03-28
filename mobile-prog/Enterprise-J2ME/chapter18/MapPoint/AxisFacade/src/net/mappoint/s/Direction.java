/**
 * Direction.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class Direction  implements java.io.Serializable {
    private net.mappoint.s.LatLong latLong;
    private net.mappoint.s.DirectionType directionType;
    private net.mappoint.s.DirectionAction action;
    private float bearingOutOfTurn;
    private float bearingIntoTurn;
    private long duration;
    private double distance;
    private java.lang.String towards;
    private java.lang.String instruction;
    private java.lang.String formattedInstruction;
    private int ID;
    private net.mappoint.s.MapViewRepresentations view;

    public Direction() {
    }

    public net.mappoint.s.LatLong getLatLong() {
        return latLong;
    }

    public void setLatLong(net.mappoint.s.LatLong latLong) {
        this.latLong = latLong;
    }

    public net.mappoint.s.DirectionType getDirectionType() {
        return directionType;
    }

    public void setDirectionType(net.mappoint.s.DirectionType directionType) {
        this.directionType = directionType;
    }

    public net.mappoint.s.DirectionAction getAction() {
        return action;
    }

    public void setAction(net.mappoint.s.DirectionAction action) {
        this.action = action;
    }

    public float getBearingOutOfTurn() {
        return bearingOutOfTurn;
    }

    public void setBearingOutOfTurn(float bearingOutOfTurn) {
        this.bearingOutOfTurn = bearingOutOfTurn;
    }

    public float getBearingIntoTurn() {
        return bearingIntoTurn;
    }

    public void setBearingIntoTurn(float bearingIntoTurn) {
        this.bearingIntoTurn = bearingIntoTurn;
    }

    public long getDuration() {
        return duration;
    }

    public void setDuration(long duration) {
        this.duration = duration;
    }

    public double getDistance() {
        return distance;
    }

    public void setDistance(double distance) {
        this.distance = distance;
    }

    public java.lang.String getTowards() {
        return towards;
    }

    public void setTowards(java.lang.String towards) {
        this.towards = towards;
    }

    public java.lang.String getInstruction() {
        return instruction;
    }

    public void setInstruction(java.lang.String instruction) {
        this.instruction = instruction;
    }

    public java.lang.String getFormattedInstruction() {
        return formattedInstruction;
    }

    public void setFormattedInstruction(java.lang.String formattedInstruction) {
        this.formattedInstruction = formattedInstruction;
    }

    public int getID() {
        return ID;
    }

    public void setID(int ID) {
        this.ID = ID;
    }

    public net.mappoint.s.MapViewRepresentations getView() {
        return view;
    }

    public void setView(net.mappoint.s.MapViewRepresentations view) {
        this.view = view;
    }

    private java.lang.Object __equalsCalc = null;
    public synchronized boolean equals(java.lang.Object obj) {
        if (!(obj instanceof Direction)) return false;
        Direction other = (Direction) obj;
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
            ((directionType==null && other.getDirectionType()==null) || 
             (directionType!=null &&
              directionType.equals(other.getDirectionType()))) &&
            ((action==null && other.getAction()==null) || 
             (action!=null &&
              action.equals(other.getAction()))) &&
            bearingOutOfTurn == other.getBearingOutOfTurn() &&
            bearingIntoTurn == other.getBearingIntoTurn() &&
            duration == other.getDuration() &&
            distance == other.getDistance() &&
            ((towards==null && other.getTowards()==null) || 
             (towards!=null &&
              towards.equals(other.getTowards()))) &&
            ((instruction==null && other.getInstruction()==null) || 
             (instruction!=null &&
              instruction.equals(other.getInstruction()))) &&
            ((formattedInstruction==null && other.getFormattedInstruction()==null) || 
             (formattedInstruction!=null &&
              formattedInstruction.equals(other.getFormattedInstruction()))) &&
            ID == other.getID() &&
            ((view==null && other.getView()==null) || 
             (view!=null &&
              view.equals(other.getView())));
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
        if (getDirectionType() != null) {
            _hashCode += getDirectionType().hashCode();
        }
        if (getAction() != null) {
            _hashCode += getAction().hashCode();
        }
        _hashCode += new Float(getBearingOutOfTurn()).hashCode();
        _hashCode += new Float(getBearingIntoTurn()).hashCode();
        _hashCode += new Long(getDuration()).hashCode();
        _hashCode += new Double(getDistance()).hashCode();
        if (getTowards() != null) {
            _hashCode += getTowards().hashCode();
        }
        if (getInstruction() != null) {
            _hashCode += getInstruction().hashCode();
        }
        if (getFormattedInstruction() != null) {
            _hashCode += getFormattedInstruction().hashCode();
        }
        _hashCode += getID();
        if (getView() != null) {
            _hashCode += getView().hashCode();
        }
        __hashCodeCalc = false;
        return _hashCode;
    }

    // Type metadata
    private static org.apache.axis.description.TypeDesc typeDesc =
        new org.apache.axis.description.TypeDesc(Direction.class);

    static {
        org.apache.axis.description.FieldDesc field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("latLong");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("directionType");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DirectionType"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DirectionType"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("action");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Action"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DirectionAction"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("bearingOutOfTurn");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "BearingOutOfTurn"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "float"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("bearingIntoTurn");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "BearingIntoTurn"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "float"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("duration");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Duration"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "long"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("distance");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Distance"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "double"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("towards");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Towards"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("instruction");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Instruction"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("formattedInstruction");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FormattedInstruction"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"));
        field.setMinOccursIs0(true);
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("ID");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ID"));
        field.setXmlType(new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "int"));
        typeDesc.addFieldDesc(field);
        field = new org.apache.axis.description.ElementDesc();
        field.setFieldName("view");
        field.setXmlName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "View"));
        field.setXmlType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapViewRepresentations"));
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
