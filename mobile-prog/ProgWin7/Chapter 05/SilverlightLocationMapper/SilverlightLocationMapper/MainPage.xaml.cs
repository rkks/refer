using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;
using System.Device.Location;
using SilverlightLocationMapper.MsrMapsService;

namespace SilverlightLocationMapper
{
    public partial class MainPage : PhoneApplicationPage
    {
        GeoCoordinateWatcher geoWatcher = new GeoCoordinateWatcher();
        TerraServiceSoapClient proxy = new TerraServiceSoapClient();

        public MainPage()
        {
            InitializeComponent();
            Loaded += OnMainPageLoaded;
        }

        void OnMainPageLoaded(object sender, RoutedEventArgs args)
        {
            // Set event handlers for TerraServiceSoapClient proxy
            proxy.GetAreaFromPtCompleted += OnProxyGetAreaFromPtCompleted;
            proxy.GetTileCompleted += OnProxyGetTileCompleted;

            // Start GeoCoordinateWatcher going
            statusText.Text = "Obtaining geographic location...";
            geoWatcher.PositionChanged += OnGeoWatcherPositionChanged;
            geoWatcher.Start();
        }

        void OnGeoWatcherPositionChanged(object sender, 
                                         GeoPositionChangedEventArgs<GeoCoordinate> args)
        {
            // Turn off GeoWatcher
            geoWatcher.PositionChanged -= OnGeoWatcherPositionChanged;
            geoWatcher.Stop();

            // Set coordinates to title text
            GeoCoordinate coord = args.Position.Location;
            ApplicationTitle.Text += ": " + String.Format("{0:F2}°{1} {2:F2}°{3}", 
                                                          Math.Abs(coord.Latitude),
                                                          coord.Latitude > 0 ? 'N' : 'S',
                                                          Math.Abs(coord.Longitude),
                                                          coord.Longitude > 0 ? 'E' : 'W');
            // Query proxy for AreaBoundingBox                 
            LonLatPt center = new LonLatPt();
            center.Lon = args.Position.Location.Longitude;
            center.Lat = args.Position.Location.Latitude;

            statusText.Text = "Accessing Microsoft Research Maps Service...";
            proxy.GetAreaFromPtAsync(center, 1, Scale.Scale16m, (int)ContentPanel.ActualWidth, 
                                                                (int)ContentPanel.ActualHeight);
        }

        void OnProxyGetAreaFromPtCompleted(object sender, GetAreaFromPtCompletedEventArgs args)
        {
            if (args.Error != null)
            {
                statusText.Text = args.Error.Message;
                return;
            }

            statusText.Text = "Getting map tiles...";

            AreaBoundingBox box = args.Result;
            int xBeg = box.NorthWest.TileMeta.Id.X;
            int yBeg = box.NorthWest.TileMeta.Id.Y;
            int xEnd = box.NorthEast.TileMeta.Id.X;
            int yEnd = box.SouthWest.TileMeta.Id.Y;

            // Loop through the tiles
            for (int x = xBeg; x <= xEnd; x++)
                for (int y = yBeg; y >= yEnd; y--)
                {
                    // Create Image object to display tile
                    Image img = new Image();
                    img.Stretch = Stretch.None;
                    img.HorizontalAlignment = HorizontalAlignment.Left;
                    img.VerticalAlignment = VerticalAlignment.Top;
                    img.Margin = new Thickness((x - xBeg) * 200 - box.NorthWest.Offset.XOffset, 
                                               (yBeg - y) * 200 - box.NorthWest.Offset.YOffset, 
                                               0, 0);

                    // Insert after TextBlock but before Image with logo
                    ContentPanel.Children.Insert(1, img);

                    // Define the tile ID
                    TileId tileId = box.NorthWest.TileMeta.Id;
                    tileId.X = x;
                    tileId.Y = y;

                    // Call proxy to get the tile (Notice that Image is user object)
                    proxy.GetTileAsync(tileId, img);
                }
        }

        void OnProxyGetTileCompleted(object sender, GetTileCompletedEventArgs args)
        {
            if (args.Error != null)
            {
                return;
            }

            Image img = args.UserState as Image;
            BitmapImage bmp = new BitmapImage();
            bmp.SetSource(new MemoryStream(args.Result));
            img.Source = bmp;
        }
    }
}
