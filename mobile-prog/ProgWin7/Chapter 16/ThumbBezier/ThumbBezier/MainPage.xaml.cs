using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace ThumbBezier
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnThumbDragDelta(object sender, DragDeltaEventArgs args)
        {
            Thumb thumb = sender as Thumb;
            TranslateTransform translate = thumb.RenderTransform as TranslateTransform;
            translate.X += args.HorizontalChange;
            translate.Y += args.VerticalChange;

            if (thumb == pt0Thumb)
            {
                pathFig.StartPoint = 
                    Move(pathFig.StartPoint, args.HorizontalChange, args.VerticalChange);
                ctrl1Line.Points[0] = 
                    Move(ctrl1Line.Points[0], args.HorizontalChange, args.VerticalChange);
            }
            else if (thumb == pt1Thumb)
            {
                pathSeg.Point1 = 
                    Move(pathSeg.Point1, args.HorizontalChange, args.VerticalChange);
                ctrl1Line.Points[1] = 
                    Move(ctrl1Line.Points[1], args.HorizontalChange, args.VerticalChange);
            }
            else if (thumb == pt2Thumb)
            {
                pathSeg.Point2 = 
                    Move(pathSeg.Point2, args.HorizontalChange, args.VerticalChange);
                ctrl2Line.Points[0] = 
                    Move(ctrl2Line.Points[0], args.HorizontalChange, args.VerticalChange);
            }
            else if (thumb == pt3Thumb)
            {
                pathSeg.Point3 = 
                    Move(pathSeg.Point3, args.HorizontalChange, args.VerticalChange);
                ctrl2Line.Points[1] = 
                    Move(ctrl2Line.Points[1], args.HorizontalChange, args.VerticalChange);
            }
        }

        Point Move(Point point, double horzChange, double vertChange)
        {
            return new Point(point.X + horzChange, point.Y + vertChange);
        }

    }
}