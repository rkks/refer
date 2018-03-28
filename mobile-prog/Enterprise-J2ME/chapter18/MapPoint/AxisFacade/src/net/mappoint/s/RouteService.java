/**
 * RouteService.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface RouteService extends javax.xml.rpc.Service {

    /**
     * Microsoft MapPoint .NET routing web-service.<p />A formal description
     * of the MapPoint .NET suite of services appears at <a href="default.htm">this
     * site</a>
     */
    public java.lang.String getRouteServiceSoapAddress();

    public net.mappoint.s.RouteServiceSoap getRouteServiceSoap() throws javax.xml.rpc.ServiceException;

    public net.mappoint.s.RouteServiceSoap getRouteServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}
