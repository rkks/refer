using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace TouchAndDrawCircles
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();
        bool isDrawing, isDragging;
        Path path;
        EllipseGeometry ellipseGeo;

        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            if (isDrawing || isDragging)
                return;

            if (args.OriginalSource is Path)
            {
                ellipseGeo = (args.OriginalSource as Path).Data as EllipseGeometry;

                isDragging = true;
                args.ManipulationContainer = ContentPanel;
                args.Handled = true;
            }
            else if (args.OriginalSource == ContentPanel)
            {
                ellipseGeo = new EllipseGeometry();
                ellipseGeo.Center = args.ManipulationOrigin;
                path = new Path();
                path.Stroke = this.Resources["PhoneForegroundBrush"] as Brush;
                path.Data = ellipseGeo;
                ContentPanel.Children.Add(path);

                isDrawing = true;
                args.Handled = true;
            }

            base.OnManipulationStarted(args);
        }

        protected override void OnManipulationDelta(ManipulationDeltaEventArgs args)
        {
            if (isDragging)
            {
                Point center = ellipseGeo.Center;
                center.X += args.DeltaManipulation.Translation.X;
                center.Y += args.DeltaManipulation.Translation.Y;
                ellipseGeo.Center = center;

                args.Handled = true;
            }
            else if (isDrawing)
            {
                Point translation = args.CumulativeManipulation.Translation;
                double radius = Math.Max(Math.Abs(translation.X), 
                                         Math.Abs(translation.Y));
                ellipseGeo.RadiusX = radius;
                ellipseGeo.RadiusY = radius;

                args.Handled = true;
            }

            base.OnManipulationDelta(args);
        }

        protected override void OnManipulationCompleted(ManipulationCompletedEventArgs args)
        {
            if (isDragging)
            {
                isDragging = false;
                args.Handled = true;
            }
            else if (isDrawing)
            {
                Color clr = Color.FromArgb(255, (byte)rand.Next(256),
                                                (byte)rand.Next(256),
                                                (byte)rand.Next(256));
                path.Fill = new SolidColorBrush(clr);

                isDrawing = false;
                args.Handled = true;
            }

            base.OnManipulationCompleted(args);
        }
    }
}