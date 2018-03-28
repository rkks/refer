/**
 * RenderService.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface RenderService extends javax.xml.rpc.Service {

    /**
     * Microsoft MapPoint .NET map rendering Web-service.<p />A formal description
     * of the MapPoint .NET suite of services appears at <a href="default.htm">this
     * site</a>
     */
    public java.lang.String getRenderServiceSoapAddress();

    public net.mappoint.s.RenderServiceSoap getRenderServiceSoap() throws javax.xml.rpc.ServiceException;

    public net.mappoint.s.RenderServiceSoap getRenderServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}
