using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace Spiral
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            Loaded += OnLoaded;
        }

        void OnLoaded(object sender, RoutedEventArgs args)
        {
            Point center = new Point(ContentPanel.ActualWidth / 2,
                                     ContentPanel.ActualHeight / 2);
            double radius = Math.Min(center.X - 1, center.Y - 1);

            Polyline polyline = new Polyline();
            polyline.Stroke = this.Resources["PhoneForegroundBrush"] as Brush;
            polyline.StrokeThickness = (double)this.Resources["PhoneStrokeThickness"];

            for (double angle = 0; angle < 3600; angle += 0.25)
            {
                double scaledRadius = radius * angle / 3600;
                double radians = Math.PI * angle / 180;
                double x = center.X + scaledRadius * Math.Cos(radians);
                double y = center.Y + scaledRadius * Math.Sin(radians);
                polyline.Points.Add(new Point(x, y));
            }
            ContentPanel.Children.Add(polyline);
        }
    }
}