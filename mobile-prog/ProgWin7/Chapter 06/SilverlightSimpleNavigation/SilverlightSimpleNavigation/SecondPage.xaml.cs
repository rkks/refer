using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightSimpleNavigation
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
    }
}