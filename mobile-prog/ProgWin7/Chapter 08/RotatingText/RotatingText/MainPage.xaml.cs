using System;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace RotatingText
{
    public partial class MainPage : PhoneApplicationPage
    {
        TimeSpan startTime;

        public MainPage()
        {
            InitializeComponent();
            CompositionTarget.Rendering += OnCompositionTargetRendering;
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            TimeSpan renderingTime = (args as RenderingEventArgs).RenderingTime;

            if (startTime.Ticks == 0)
            {
                startTime = renderingTime;
            }
            else
            {
                TimeSpan elapsedTime = renderingTime - startTime;
                rotate.Angle = 180 * elapsedTime.TotalSeconds % 360;
            }
        }
    }
}
