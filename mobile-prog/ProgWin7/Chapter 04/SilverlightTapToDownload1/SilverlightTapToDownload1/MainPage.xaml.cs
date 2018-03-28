using System;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;

namespace SilverlightTapToDownload1
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
            Uri uri = new Uri("http://www.charlespetzold.com/Media/HelloWP7.jpg");
            BitmapImage bmp = new BitmapImage(uri);
            img.Source = bmp;

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

    }
}
