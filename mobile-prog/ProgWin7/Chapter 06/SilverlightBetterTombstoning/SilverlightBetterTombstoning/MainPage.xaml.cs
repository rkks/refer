using System;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace SilverlightBetterTombstoning
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
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            ContentPanel.Background =
                new SolidColorBrush(Color.FromArgb(255, (byte)rand.Next(256),
                                                        (byte)rand.Next(256),
                                                        (byte)rand.Next(256)));
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

            if (ContentPanel.Background is SolidColorBrush)
            {
                appService.State["backgroundColor"] = 
                                    (ContentPanel.Background as SolidColorBrush).Color;
            }

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

            // Load background color
            object obj;

            if (appService.State.TryGetValue("backgroundColor", out obj))
                ContentPanel.Background = new SolidColorBrush((Color)obj);

            base.OnNavigatedTo(args);
        }
    }
}
