/**
 * FindServiceLocator.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class FindServiceLocator extends org.apache.axis.client.Service implements net.mappoint.s.FindService {

    /**
     * The Find service allows you to locate addresses, places, latitude/longitude
     * coordinates, and points of interest (from either MapPoint .NET data
     * or your data that we host for your use).</p><p class="intro">A formal
     * description of the MapPoint .NET suite of services appears at <a href="default.htm">this
     * site</a>
     */

    // Use to get a proxy class for FindServiceSoap
    private final java.lang.String FindServiceSoap_address = "http://findv3.staging.mappoint.net/Find-30/FindService.asmx";

    public java.lang.String getFindServiceSoapAddress() {
        return FindServiceSoap_address;
    }

    // The WSDD service name defaults to the port name.
    private java.lang.String FindServiceSoapWSDDServiceName = "FindServiceSoap";

    public java.lang.String getFindServiceSoapWSDDServiceName() {
        return FindServiceSoapWSDDServiceName;
    }

    public void setFindServiceSoapWSDDServiceName(java.lang.String name) {
        FindServiceSoapWSDDServiceName = name;
    }

    public net.mappoint.s.FindServiceSoap getFindServiceSoap() throws javax.xml.rpc.ServiceException {
       java.net.URL endpoint;
        try {
            endpoint = new java.net.URL(FindServiceSoap_address);
        }
        catch (java.net.MalformedURLException e) {
            return null; // unlikely as URL was validated in WSDL2Java
        }
        return getFindServiceSoap(endpoint);
    }

    public net.mappoint.s.FindServiceSoap getFindServiceSoap(java.net.URL portAddress) throws javax.xml.rpc.ServiceException {
        try {
            net.mappoint.s.FindServiceSoapStub _stub = new net.mappoint.s.FindServiceSoapStub(portAddress, this);
            _stub.setPortName(getFindServiceSoapWSDDServiceName());
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
            if (net.mappoint.s.FindServiceSoap.class.isAssignableFrom(serviceEndpointInterface)) {
                net.mappoint.s.FindServiceSoapStub _stub = new net.mappoint.s.FindServiceSoapStub(new java.net.URL(FindServiceSoap_address), this);
                _stub.setPortName(getFindServiceSoapWSDDServiceName());
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
        return new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindService");
    }

    private java.util.HashSet ports = null;

    public java.util.Iterator getPorts() {
        if (ports == null) {
            ports = new java.util.HashSet();
            ports.add(new javax.xml.namespace.QName("FindServiceSoap"));
        }
        return ports.iterator();
    }

}
