using System;
using System.Net;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;

namespace SilverlightTapToDownload2
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            WebClient webClient = new WebClient();
            webClient.OpenReadCompleted += OnWebClientOpenReadCompleted;
            webClient.OpenReadAsync(new Uri("http://www.charlespetzold.com/Media/HelloWP7.jpg"));

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

        void OnWebClientOpenReadCompleted(object sender, OpenReadCompletedEventArgs args)
        {
            if (!args.Cancelled && args.Error == null)
            {
                BitmapImage bmp = new BitmapImage();
                bmp.SetSource(args.Result);
                img.Source = bmp;
            }
        }
    }
}
