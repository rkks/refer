using System;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;

namespace RecursivePageCaptures
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            img.Source = new WriteableBitmap(this, null);

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }
    }
}
