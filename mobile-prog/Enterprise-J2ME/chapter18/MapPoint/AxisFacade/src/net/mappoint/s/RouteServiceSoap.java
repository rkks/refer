/**
 * RouteServiceSoap.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface RouteServiceSoap extends java.rmi.Remote {
    public net.mappoint.s.Route calculateSimpleRoute(net.mappoint.s.ArrayOfLatLong latLongs, java.lang.String dataSourceName, net.mappoint.s.SegmentPreference preference) throws java.rmi.RemoteException;
    public net.mappoint.s.Route calculateRoute(net.mappoint.s.RouteSpecification specification) throws java.rmi.RemoteException;
}
