/**
 * FindService.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface FindService extends javax.xml.rpc.Service {

    /**
     * The Find service allows you to locate addresses, places, latitude/longitude
     * coordinates, and points of interest (from either MapPoint .NET data
     * or your data that we host for your use).</p><p class="intro">A formal
     * description of the MapPoint .NET suite of services appears at <a href="default.htm">this
     * site</a>
     */
    public java.lang.String getFindServiceSoapAddress();

    public net.mappoint.s.FindServiceSoap getFindServiceSoap() throws javax.xml.rpc.ServiceException;

    public net.mappoint.s.FindServiceSoap getFindServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}
