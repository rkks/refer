using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightPassData
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
            string destination = "/SecondPage.xaml";

            if (ContentPanel.Background is SolidColorBrush)
            {
                Color clr = (ContentPanel.Background as SolidColorBrush).Color;
                destination += String.Format("?Red={0}&Green={1}&Blue={2}",
                                             clr.R, clr.G, clr.B);
            }

            this.NavigationService.Navigate(new Uri(destination, UriKind.Relative));

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
    }
}