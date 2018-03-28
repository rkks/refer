using System;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SilverlightTapHello1
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();

        public MainPage()
        {
            InitializeComponent();
        }
        void OnTextBlockManipulationStarted(object sender,
                                            ManipulationStartedEventArgs args)
        {
            TextBlock txtblk = sender as TextBlock;

            Color clr = Color.FromArgb(255, (byte)rand.Next(256),
                                            (byte)rand.Next(256),
                                            (byte)rand.Next(256));

            txtblk.Foreground = new SolidColorBrush(clr);

            args.Complete();
        }
    }
}
