using System;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace SilverlightRetainData
{
    public partial class SecondPage : PhoneApplicationPage
    {
        Random rand = new Random();

        public SecondPage()
        {
            InitializeComponent();
        }

        void OnTextBlockManipulationStarted(object sender, ManipulationStartedEventArgs args)
        {
            this.NavigationService.GoBack();

            args.Complete();
            args.Handled = true;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            ContentPanel.Background = new SolidColorBrush(
                Color.FromArgb(255, (byte)rand.Next(255),
                                    (byte)rand.Next(255),
                                    (byte)rand.Next(255)));

            base.OnManipulationStarted(args);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            if (ContentPanel.Background is SolidColorBrush)
            {
                Color clr = (ContentPanel.Background as SolidColorBrush).Color;

                if (args.Content is MainPage)
                    (args.Content as MainPage).ReturnedColor = clr;

                // Save color
                PhoneApplicationService.Current.State["Color"] = clr;
            }

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            // Retrieve color
            if (PhoneApplicationService.Current.State.ContainsKey("Color"))
            {
                Color clr = (Color)PhoneApplicationService.Current.State["Color"];
                ContentPanel.Background = new SolidColorBrush(clr);
            }

            base.OnNavigatedTo(args);
        }
    }
}