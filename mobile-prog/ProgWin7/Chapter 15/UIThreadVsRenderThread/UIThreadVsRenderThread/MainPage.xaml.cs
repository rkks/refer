using System;
using System.Threading;
using System.Windows;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace UIThreadVsRenderThread
{
    public partial class MainPage : PhoneApplicationPage
    {
        DateTime startTime;

        public MainPage()
        {
            InitializeComponent();

            storyboard.Begin();
            startTime = DateTime.Now;
            CompositionTarget.Rendering += OnCompositionTargetRendering;
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            TimeSpan elapsedTime = DateTime.Now - startTime;
            rotate1.Angle = (elapsedTime.TotalMinutes * 360) % 360;
        }

        void OnButtonClick(object sender, RoutedEventArgs args)
        {
            Thread.Sleep(5000);
        }
    }
}