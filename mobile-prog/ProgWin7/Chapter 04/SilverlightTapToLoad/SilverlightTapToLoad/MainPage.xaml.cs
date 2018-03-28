using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using System.Windows.Resources;
using Microsoft.Phone.Controls;

namespace SilverlightTapToLoad
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            Uri uri = new Uri("/SilverlightTapToLoad;component/Images/Hello.png", UriKind.Relative);
            StreamResourceInfo resourceInfo = Application.GetResourceStream(uri);
            BitmapImage bmp = new BitmapImage();
            bmp.SetSource(resourceInfo.Stream);
            img.Source = bmp;

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }
    }
}
