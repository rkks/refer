using System;
using System.Windows;
using System.Windows.Input;
using Microsoft.Phone.Controls;

namespace QuadraticBezier
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            if (args.OriginalSource == pt0Dragger ||
                args.OriginalSource == pt1Dragger ||
                args.OriginalSource == pt2Dragger)
            {
                args.ManipulationContainer = ContentPanel;
                args.Handled = true;
            }
            base.OnManipulationStarted(args);
        }

        protected override void OnManipulationDelta(ManipulationDeltaEventArgs args)
        {
            Point translate = args.DeltaManipulation.Translation;

            if (args.OriginalSource == pt0Dragger)
            {
                pathFig.StartPoint = Move(pathFig.StartPoint, translate);
                ctrlLine.Points[0] = Move(ctrlLine.Points[0], translate);
                pt0Ellipse.Center = Move(pt0Ellipse.Center, translate);
                args.Handled = true;
            }
            else if (args.OriginalSource == pt1Dragger)
            {
                pathSeg.Point1 = Move(pathSeg.Point1, translate);
                ctrlLine.Points[1] = Move(ctrlLine.Points[1], translate);
                pt1Ellipse.Center = Move(pt1Ellipse.Center, translate);
                args.Handled = true;
            }
            else if (args.OriginalSource == pt2Dragger)
            {
                pathSeg.Point2 = Move(pathSeg.Point2, translate);
                ctrlLine.Points[2] = Move(ctrlLine.Points[2], translate);
                pt2Ellipse.Center = Move(pt2Ellipse.Center, translate);
                args.Handled = true;
            }
            base.OnManipulationDelta(args);
        }

        Point Move(Point point, Point translate)
        {
            return new Point(point.X + translate.X, point.Y + translate.Y);
        }
    }
}