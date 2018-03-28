/**
 * CommonService.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface CommonService extends javax.xml.rpc.Service {

    /**
     * Microsoft MapPoint.Net common web-service
     */
    public java.lang.String getCommonServiceSoapAddress();

    public net.mappoint.s.CommonServiceSoap getCommonServiceSoap() throws javax.xml.rpc.ServiceException;

    public net.mappoint.s.CommonServiceSoap getCommonServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException;
}
