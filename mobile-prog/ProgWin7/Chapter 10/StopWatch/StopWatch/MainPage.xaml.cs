using System;
using System.Diagnostics;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using System.Globalization;

namespace StopWatch
{
    public partial class MainPage : PhoneApplicationPage
    {
        Stopwatch stopwatch = new Stopwatch();
        TimeSpan suspensionAdjustment = new TimeSpan();
        string decimalSeparator = NumberFormatInfo.CurrentInfo.NumberDecimalSeparator;

        public MainPage()
        {
            InitializeComponent();
            DisplayTime();
        }

        void OnToggleButtonChecked(object sender, RoutedEventArgs e)
        {
            if ((bool)startStopToggle.IsChecked)
            {
                stopwatch.Start();
                startStopToggle.Content = "Stop";
                CompositionTarget.Rendering += OnCompositionTargetRendering;
            }
            else
            {
                stopwatch.Stop();
                startStopToggle.Content = "Start";
                CompositionTarget.Rendering -= OnCompositionTargetRendering;
            }
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            DisplayTime();
        }

        void OnAppbarResetClick(object sender, EventArgs args)
        {
            stopwatch.Reset();
            startStopToggle.IsChecked = false;
            suspensionAdjustment = new TimeSpan();
            DisplayTime();
        }

        void OnAppbarFormatClick(object sender, EventArgs args)
        {
            disableRect.Visibility = Visibility.Visible;
            formatDialog.Visibility = Visibility.Visible;

            // Initialize radio buttons
            ElapsedTimeFormat currentFormat = (Application.Current as App).ElapsedTimeFormat;

            foreach (UIElement child in radioButtonPanel.Children)
            {
                RadioButton radio = child as RadioButton;
                ElapsedTimeFormat radioFormat = 
                    (ElapsedTimeFormat)Enum.Parse(typeof(ElapsedTimeFormat), 
                                                  radio.Tag as string, true);
                radio.IsChecked = currentFormat == radioFormat;
            }
        }

        void OnOkButtonClick(object sender, RoutedEventArgs args)
        {
            foreach (UIElement child in radioButtonPanel.Children)
            {
                RadioButton radio = child as RadioButton;
                if ((bool)radio.IsChecked)
                    (Application.Current as App).ElapsedTimeFormat = 
                        (ElapsedTimeFormat)Enum.Parse(typeof(ElapsedTimeFormat), 
                                                      radio.Tag as string, true);
            }

            OnCancelButtonClick(sender, args);
        }

        void OnCancelButtonClick(object sender, RoutedEventArgs args)
        {
            disableRect.Visibility = Visibility.Collapsed;
            formatDialog.Visibility = Visibility.Collapsed;
            DisplayTime();
        }

        void DisplayTime()
        {
            TimeSpan elapsedTime = stopwatch.Elapsed + suspensionAdjustment;
            string str = null;

            switch ((Application.Current as App).ElapsedTimeFormat)
            {
                case ElapsedTimeFormat.HourMinuteSecond:
                    str = String.Format("{0:D2} {1:D2} {2:D2}{3}{4:D2}",
                                        elapsedTime.Hours, elapsedTime.Minutes,
                                        elapsedTime.Seconds, decimalSeparator,
                                        elapsedTime.Milliseconds / 10);
                    break;

                case ElapsedTimeFormat.Seconds:
                    str = String.Format("{0:F2} sec", elapsedTime.TotalSeconds);
                    break;

                case ElapsedTimeFormat.Milliseconds:
                    str = String.Format("{0:F0} msec", elapsedTime.TotalMilliseconds);
                    break;
            }

            elapsedText.Text = str;
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            PhoneApplicationService service = PhoneApplicationService.Current;
            service.State["stopWatchRunning"] = (bool)startStopToggle.IsChecked;
            service.State["suspensionAdjustment"] = suspensionAdjustment + stopwatch.Elapsed;
            service.State["tombstoneBeginTime"] = DateTime.Now;

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            PhoneApplicationService service = PhoneApplicationService.Current;

            if (service.State.ContainsKey("stopWatchRunning"))
            {
                suspensionAdjustment = (TimeSpan)service.State["suspensionAdjustment"];

                if ((bool)service.State["stopWatchRunning"])
                {
                    suspensionAdjustment += DateTime.Now - 
                                                (DateTime)service.State["tombstoneBeginTime"];
                    startStopToggle.IsChecked = true;
                }
                else
                {
                    DisplayTime();
                }
            }
            base.OnNavigatedTo(args);
        }
    }
}
