using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace SilverlightShareData
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();

        public MainPage()
        {
            InitializeComponent();
        }

        void OnTextBlockManipulationStarted(object sender, ManipulationStartedEventArgs args)
        {
            if (ContentPanel.Background is SolidColorBrush)
                (Application.Current as App).SharedColor = 
                                (ContentPanel.Background as SolidColorBrush).Color;

            this.NavigationService.Navigate(new Uri("/SecondPage.xaml", UriKind.Relative));

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

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            Color? sharedColor = (Application.Current as App).SharedColor;

            if (sharedColor != null)
                ContentPanel.Background =
                        new SolidColorBrush(sharedColor.Value);

            base.OnNavigatedTo(args);
        }
    }
}