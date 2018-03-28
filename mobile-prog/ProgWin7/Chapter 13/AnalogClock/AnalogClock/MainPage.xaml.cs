using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using System.Windows.Threading;

namespace AnalogClock
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            DispatcherTimer tmr = new DispatcherTimer();
            tmr.Interval = TimeSpan.FromSeconds(1);
            tmr.Tick += new EventHandler(OnTimerTick);
            tmr.Start();
        }

        void OnTimerTick(object sender, EventArgs args)
        {
            DateTime dt = DateTime.Now;

            rotateSecond.Angle = 6 * dt.Second;
            rotateMinute.Angle = 6 * dt.Minute + rotateSecond.Angle / 60;
            rotateHour.Angle = 30 * (dt.Hour % 12) + rotateMinute.Angle / 12;
        }

        void OnContentPanelSizeChanged(object sender, SizeChangedEventArgs args)
        {
            double scale = Math.Min(args.NewSize.Width, args.NewSize.Height) / 200;
            scaleClock.ScaleX = scale;
            scaleClock.ScaleY = scale;
        }
    }
}