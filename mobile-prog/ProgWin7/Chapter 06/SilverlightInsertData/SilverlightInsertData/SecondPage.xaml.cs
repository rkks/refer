using System;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace SilverlightInsertData
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
            }

            base.OnNavigatedFrom(args);
        }
    }
}