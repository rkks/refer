/**
 * RenderServiceSoap.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface RenderServiceSoap extends java.rmi.Remote {

    /**
     * Renders map images of the given MapViews
     */
    public net.mappoint.s.ArrayOfMapImage getMap(net.mappoint.s.MapSpecification specification) throws java.rmi.RemoteException;

    /**
     * Convert individual LatLongs into PixelCoords as they would appear
     * in the given MapView
     */
    public net.mappoint.s.ArrayOfPixelCoord convertToPoint(net.mappoint.s.ArrayOfLatLong latLong, net.mappoint.s.MapView view, int width, int height) throws java.rmi.RemoteException;

    /**
     * Convert individual PixelCoords into LatLongs as they would appear
     * in the given MapView
     */
    public net.mappoint.s.ArrayOfLatLong convertToLatLong(net.mappoint.s.ArrayOfPixelCoord pixels, net.mappoint.s.MapView view, int width, int height) throws java.rmi.RemoteException;

    /**
     * Get a MapView that encompasses the given locations
     */
    public net.mappoint.s.MapViewRepresentations getBestMapView(net.mappoint.s.ArrayOfLocation locations, java.lang.String dataSourceName) throws java.rmi.RemoteException;
}
