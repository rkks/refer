/**
 * DirectionAction.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class DirectionAction implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected DirectionAction(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _Other = "Other";
    public static final java.lang.String _Depart = "Depart";
    public static final java.lang.String _Arrive = "Arrive";
    public static final java.lang.String _TurnLeft = "TurnLeft";
    public static final java.lang.String _TurnRight = "TurnRight";
    public static final java.lang.String _BearLeft = "BearLeft";
    public static final java.lang.String _BearRight = "BearRight";
    public static final java.lang.String _Merge = "Merge";
    public static final java.lang.String _Continue = "Continue";
    public static final java.lang.String _TurnBack = "TurnBack";
    public static final java.lang.String _TakeRoundabout = "TakeRoundabout";
    public static final java.lang.String _ConstructionDelay = "ConstructionDelay";
    public static final java.lang.String _ConstructionStop = "ConstructionStop";
    public static final java.lang.String _NameChange = "NameChange";
    public static final java.lang.String _LeftLeft = "LeftLeft";
    public static final java.lang.String _LeftRight = "LeftRight";
    public static final java.lang.String _RightLeft = "RightLeft";
    public static final java.lang.String _RightRight = "RightRight";
    public static final java.lang.String _TakeRamp = "TakeRamp";
    public static final java.lang.String _TakeRampLeft = "TakeRampLeft";
    public static final java.lang.String _TakeRampRight = "TakeRampRight";
    public static final java.lang.String _KeepStraight = "KeepStraight";
    public static final java.lang.String _KeepLeft = "KeepLeft";
    public static final java.lang.String _KeepRight = "KeepRight";
    public static final DirectionAction Other = new DirectionAction(_Other);
    public static final DirectionAction Depart = new DirectionAction(_Depart);
    public static final DirectionAction Arrive = new DirectionAction(_Arrive);
    public static final DirectionAction TurnLeft = new DirectionAction(_TurnLeft);
    public static final DirectionAction TurnRight = new DirectionAction(_TurnRight);
    public static final DirectionAction BearLeft = new DirectionAction(_BearLeft);
    public static final DirectionAction BearRight = new DirectionAction(_BearRight);
    public static final DirectionAction Merge = new DirectionAction(_Merge);
    public static final DirectionAction Continue = new DirectionAction(_Continue);
    public static final DirectionAction TurnBack = new DirectionAction(_TurnBack);
    public static final DirectionAction TakeRoundabout = new DirectionAction(_TakeRoundabout);
    public static final DirectionAction ConstructionDelay = new DirectionAction(_ConstructionDelay);
    public static final DirectionAction ConstructionStop = new DirectionAction(_ConstructionStop);
    public static final DirectionAction NameChange = new DirectionAction(_NameChange);
    public static final DirectionAction LeftLeft = new DirectionAction(_LeftLeft);
    public static final DirectionAction LeftRight = new DirectionAction(_LeftRight);
    public static final DirectionAction RightLeft = new DirectionAction(_RightLeft);
    public static final DirectionAction RightRight = new DirectionAction(_RightRight);
    public static final DirectionAction TakeRamp = new DirectionAction(_TakeRamp);
    public static final DirectionAction TakeRampLeft = new DirectionAction(_TakeRampLeft);
    public static final DirectionAction TakeRampRight = new DirectionAction(_TakeRampRight);
    public static final DirectionAction KeepStraight = new DirectionAction(_KeepStraight);
    public static final DirectionAction KeepLeft = new DirectionAction(_KeepLeft);
    public static final DirectionAction KeepRight = new DirectionAction(_KeepRight);
    public java.lang.String getValue() { return _value_;}
    public static DirectionAction fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        DirectionAction enum = (DirectionAction)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static DirectionAction fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
