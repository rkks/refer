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
using Microsoft.Phone.Controls;

namespace SunnyDay
{
    public partial class MainPage : PhoneApplicationPage
    {
        const int BEAMCOUNT = 24;
        const double INCREMENT = Math.PI / BEAMCOUNT;
        
        public MainPage()
        {
            InitializeComponent();
            Loaded += OnLoaded;
        }

        void OnLoaded(object sender, RoutedEventArgs args)
        {
            Point center = new Point(ContentPanel.ActualWidth / 2,
                                     ContentPanel.ActualHeight / 2);

            double radius = 0.45 * Math.Min(ContentPanel.ActualWidth,
                                            ContentPanel.ActualHeight);
            double innerRadius = 0.8 * radius;
            
            for (int i = 0; i < BEAMCOUNT; i++)
            {
                double radians = 2 * Math.PI * i / BEAMCOUNT;

                if (i == 0)
                {
                    pathFigure.StartPoint = new Point(center.X, center.Y - radius);
                }

                LineSegment lineSeg = new LineSegment();
                lineSeg.Point = new Point(
                    center.X + innerRadius * Math.Sin(radians + INCREMENT / 2), 
                    center.Y - innerRadius * Math.Cos(radians + INCREMENT / 2));
                pathFigure.Segments.Add(lineSeg);

                ArcSegment arcSeg = new ArcSegment();
                arcSeg.Point = new Point(
                    center.X + innerRadius * Math.Sin(radians + 3 * INCREMENT / 2),
                    center.Y - innerRadius * Math.Cos(radians + 3 * INCREMENT / 2));
                pathFigure.Segments.Add(arcSeg);

                lineSeg = new LineSegment();
                lineSeg.Point = new Point(
                    center.X + radius * Math.Sin(radians + 2 * INCREMENT), 
                    center.Y - radius * Math.Cos(radians + 2 * INCREMENT));
                pathFigure.Segments.Add(lineSeg);
            }
        }
    }
}