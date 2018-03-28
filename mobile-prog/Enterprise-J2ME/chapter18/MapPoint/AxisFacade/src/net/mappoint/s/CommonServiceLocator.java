/**
 * CommonServiceLocator.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class CommonServiceLocator extends org.apache.axis.client.Service implements net.mappoint.s.CommonService {

    /**
     * Microsoft MapPoint.Net common web-service
     */

    // Use to get a proxy class for CommonServiceSoap
    private final java.lang.String CommonServiceSoap_address = "http://findv3.staging.mappoint.net/Find-30/Common.asmx";

    public java.lang.String getCommonServiceSoapAddress() {
        return CommonServiceSoap_address;
    }

    // The WSDD service name defaults to the port name.
    private java.lang.String CommonServiceSoapWSDDServiceName = "CommonServiceSoap";

    public java.lang.String getCommonServiceSoapWSDDServiceName() {
        return CommonServiceSoapWSDDServiceName;
    }

    public void setCommonServiceSoapWSDDServiceName(java.lang.String name) {
        CommonServiceSoapWSDDServiceName = name;
    }

    public net.mappoint.s.CommonServiceSoap getCommonServiceSoap() throws javax.xml.rpc.ServiceException {
       java.net.URL endpoint;
        try {
            endpoint = new java.net.URL(CommonServiceSoap_address);
        }
        catch (java.net.MalformedURLException e) {
            return null; // unlikely as URL was validated in WSDL2Java
        }
        return getCommonServiceSoap(endpoint);
    }

    public net.mappoint.s.CommonServiceSoap getCommonServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException {
        try {
            net.mappoint.s.CommonServiceSoapStub _stub = new net.mappoint.s.CommonServiceSoapStub(portAddress, this);
            _stub.setPortName(getCommonServiceSoapWSDDServiceName());
            return _stub;
        }
        catch (org.apache.axis.AxisFault e) {
            return null;
        }
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        try {
            if (net.mappoint.s.CommonServiceSoap.class.isAssignableFrom(serviceEndpointInterface)) {
                net.mappoint.s.CommonServiceSoapStub _stub = new net.mappoint.s.CommonServiceSoapStub(new java.net.URL(CommonServiceSoap_address), this);
                _stub.setPortName(getCommonServiceSoapWSDDServiceName());
                return _stub;
            }
        }
        catch (java.lang.Throwable t) {
            throw new javax.xml.rpc.ServiceException(t);
        }
        throw new javax.xml.rpc.ServiceException("There is no stub implementation for the interface:  " + (serviceEndpointInterface == null ? "null" : serviceEndpointInterface.getName()));
    }

    /**
     * For the given interface, get the stub implementation.
     * If this service has no port for the given interface,
     * then ServiceException is thrown.
     */
    public java.rmi.Remote getPort(javax.xml.namespace.QName portName, Class serviceEndpointInterface) throws javax.xml.rpc.ServiceException {
        java.rmi.Remote _stub = getPort(serviceEndpointInterface);
        ((org.apache.axis.client.Stub) _stub).setPortName(portName);
        return _stub;
    }

    public javax.xml.namespace.QName getServiceName() {
        return new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CommonService");
    }

    private java.util.HashSet ports = null;

    public java.util.Iterator getPorts() {
        if (ports == null) {
            ports = new java.util.HashSet();
            ports.add(new javax.xml.namespace.QName("CommonServiceSoap"));
        }
        return ports.iterator();
    }

}
