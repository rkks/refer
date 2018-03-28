using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightFlawedTombstoning
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();
        int numTaps = 0;

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
    }
}
