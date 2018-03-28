/**
 * FindServiceSoap.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface FindServiceSoap extends java.rmi.Remote {

    /**
     * Finds places based on the find options.
     */
    public net.mappoint.s.FindResults find(net.mappoint.s.FindSpecification specification) throws java.rmi.RemoteException;

    /**
     * Finds a list of addresses that match the input address.
     */
    public net.mappoint.s.FindResults findAddress(net.mappoint.s.FindAddressSpecification specification) throws java.rmi.RemoteException;

    /**
     * Parses Address.FormattedAddress into its component Address pieces.
     */
    public net.mappoint.s.Address parseAddress(java.lang.String inputAddress, java.lang.String countryRegion) throws java.rmi.RemoteException;

    /**
     * Searches around LatLongCoord for nearest addresses and entities.
     */
    public net.mappoint.s.ArrayOfLocation getLocationInfo(net.mappoint.s.LatLong location, java.lang.String dataSourceName, net.mappoint.s.GetInfoOptions options) throws java.rmi.RemoteException;

    /**
     * Find a list of POI, within a circle around a lat/long.
     */
    public net.mappoint.s.FindResults findNearby(net.mappoint.s.FindNearbySpecification specification) throws java.rmi.RemoteException;
}
