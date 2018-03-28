using System;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Devices.Sensors;
using Microsoft.Phone.Controls;

namespace SilverlightAccelerometer
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            Accelerometer acc = new Accelerometer();
            acc.ReadingChanged += OnAccelerometerReadingChanged;

            try
            {
                acc.Start();
            }
            catch (Exception exc)
            {
                txtblk.Text = exc.Message;
            }
        }

        void OnAccelerometerReadingChanged(object sender, AccelerometerReadingEventArgs args)
        {
            string str = String.Format("X = {0:F2}\n" +
                                       "Y = {1:F2}\n" +
                                       "Z = {2:F2}\n\n" +
                                       "Magnitude = {3:F2}\n\n" +
                                       "{4}", 
                                       args.X, args.Y, args.Z, 
                                       Math.Sqrt(args.X * args.X + args.Y * args.Y + 
                                                                   args.Z * args.Z),                                       
                                       args.Timestamp);

            if (txtblk.CheckAccess())
            {
                SetTextBlockText(txtblk, str);
            }
            else
            {
                txtblk.Dispatcher.BeginInvoke(new SetTextBlockTextDelegate(SetTextBlockText), 
                                              txtblk, str);
            }
        }

        delegate void SetTextBlockTextDelegate(TextBlock txtblk, string text);

        void SetTextBlockText(TextBlock txtblk, string text)
        {
            txtblk.Text = text;
        }
    }
}
