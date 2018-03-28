using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace GpaBarChart
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            Touch.FrameReported += OnTouchFrameReported;
        }

        void OnTouchFrameReported(object sender, TouchFrameEventArgs args)
        {
            TouchPoint touchPoint = args.GetPrimaryTouchPoint(this);

            if (touchPoint != null && touchPoint.Action == TouchAction.Down)
                args.SuspendMousePromotionUntilTouchUp();

            if (touchPoint != null && touchPoint.TouchDevice.DirectlyOver is Rectangle)
            {
                Rectangle rectangle = (touchPoint.TouchDevice.DirectlyOver as Rectangle);

                // This DataContext is an object of type Student
                object dataContext = rectangle.DataContext;
                studentDisplay.DataContext = dataContext;

                if (touchPoint.Action == TouchAction.Down)
                    studentDisplay.Visibility = Visibility.Visible;

                else if (touchPoint.Action == TouchAction.Up)
                    studentDisplay.Visibility = Visibility.Collapsed;
            }
        }
    }
}
