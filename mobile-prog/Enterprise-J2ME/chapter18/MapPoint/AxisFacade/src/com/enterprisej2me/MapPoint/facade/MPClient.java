package com.enterprisej2me.MapPoint.facade;

import java.net.*;
import java.io.*;
import java.util.*;
import java.text.*;
import net.mappoint.s.*;

public class MPClient {

  private static String userName = "5236";
  private static String password = "Q1rJjsel2Ub";
  
  private static final String findStagingUrl =
    "http://findv3.staging.mappoint.net/Find-30/FindService.asmx";

  private static final String routeStagingUrl =
    "http://routev3.staging.mappoint.net/Route-30/RouteService.asmx";

  private static final String renderStagingUrl =
    "http://renderv3.staging.mappoint.net/Render-30/RenderService.asmx";

  // Services
  private FindServiceSoap findService;
  private RouteServiceSoap routeService;
  private RenderServiceSoap renderService;

  // Driving directions and maps
  private Segment[] segments;
  private int segmentNum;
  private Route route, displayedRoute;

  public MPClient () {
    try {
      String findUrl = findStagingUrl;
      findService = new FindServiceLocator().getFindServiceSoap(
                                                   new URL(findUrl));
      ((FindServiceSoapStub) findService).setUsername(userName);
      ((FindServiceSoapStub) findService).setPassword(password);
 
      String routeUrl = routeStagingUrl;
      routeService = new RouteServiceLocator().getRouteServiceSoap(
                                                    new URL(routeUrl));
      ((RouteServiceSoapStub) routeService).setUsername(userName);
      ((RouteServiceSoapStub) routeService).setPassword(password);

      String renderUrl = renderStagingUrl;
      renderService = new RenderServiceLocator().getRenderServiceSoap(
                                                   new URL(renderUrl));
      ((RenderServiceSoapStub) renderService).setUsername(userName);
      ((RenderServiceSoapStub) renderService).setPassword(password);
      
    } catch (Exception e) {
      e.printStackTrace();
    }
  }

  // Get the driving directions between 
  // two human-ready addresses. This method
  // also caches the route in the segments array
  //
  // You have to run this method before you can
  // retrieve maps for the entire route or for 
  // each route segement.
  public String [] getDirections (
   String fromStreet, String fromCity,
   String fromState, String fromZip,
   String toStreet, String toCity,
   String toState, String toZip
                       ) throws Exception {

    Location fromLocation = findAddress(fromStreet, fromCity,
            fromState, fromZip);
    Location toLocation = findAddress(toStreet, toCity,
            toState, toZip);
    route = findRoute(fromLocation, toLocation);
    displayedRoute = new Route();
    displayedRoute.setCalculatedRepresentation(
            route.getCalculatedRepresentation());
    segments =
            route.getItinerary().getSegments().getSegment();

    Vector directionsVec = new Vector();
    DecimalFormat distanceFormatter = new DecimalFormat("0.#");
    for (int i=0; i < segments.length; i++) {
      Direction[] segmentDirections =
              segments[i].getDirections().getDirection();
      for (int j=0; j < segmentDirections.length; j++) {
        String instruction = segmentDirections[j].getInstruction();
        double distance = segmentDirections[j].getDistance();
        // Convert the distance to miles.
        distance *= 0.6213712;
        String distanceUnitAbbr = "mi";
        distance = Math.max(distance, 0.1);
        instruction += ":  " + distanceFormatter.format(distance)
                + " " + distanceUnitAbbr + "\n";
        directionsVec.add(instruction);
        System.out.println("Direction: " + instruction);
      }
    }
    segmentNum = directionsVec.size();

    if (segmentNum == 0) {
      throw new Exception("Cannot find direction");
    } else {
      String [] directions = new String [segmentNum];
      for (int i = 0; i < segmentNum; i++) {
        directions[i] = (String) directionsVec.get(i);
      }
      return directions;
    }
  }

  // Return the number of segments of the current
  // cached route. The number is available after
  // you call the getDirections() method.
  public int getSegmentNum () throws Exception {
    return  segmentNum;
  }

  // Get a map from the current cached route.
  // The return value is a byte array for
  // the GIF image.
  // 
  // index == 0 for the overview map
  // index <= segmentNum for a segment map
  public byte [] getMap (int index, 
          int width, int height) throws Exception {
    if (segmentNum == 0) throw new Exception("Run getDirections first");
    if (index > segmentNum) throw new Exception("index out of bounds");

    // generate start and end pushpins
    Pushpin startPushpin = new Pushpin();
    Waypoint startWaypoint = segments[0].getWaypoint();
    startPushpin.setLatLong(startWaypoint.getLocation().getLatLong());
    startPushpin.setLabel(startWaypoint.getName());
    startPushpin.setIconName("Car");
    startPushpin.setIconDataSource("MapPoint.Icons");
    Pushpin endPushpin = new Pushpin();
    Waypoint endWaypoint =
            segments[segments.length - 1].getWaypoint();
    endPushpin.setLatLong(endWaypoint.getLocation().getLatLong());
    endPushpin.setLabel(endWaypoint.getName());
    endPushpin.setIconName("33");
    endPushpin.setIconDataSource("MapPoint.Icons");
    ArrayOfPushpin pushpins = new ArrayOfPushpin();
    pushpins.setPushpin(new Pushpin[] {startPushpin, endPushpin});

    // Map options and specification
    // Ensure that the width and height of the requested map
    // fall within the valid range of 32 to 2000 pixels.
    ImageFormat format = new ImageFormat();
    format.setWidth(new Integer(Math.min(Math.max(width, 32), 2000)));
    format.setHeight(new Integer(Math.min(Math.max(height, 32),
            2000)));
    MapOptions mapOptions = new MapOptions();
    mapOptions.setFormat(format);
    mapOptions.setIsOverviewMap(Boolean.FALSE);
    mapOptions.setStyle(MapStyle.DefaultStyle);
    mapOptions.setFontSize(MapFontSize.Smaller);
    mapOptions.setReturnType(MapReturnType.ReturnImage);
    mapOptions.setRouteHighlightColor(
            RouteHighlightColor.DefaultColor);
    mapOptions.setConstructionDelayHighlightColor(
            RouteHighlightColor.DefaultColor);
    mapOptions.setConstructionClosureHighlightColor(
            RouteHighlightColor.DefaultColor);
    MapSpecification mapSpec = new MapSpecification();
    mapSpec.setDataSourceName("MapPoint.NA");
    mapSpec.setOptions(mapOptions);

    // Adjust the map view to display the entire route.
    MapView mapView =
            route.getItinerary().getView().getByHeightWidth();
    // Specify what the map should display.
    ArrayOfMapView mapViews = new ArrayOfMapView();
    mapViews.setMapView(new MapView[] {mapView});
    mapSpec.setViews(mapViews);
    mapSpec.setRoute(displayedRoute);
    mapSpec.setPushpins(pushpins);
    // Call the MapPoint .NET server to render the map, and save the
    // returned map views.
    ArrayOfMapImage mapImages = renderService.getMap(mapSpec);

    if ( index == 0 ) {
      // need overview map
      return mapImages.getMapImage(0).getMimeData().getBits();
    } else {
      int currentIdx = 1;
      // Iterate through segments to get the requested map.
      for (int i=0; i < segments.length; i++) {
        Direction[] segmentDirections =
                segments[i].getDirections().getDirection();
        for (int j=0; j < segmentDirections.length; j++) {
          if (currentIdx == index) {
            mapView = segmentDirections[j].getView().getByHeightWidth();
            mapViews.setMapView(new MapView[] {mapView});
            mapSpec.setViews(mapViews);
            mapSpec.setRoute(displayedRoute);
            mapSpec.setPushpins(pushpins);
            mapImages = renderService.getMap(mapSpec);
            return mapImages.getMapImage(0).getMimeData().getBits();
          }
          currentIdx++;
        }
      }
    }
    // It should never reach here
    throw new Exception("error");
  }
  
  
  private Location findAddress(
      String street, String city, 
      String state, String zip) throws Exception {

    // Set the data source that corresponds with the selected country.
    FindAddressSpecification findSpec = new FindAddressSpecification();
    findSpec.setDataSourceName("MapPoint.NA");

    // Construct an Address from the various text fields.
    Address address = new Address();
    address.setAddressLine(street);
    address.setPrimaryCity(city);
    // US only
    address.setSecondaryCity("");
    address.setSubdivision(state);
    address.setPostalCode(zip);
    address.setCountryRegion("United States");
    findSpec.setInputAddress(address);

    // Call the MapPoint .NET server to find the specified address.
    FindResults results = findService.findAddress(findSpec);
    Location location = null;
      
    if (results.getNumberFound() == 0) {
     // not found
    } else {
      // get first result
      location =
          results.getResults().getFindResult(0).getFoundLocation();
    }
    return location;
  }
  
  private Route findRoute(Location fromLocation, Location toLocation) 
                                                   throws Exception {

    RouteSpecification routeSpec = new RouteSpecification();
    routeSpec.setDataSourceName(fromLocation.getDataSourceName());

    SegmentSpecification[] segmentSpecs = new SegmentSpecification[2];

    // Construct the waypoints representing the two locations, and
    // use the street address as the name of each waypoint.  The names
    // are included in the driving directions, and we also use them
    // for the pushpins in the MapDisplay.
    Waypoint fromWaypoint = new Waypoint();
    fromWaypoint.setLocation(fromLocation);
    fromWaypoint.setSnap(SnapType.Normal);
    fromWaypoint.setName(fromLocation.getAddress().getAddressLine());
    segmentSpecs[0] = new SegmentSpecification();
    segmentSpecs[0].setWaypoint(fromWaypoint);
      
    Waypoint toWaypoint = new Waypoint();
    toWaypoint.setLocation(toLocation);
    toWaypoint.setSnap(SnapType.Normal);
    toWaypoint.setName(toLocation.getAddress().getAddressLine());
    segmentSpecs[1] = new SegmentSpecification();
    segmentSpecs[1].setWaypoint(toWaypoint);

    ArrayOfSegmentSpecification arrsegmentspecs =
      new ArrayOfSegmentSpecification();
    arrsegmentspecs.setSegmentSpecification(segmentSpecs);
    routeSpec.setSegments(arrsegmentspecs);

    // Call the MapPoint .NET server to calculate the route.
    // Unfortunately, Axis doesn't let us pass a UserInfoRouteHeader
    // to the route service, so we cannot specify the distance units to
    // use for the route.  Instead, we use the default (kilometers) and
    // convert to the desired units in the DrivingDirectionsWindow.
    return routeService.calculateRoute(routeSpec);
  }
}
