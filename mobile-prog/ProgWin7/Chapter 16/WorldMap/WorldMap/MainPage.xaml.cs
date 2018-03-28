using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace WorldMap
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            DisplayCoordinates(xySlider.Value);
        }

        void OnXYSliderValueChanged(object sender, 
                                    RoutedPropertyChangedEventArgs<Point> args)
        {
            DisplayCoordinates(args.NewValue);
        }

        void DisplayCoordinates(Point point)
        {
            double longitude = 360 * point.X - 180;
            double latitude = 90 - 180 * point.Y;
            txtblk.Text = String.Format("Longitude: {0:F0} Latitude: {1:F0}", 
                                        longitude, latitude);
        }
    }
}