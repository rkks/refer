/**
 * SearchOperatorFlag.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class SearchOperatorFlag implements java.io.Serializable {
    private java.lang.String _value_;
    private static java.util.HashMap _table_ = new java.util.HashMap();

    // Constructor
    protected SearchOperatorFlag(java.lang.String value) {
        _value_ = value;
        _table_.put(_value_,this);
    };

    public static final java.lang.String _And = "And";
    public static final java.lang.String _Or = "Or";
    public static final SearchOperatorFlag And = new SearchOperatorFlag(_And);
    public static final SearchOperatorFlag Or = new SearchOperatorFlag(_Or);
    public java.lang.String getValue() { return _value_;}
    public static SearchOperatorFlag fromValue(java.lang.String value)
          throws java.lang.IllegalStateException {
        SearchOperatorFlag enum = (SearchOperatorFlag)
            _table_.get(value);
        if (enum==null) throw new java.lang.IllegalStateException();
        return enum;
    }
    public static SearchOperatorFlag fromString(java.lang.String value)
          throws java.lang.IllegalStateException {
        return fromValue(value);
    }
    public boolean equals(java.lang.Object obj) {return (obj == this);}
    public int hashCode() { return toString().hashCode();}
    public java.lang.String toString() { return _value_;}
}
