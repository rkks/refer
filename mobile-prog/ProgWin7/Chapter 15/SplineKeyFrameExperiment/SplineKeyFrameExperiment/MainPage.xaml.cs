using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Animation;
using Microsoft.Phone.Controls;

namespace SplineKeyFrameExperiment
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            UpdateTextBlock();
        }

        void UpdateTextBlock()
        {
            txtblk.Text = String.Format("pt1 = {0:F2}\npt2 = {1:F2}",
                                        NormalizePoint(dragger1Geometry.Center),
                                        NormalizePoint(dragger2Geometry.Center));
        }

        Point NormalizePoint(Point pt)
        {
            return new Point(pt.X / 400, pt.Y / 400);
        }

        protected override void OnManipulationDelta(ManipulationDeltaEventArgs args)
        {
            Point translation = args.DeltaManipulation.Translation;

            if (args.ManipulationContainer == dragger1)
            {
                Point pt = new Point(Clamp(dragger1Geometry.Center.X + translation.X),
                                     Clamp(dragger1Geometry.Center.Y + translation.Y));

                dragger1Geometry.Center = pt;
                bezierSegment.Point1 = pt;
                tangentLine1.Point = pt;
                UpdateTextBlock();
            }
            if (args.ManipulationContainer == dragger2)
            {
                Point pt = new Point(Clamp(dragger2Geometry.Center.X + translation.X),
                                     Clamp(dragger2Geometry.Center.Y + translation.Y));

                dragger2Geometry.Center = pt;
                bezierSegment.Point2 = pt;
                tangentLine2.Point = pt;
                UpdateTextBlock();
            }

            base.OnManipulationDelta(args);
        }

        double Clamp(double input)
        {
            return Math.Max(0, Math.Min(400, input));
        }

        void OnAppbarAnimateButtonClick(object sender, EventArgs args)
        {
            Point controlPoint1 = NormalizePoint(dragger1Geometry.Center);
            Point controlPoint2 = NormalizePoint(dragger2Geometry.Center);

            splineKeyFrame1.KeySpline = new KeySpline();
            splineKeyFrame1.KeySpline.ControlPoint1 = controlPoint1;
            splineKeyFrame1.KeySpline.ControlPoint2 = controlPoint2;

            splineKeyFrame2.KeySpline = new KeySpline();
            splineKeyFrame2.KeySpline.ControlPoint1 = controlPoint1;
            splineKeyFrame2.KeySpline.ControlPoint2 = controlPoint2;

            splineKeyFrame3.KeySpline = new KeySpline();
            splineKeyFrame3.KeySpline.ControlPoint1 = controlPoint1;
            splineKeyFrame3.KeySpline.ControlPoint2 = controlPoint2;

            splineKeyFrame4.KeySpline = new KeySpline();
            splineKeyFrame4.KeySpline.ControlPoint1 = controlPoint1;
            splineKeyFrame4.KeySpline.ControlPoint2 = controlPoint2;

            storyboard.Begin();
        }
    }
}