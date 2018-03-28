using System;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace FrameBasedVsTimeBased
{
    public partial class MainPage : PhoneApplicationPage
    {
        DateTime startTime;

        public MainPage()
        {
            InitializeComponent();

            startTime = DateTime.Now;
            CompositionTarget.Rendering += OnCompositionTargetRendering;
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            // Frame-based
            rotate1.Angle = (rotate1.Angle + 0.2) % 360;

            // Time-based
            TimeSpan elapsedTime = DateTime.Now - startTime;
            rotate2.Angle = (elapsedTime.TotalMinutes * 360) % 360;
        }

        void OnButtonClick(object sender, RoutedEventArgs args)
        {
            Thread.Sleep(5000);
        }
    }
}
