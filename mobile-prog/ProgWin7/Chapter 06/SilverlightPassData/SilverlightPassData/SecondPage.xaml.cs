using System;
using System.Collections;
using System.Collections.Generic;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace SilverlightPassData
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

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            IDictionary<string, string> parameters = this.NavigationContext.QueryString;

            if (parameters.ContainsKey("Red"))
            {
                byte R = Byte.Parse(parameters["Red"]);
                byte G = Byte.Parse(parameters["Green"]);
                byte B = Byte.Parse(parameters["Blue"]);

                ContentPanel.Background = 
                    new SolidColorBrush(Color.FromArgb(255, R, G, B));
            }

            base.OnNavigatedTo(args);
        }
    }
}