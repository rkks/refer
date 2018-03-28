/**
 * CommonServiceSoap.java
 *
 * This file was auto-generated from WSDL
 * by the Apache Axis WSDL2Java emitter.
 */

package net.mappoint.s;

public interface CommonServiceSoap extends java.rmi.Remote {

    /**
     * Get the version of components on this Web Service.
     */
    public net.mappoint.s.ArrayOfVersionInfo getVersionInfo() throws java.rmi.RemoteException;

    /**
     * Retrieve information about countries or national regions.
     */
    public net.mappoint.s.ArrayOfCountryRegionInfo getCountryRegionInfo(net.mappoint.s.ArrayOfInt entityIDs) throws java.rmi.RemoteException;

    /**
     * Retrieve the list types and their properties in a DataSource.
     */
    public net.mappoint.s.ArrayOfEntityType getEntityTypes(java.lang.String dataSourceName) throws java.rmi.RemoteException;

    /**
     * Get a general description of a DataSource.
     */
    public net.mappoint.s.ArrayOfDataSource getDataSourceInfo(net.mappoint.s.ArrayOfString dataSourceNames) throws java.rmi.RemoteException;

    /**
     * Calculate pair-wise distances
     */
    public net.mappoint.s.ArrayOfDouble getGreatCircleDistances(net.mappoint.s.ArrayOfLatLong latLongs) throws java.rmi.RemoteException;
}
