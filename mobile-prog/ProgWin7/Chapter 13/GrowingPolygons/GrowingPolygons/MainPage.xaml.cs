using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Threading;
using Microsoft.Phone.Controls;

namespace GrowingPolygons
{
    public partial class MainPage : PhoneApplicationPage
    {
        Point center;
        double radius;
        int numSides = 2;

        public MainPage()
        {
            InitializeComponent();
            Loaded += OnLoaded;
        }

        void OnLoaded(object sender, RoutedEventArgs args)
        {
            center = new Point(ContentPanel.ActualWidth / 2 - 1,
                               ContentPanel.ActualHeight / 2 - 1);
            radius = Math.Min(center.X, center.Y);

            polygon.Points.Add(new Point(center.X, center.Y - radius));
            polygon.Points.Add(new Point(center.X, center.Y + radius));

            DispatcherTimer tmr = new DispatcherTimer();
            tmr.Interval = TimeSpan.FromSeconds(1);
            tmr.Tick += OnTimerTick;
            tmr.Start();
        }

        void OnTimerTick(object sender, EventArgs args)
        {
            numSides += 1;

            for (int vertex = 1; vertex < numSides; vertex++)
            {
                double radians = vertex * 2 * Math.PI / numSides;
                double x = center.X + radius * Math.Sin(radians);
                double y = center.Y - radius * Math.Cos(radians);
                Point point = new Point(x, y);

                if (vertex < numSides - 1)
                    polygon.Points[vertex] = point;
                else
                    polygon.Points.Add(point);
            }

            PageTitle.Text = "" + numSides + " sides";            
        }
    }
}