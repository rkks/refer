using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightTapHello2
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();
        Brush originalBrush;

        public MainPage()
        {
            InitializeComponent();
            originalBrush = txtblk.Foreground;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            if (args.OriginalSource == txtblk)
            {
                txtblk.Foreground = new SolidColorBrush(
                            Color.FromArgb(255, (byte)rand.Next(256),
                                                (byte)rand.Next(256),
                                                (byte)rand.Next(256)));
            }
            else
            {
                txtblk.Foreground = originalBrush;
            }

            args.Complete();
            base.OnManipulationStarted(args);
        }
    }
}
