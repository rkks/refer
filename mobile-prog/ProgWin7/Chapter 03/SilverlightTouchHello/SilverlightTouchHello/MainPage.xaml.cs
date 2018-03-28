using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightTouchHello
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();
        Brush originalBrush;
        
        public MainPage()
        {
            InitializeComponent();
            originalBrush = txtblk.Foreground;
            Touch.FrameReported += OnTouchFrameReported;
        }

        void OnTouchFrameReported(object sender, TouchFrameEventArgs args)
        {
            TouchPoint primaryTouchPoint = args.GetPrimaryTouchPoint(null);

            if (primaryTouchPoint != null && primaryTouchPoint.Action == TouchAction.Down)
            {
                if (primaryTouchPoint.TouchDevice.DirectlyOver == txtblk)
                {
                    txtblk.Foreground = new SolidColorBrush(
                                Color.FromArgb(255, (byte)rand.Next(256),
                                                    (byte)rand.Next(256),
                                                    (byte)rand.Next(256)));
                }
                else
                {
                    txtblk.Foreground = originalBrush;
                }
            }
        }
    }
}
