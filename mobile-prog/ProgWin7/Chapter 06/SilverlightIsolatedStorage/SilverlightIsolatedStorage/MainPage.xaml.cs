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
using System.Windows.Navigation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace SilverlightIsolatedStorage
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();
        int numTaps = 0;
        PhoneApplicationService appService = PhoneApplicationService.Current;

        public MainPage()
        {
            InitializeComponent();
            UpdatePageTitle(numTaps);

            // Access App class for isolated storage setting
            Brush brush = (Application.Current as App).BackgroundBrush;

            if (brush != null)
                ContentPanel.Background = brush;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            SolidColorBrush brush =
                new SolidColorBrush(Color.FromArgb(255, (byte)rand.Next(256),
                                                        (byte)rand.Next(256),
                                                        (byte)rand.Next(256)));
            ContentPanel.Background = brush;

            // Save to App class for isolated storage setting
            (Application.Current as App).BackgroundBrush = brush;

            UpdatePageTitle(++numTaps);

            args.Complete();
            base.OnManipulationStarted(args);
        }

        void UpdatePageTitle(int numTaps)
        {
            PageTitle.Text = String.Format("{0} taps total", numTaps);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            appService.State["numTaps"] = numTaps;

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            // Load numTaps
            if (appService.State.ContainsKey("numTaps"))
            {
                numTaps = (int)appService.State["numTaps"];
                UpdatePageTitle(numTaps);
            }
        }
    }
}
