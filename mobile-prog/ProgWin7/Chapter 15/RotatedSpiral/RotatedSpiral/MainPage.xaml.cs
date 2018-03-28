using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace RotatedSpiral
{
    public partial class MainPage : PhoneApplicationPage
    {
        RotateTransform rotateTransform = new RotateTransform();

        public MainPage()
        {
            InitializeComponent();
            Loaded += OnLoaded;
        }

        void OnLoaded(object sender, RoutedEventArgs args)
        {
            Point center = new Point(ContentPanel.ActualWidth / 2 - 1,
                                     ContentPanel.ActualHeight / 2 - 1);
            double radius = Math.Min(center.X, center.Y);

            Polyline polyline = new Polyline();
            polyline.Stroke = this.Resources["PhoneForegroundBrush"] as Brush;
            polyline.StrokeThickness = 3;

            for (double angle = 0; angle < 3600; angle += 0.25)
            {
                double scaledRadius = radius * angle / 3600;
                double radians = Math.PI * angle / 180;
                double x = center.X + scaledRadius * Math.Cos(radians);
                double y = center.Y + scaledRadius * Math.Sin(radians);
                polyline.Points.Add(new Point(x, y));
            }
            ContentPanel.Children.Add(polyline);

            rotateTransform.CenterX = center.X;
            rotateTransform.CenterY = center.Y;
            polyline.RenderTransform = rotateTransform;

            CompositionTarget.Rendering += OnCompositionTargetRendering;
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            TimeSpan elapsedTime = (args as RenderingEventArgs).RenderingTime;
            rotateTransform.Angle = 360 * elapsedTime.TotalSeconds / 3 % 360;
        }
    }
}