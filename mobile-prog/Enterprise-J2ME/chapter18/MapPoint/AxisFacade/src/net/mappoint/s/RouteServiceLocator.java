/**
 * RouteServiceLocator.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class RouteServiceLocator extends org.apache.axis.client.Service implements net.mappoint.s.RouteService {

    /**
     * Microsoft MapPoint .NET routing web-service.<p />A formal description
     * of the MapPoint .NET suite of services appears at <a href="default.htm">this
     * site</a>
     */

    // Use to get a proxy class for RouteServiceSoap
    private final java.lang.String RouteServiceSoap_address = "http://routev3.staging.mappoint.net/Route-30/RouteService.asmx";

    public java.lang.String getRouteServiceSoapAddress() {
        return RouteServiceSoap_address;
    }

    // The WSDD service name defaults to the port name.
    private java.lang.String RouteServiceSoapWSDDServiceName = "RouteServiceSoap";

    public java.lang.String getRouteServiceSoapWSDDServiceName() {
        return RouteServiceSoapWSDDServiceName;
    }

    public void setRouteServiceSoapWSDDServiceName(java.lang.String name) {
        RouteServiceSoapWSDDServiceName = name;
    }

    public net.mappoint.s.RouteServiceSoap getRouteServiceSoap() throws javax.xml.rpc.ServiceException {
       java.net.URL endpoint;
        try {
            endpoint = new java.net.URL(RouteServiceSoap_address);
        }
        catch (java.net.MalformedURLException e) {
            return null; // unlikely as URL was validated in WSDL2Java
        }
        return getRouteServiceSoap(endpoint);
    }

    public net.mappoint.s.RouteServiceSoap getRouteServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException {
        try {
            net.mappoint.s.RouteServiceSoapStub _stub = new net.mappoint.s.RouteServiceSoapStub(portAddress, this);
            _stub.setPortName(getRouteServiceSoapWSDDServiceName());
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
            if (net.mappoint.s.RouteServiceSoap.class.isAssignableFrom(serviceEndpointInterface)) {
                net.mappoint.s.RouteServiceSoapStub _stub = new net.mappoint.s.RouteServiceSoapStub(new java.net.URL(RouteServiceSoap_address), this);
                _stub.setPortName(getRouteServiceSoapWSDDServiceName());
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
        return new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteService");
    }

    private java.util.HashSet ports = null;

    public java.util.Iterator getPorts() {
        if (ports == null) {
            ports = new java.util.HashSet();
            ports.add(new javax.xml.namespace.QName("RouteServiceSoap"));
        }
        return ports.iterator();
    }

}
