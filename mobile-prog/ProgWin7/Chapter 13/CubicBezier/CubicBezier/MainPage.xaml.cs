using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace CubicBezier
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnPointDraggerPointChanged(object sender, 
                                        RoutedPropertyChangedEventArgs<Point> args)
        {
            Point translate = new Point(args.NewValue.X - args.OldValue.X,
                                        args.NewValue.Y - args.OldValue.Y);

            if (sender == pt0Dragger)
            {
                pathFig.StartPoint = Move(pathFig.StartPoint, translate);
                ctrl1Line.Points[0] = Move(ctrl1Line.Points[0], translate);
            }
            else if (sender == pt1Dragger)
            {
                pathSeg.Point1 = Move(pathSeg.Point1, translate);
                ctrl1Line.Points[1] = Move(ctrl1Line.Points[1], translate);
            }
            else if (sender == pt2Dragger)
            {
                pathSeg.Point2 = Move(pathSeg.Point2, translate);
                ctrl2Line.Points[0] = Move(ctrl2Line.Points[0], translate);
            }
            else if (sender == pt3Dragger)
            {
                pathSeg.Point3 = Move(pathSeg.Point3, translate);
                ctrl2Line.Points[1] = Move(ctrl2Line.Points[1], translate);
            }
        }

        Point Move(Point point, Point translate)
        {
            return new Point(point.X + translate.X, point.Y + translate.Y);
        }
    }
}