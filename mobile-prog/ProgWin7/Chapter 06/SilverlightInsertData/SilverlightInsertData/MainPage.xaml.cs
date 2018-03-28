using System;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace SilverlightInsertData
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        public Color? ReturnedColor { set; get; }

        void OnTextBlockManipulationStarted(object sender, ManipulationStartedEventArgs args)
        {
            this.NavigationService.Navigate(new Uri("/SecondPage.xaml", UriKind.Relative));

            args.Complete();
            args.Handled = true;
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            if (ReturnedColor != null)
                ContentPanel.Background =
                        new SolidColorBrush(ReturnedColor.Value);

            base.OnNavigatedTo(args);
        }
    }
}