using System;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightTapHello3
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

        void OnTextBlockManipulationStarted(object sender,
                                            ManipulationStartedEventArgs args)
        {
            txtblk.Foreground = new SolidColorBrush(
                        Color.FromArgb(255, (byte)rand.Next(256),
                                            (byte)rand.Next(256),
                                            (byte)rand.Next(256)));
            args.Complete();
            args.Handled = true;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            txtblk.Foreground = originalBrush;

            args.Complete();
            base.OnManipulationStarted(args);
        }
    }
}
