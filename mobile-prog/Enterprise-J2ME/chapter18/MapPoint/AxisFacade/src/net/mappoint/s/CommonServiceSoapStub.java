/**
 * CommonServiceSoapStub.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class CommonServiceSoapStub extends org.apache.axis.client.Stub implements net.mappoint.s.CommonServiceSoap {
    private java.util.Vector cachedSerClasses = new java.util.Vector();
    private java.util.Vector cachedSerQNames = new java.util.Vector();
    private java.util.Vector cachedSerFactories = new java.util.Vector();
    private java.util.Vector cachedDeserFactories = new java.util.Vector();

    public CommonServiceSoapStub() throws org.apache.axis.AxisFault {
         this(null);
    }

    public CommonServiceSoapStub(java.net.URL endpointURL, javax.xml.rpc.Service service) throws org.apache.axis.AxisFault {
         this(service);
         super.cachedEndpoint = endpointURL;
    }

    public CommonServiceSoapStub(javax.xml.rpc.Service service) throws org.apache.axis.AxisFault {
        if (service == null) {
            super.service = new org.apache.axis.client.Service();
        } else {
            super.service = service;
        }
            java.lang.Class cls;
            javax.xml.namespace.QName qName;
            java.lang.Class beansf = org.apache.axis.encoding.ser.BeanSerializerFactory.class;
            java.lang.Class beandf = org.apache.axis.encoding.ser.BeanDeserializerFactory.class;
            java.lang.Class enumsf = org.apache.axis.encoding.ser.EnumSerializerFactory.class;
            java.lang.Class enumdf = org.apache.axis.encoding.ser.EnumDeserializerFactory.class;
            java.lang.Class arraysf = org.apache.axis.encoding.ser.ArraySerializerFactory.class;
            java.lang.Class arraydf = org.apache.axis.encoding.ser.ArrayDeserializerFactory.class;
            java.lang.Class simplesf = org.apache.axis.encoding.ser.SimpleSerializerFactory.class;
            java.lang.Class simpledf = org.apache.axis.encoding.ser.SimpleDeserializerFactory.class;
            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "VersionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.VersionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfCountryRegionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfCountryRegionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityProperty");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.EntityProperty.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSource");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DataSource.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CountryRegionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityProperty");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfEntityProperty.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceCapability");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DataSourceCapability.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(simplesf);
            cachedDeserFactories.add(simpledf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfEntityType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfVersionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfVersionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDouble");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfDouble.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfString.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.LatLong.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.EntityType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfInt");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfInt.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDataSource");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfDataSource.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfLatLong");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfLatLong.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

    }

    private org.apache.axis.client.Call createCall() throws java.rmi.RemoteException {
        try {
            org.apache.axis.client.Call _call =
                    (org.apache.axis.client.Call) super.service.createCall();
            if (super.maintainSessionSet) {
                _call.setMaintainSession(super.maintainSession);
            }
            if (super.cachedUsername != null) {
                _call.setUsername(super.cachedUsername);
            }
            if (super.cachedPassword != null) {
                _call.setPassword(super.cachedPassword);
            }
            if (super.cachedEndpoint != null) {
                _call.setTargetEndpointAddress(super.cachedEndpoint);
            }
            if (super.cachedTimeout != null) {
                _call.setTimeout(super.cachedTimeout);
            }
            if (super.cachedPortName != null) {
                _call.setPortName(super.cachedPortName);
            }
            java.util.Enumeration keys = super.cachedProperties.keys();
            while (keys.hasMoreElements()) {
                java.lang.String key = (java.lang.String) keys.nextElement();
                if(_call.isPropertySupported(key))
                    _call.setProperty(key, super.cachedProperties.get(key));
                else
                    _call.setScopedProperty(key, super.cachedProperties.get(key));
            }
            // All the type mapping information is registered
            // when the first call is made.
            // The type mapping information is actually registered in
            // the TypeMappingRegistry of the service, which
            // is the reason why registration is only needed for the first call.
            synchronized (this) {
                if (firstCall()) {
                    // must set encoding style before registering serializers
                    _call.setEncodingStyle(null);
                    for (int i = 0; i < cachedSerFactories.size(); ++i) {
                        java.lang.Class cls = (java.lang.Class) cachedSerClasses.get(i);
                        javax.xml.namespace.QName qName =
                                (javax.xml.namespace.QName) cachedSerQNames.get(i);
                        java.lang.Class sf = (java.lang.Class)
                                 cachedSerFactories.get(i);
                        java.lang.Class df = (java.lang.Class)
                                 cachedDeserFactories.get(i);
                        _call.registerTypeMapping(cls, qName, sf, df, false);
                    }
                }
            }
            return _call;
        }
        catch (java.lang.Throwable t) {
            throw new org.apache.axis.AxisFault("Failure trying to get the Call object", t);
        }
    }

    public net.mappoint.s.ArrayOfVersionInfo getVersionInfo() throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfVersionInfo"), net.mappoint.s.ArrayOfVersionInfo.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/GetVersionInfo");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetVersionInfo"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetVersionInfoResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.ArrayOfVersionInfo) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.ArrayOfVersionInfo) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.ArrayOfVersionInfo.class);
            }
        }
    }

    public net.mappoint.s.ArrayOfCountryRegionInfo getCountryRegionInfo(net.mappoint.s.ArrayOfInt entityIDs) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "entityIDs"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfInt"), net.mappoint.s.ArrayOfInt.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfCountryRegionInfo"), net.mappoint.s.ArrayOfCountryRegionInfo.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/GetCountryRegionInfo");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetCountryRegionInfo"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetCountryRegionInfoResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {entityIDs});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.ArrayOfCountryRegionInfo) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.ArrayOfCountryRegionInfo) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.ArrayOfCountryRegionInfo.class);
            }
        }
    }

    public net.mappoint.s.ArrayOfEntityType getEntityTypes(java.lang.String dataSourceName) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "dataSourceName"), new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"), java.lang.String.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityType"), net.mappoint.s.ArrayOfEntityType.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/GetEntityTypes");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetEntityTypes"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetEntityTypesResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {dataSourceName});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.ArrayOfEntityType) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.ArrayOfEntityType) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.ArrayOfEntityType.class);
            }
        }
    }

    public net.mappoint.s.ArrayOfDataSource getDataSourceInfo(net.mappoint.s.ArrayOfString dataSourceNames) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "dataSourceNames"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString"), net.mappoint.s.ArrayOfString.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDataSource"), net.mappoint.s.ArrayOfDataSource.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/GetDataSourceInfo");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetDataSourceInfo"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetDataSourceInfoResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {dataSourceNames});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.ArrayOfDataSource) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.ArrayOfDataSource) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.ArrayOfDataSource.class);
            }
        }
    }

    public net.mappoint.s.ArrayOfDouble getGreatCircleDistances(net.mappoint.s.ArrayOfLatLong latLongs) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "latLongs"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfLatLong"), net.mappoint.s.ArrayOfLatLong.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDouble"), net.mappoint.s.ArrayOfDouble.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/GetGreatCircleDistances");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetGreatCircleDistances"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetGreatCircleDistancesResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {latLongs});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.ArrayOfDouble) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.ArrayOfDouble) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.ArrayOfDouble.class);
            }
        }
    }

}
