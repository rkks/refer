/**
 * RouteServiceSoapStub.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public class RouteServiceSoapStub extends org.apache.axis.client.Stub implements net.mappoint.s.RouteServiceSoap {
    private java.util.Vector cachedSerClasses = new java.util.Vector();
    private java.util.Vector cachedSerQNames = new java.util.Vector();
    private java.util.Vector cachedSerFactories = new java.util.Vector();
    private java.util.Vector cachedDeserFactories = new java.util.Vector();

    public RouteServiceSoapStub() throws org.apache.axis.AxisFault {
         this(null);
    }

    public RouteServiceSoapStub(java.net.URL endpointURL, javax.xml.rpc.Service service) throws org.apache.axis.AxisFault {
         this(service);
         super.cachedEndpoint = endpointURL;
    }

    public RouteServiceSoapStub(javax.xml.rpc.Service service) throws org.apache.axis.AxisFault {
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
            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DistanceUnit");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DistanceUnit.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">FindAddress");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindAddress.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "UserInfoRouteHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.UserInfoRouteHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Segment");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Segment.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Route");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Route.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceCapabilityFlag");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DataSourceCapabilityFlag.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Waypoint");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Waypoint.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.RouteSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindNearbySpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindNearbySpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetEntityTypes");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetEntityTypes.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetVersionInfoResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetVersionInfoResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfSegmentSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfSegmentSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">FindAddressResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindAddressResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">FindResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ConvertToPointResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ConvertToPointResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Direction");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Direction.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDataSource");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfDataSource.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetDataSourceInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetDataSourceInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDirection");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfDirection.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetMapResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetMapResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegionContext");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CountryRegionContext.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ParseAddressResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ParseAddressResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindFilter");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindFilter.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ParseAddress");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ParseAddress.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByBoundingRectangle");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ViewByBoundingRectangle.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityPropertyValue");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.EntityPropertyValue.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetVersionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetVersionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfEntityType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLong");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.LatLong.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapView");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapView.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ConvertToPoint");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ConvertToPoint.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetGreatCircleDistances");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetGreatCircleDistances.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">Find");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Find.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DirectionAction");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DirectionAction.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteResultMask");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.RouteResultMask.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(simplesf);
            cachedDeserFactories.add(simpledf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfPushpin");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfPushpin.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfCountryRegionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfCountryRegionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MimeData");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MimeData.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">CalculateSimpleRoute");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CalculateSimpleRoute.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SegmentOptions");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.SegmentOptions.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CultureInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CultureInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "UserInfoHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.UserInfoHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelCoord");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.PixelCoord.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Pushpin");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Pushpin.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityPropertyValue");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfEntityPropertyValue.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByScale");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ViewByScale.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetCountryRegionInfoResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetCountryRegionInfoResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfInt");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfInt.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResultMask");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindResultMask.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(simplesf);
            cachedDeserFactories.add(simpledf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "UserInfoFindHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.UserInfoFindHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">FindNearbyResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindNearbyResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "VersionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.VersionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfMapImage");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfMapImage.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetLocationInfoResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetLocationInfoResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">CalculateSimpleRouteResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CalculateSimpleRouteResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteItinerary");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.RouteItinerary.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "HotArea");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.HotArea.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapStyle");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapStyle.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfFindResult");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfFindResult.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "WhereClause");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.WhereClause.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SegmentSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.SegmentSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Address");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Address.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetGreatCircleDistancesResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetGreatCircleDistancesResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfLatLong");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfLatLong.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "LatLongRectangle");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.LatLongRectangle.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfLocation");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfLocation.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">CalculateRouteResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CalculateRouteResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindAddressSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindAddressSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfHotArea");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfHotArea.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByBoundingLocations");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ViewByBoundingLocations.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CustomerInfoRenderHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CustomerInfoRenderHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetCountryRegionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetCountryRegionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindOptions");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindOptions.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DirectionType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DirectionType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "GetInfoOptions");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetInfoOptions.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ConvertToLatLongResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ConvertToLatLongResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResults");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindResults.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CountryRegionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CountryRegionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetBestMapViewResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetBestMapViewResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CustomerInfoHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CustomerInfoHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapFontSize");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapFontSize.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculatedRouteRepresentation");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CalculatedRouteRepresentation.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteResultMaskFlag");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.RouteResultMaskFlag.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteHighlightColor");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.RouteHighlightColor.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ImageFormat");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ImageFormat.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfSegment");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfSegment.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindRange");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindRange.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapReturnType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapReturnType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SegmentPreference");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.SegmentPreference.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

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

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResultMaskFlag");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindResultMaskFlag.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DataSourceCapability");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DataSourceCapability.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(simplesf);
            cachedDeserFactories.add(simpledf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfMapView");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfMapView.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfString");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfString.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfPixelCoord");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfPixelCoord.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetDataSourceInfoResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetDataSourceInfoResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetLocationInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetLocationInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetBestMapView");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetBestMapView.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "UserInfoRenderHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.UserInfoRenderHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "PixelRectangle");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.PixelRectangle.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfVersionInfo");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfVersionInfo.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "EntityType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.EntityType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">GetEntityTypesResponse");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.GetEntityTypesResponse.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapViewRepresentations");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapViewRepresentations.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">CalculateRoute");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CalculateRoute.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapPointConstants");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapPointConstants.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">ConvertToLatLong");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ConvertToLatLong.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CustomerInfoRouteHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CustomerInfoRouteHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindSpecification");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindSpecification.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapOptions");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapOptions.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SnapType");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.SnapType.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Entity");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Entity.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "MapImage");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.MapImage.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ViewByHeightWidth");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ViewByHeightWidth.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfEntityProperty");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfEntityProperty.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "DriverProfile");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.DriverProfile.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "FindResult");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindResult.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfDouble");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.ArrayOfDouble.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", ">FindNearby");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.FindNearby.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CustomerInfoFindHeader");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.CustomerInfoFindHeader.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Location");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.Location.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(beansf);
            cachedDeserFactories.add(beandf);

            qName = new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SearchOperatorFlag");
            cachedSerQNames.add(qName);
            cls = net.mappoint.s.SearchOperatorFlag.class;
            cachedSerClasses.add(cls);
            cachedSerFactories.add(enumsf);
            cachedDeserFactories.add(enumdf);

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

    public net.mappoint.s.Route calculateSimpleRoute(net.mappoint.s.ArrayOfLatLong latLongs, java.lang.String dataSourceName, net.mappoint.s.SegmentPreference preference) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "latLongs"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "ArrayOfLatLong"), net.mappoint.s.ArrayOfLatLong.class, javax.xml.rpc.ParameterMode.IN);
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "dataSourceName"), new javax.xml.namespace.QName("http://www.w3.org/2001/XMLSchema", "string"), java.lang.String.class, javax.xml.rpc.ParameterMode.IN);
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "preference"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "SegmentPreference"), net.mappoint.s.SegmentPreference.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Route"), net.mappoint.s.Route.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/CalculateSimpleRoute");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateSimpleRoute"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateSimpleRouteResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {latLongs, dataSourceName, preference});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.Route) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.Route) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.Route.class);
            }
        }
    }

    public net.mappoint.s.Route calculateRoute(net.mappoint.s.RouteSpecification specification) throws java.rmi.RemoteException {
        if (super.cachedEndpoint == null) {
            throw new org.apache.axis.NoEndPointException();
        }
        org.apache.axis.client.Call _call = createCall();
        _call.addParameter(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "specification"), new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "RouteSpecification"), net.mappoint.s.RouteSpecification.class, javax.xml.rpc.ParameterMode.IN);
        _call.setReturnType(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "Route"), net.mappoint.s.Route.class);
        _call.setUseSOAPAction(true);
        _call.setSOAPActionURI("http://s.mappoint.net/mappoint-30/CalculateRoute");
        _call.setEncodingStyle(null);
        _call.setScopedProperty(org.apache.axis.client.Call.SEND_TYPE_ATTR, Boolean.FALSE);
        _call.setScopedProperty(org.apache.axis.AxisEngine.PROP_DOMULTIREFS, Boolean.FALSE);
        _call.setOperationStyle("wrapped");
        _call.setOperationName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateRoute"));
        _call.setReturnQName(new javax.xml.namespace.QName("http://s.mappoint.net/mappoint-30/", "CalculateRouteResult"));

        java.lang.Object _resp = _call.invoke(new java.lang.Object[] {specification});

        if (_resp instanceof java.rmi.RemoteException) {
            throw (java.rmi.RemoteException)_resp;
        }
        else {
            try {
                return (net.mappoint.s.Route) _resp;
            } catch (java.lang.Exception _exception) {
                return (net.mappoint.s.Route) org.apache.axis.utils.JavaUtils.convert(_resp, net.mappoint.s.Route.class);
            }
        }
    }

}
