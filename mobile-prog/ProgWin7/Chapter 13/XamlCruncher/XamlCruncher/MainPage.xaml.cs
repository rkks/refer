using System;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Phone.Controls;
using Petzold.Phone.Silverlight;

namespace XamlCruncher
{
    public partial class MainPage : PhoneApplicationPage
    {
        IsolatedStorageSettings settings = IsolatedStorageSettings.ApplicationSettings;
        
        public MainPage()
        {
            InitializeComponent();
            Application.Current.UnhandledException += OnUnhandledException;

            string text;

            if (!settings.TryGetValue<string>("text", out text))
                text = "<Grid Background=\"AliceBlue\">\r    \r</Grid>";

            txtbox.Text = text;
        }

        protected override void OnOrientationChanged(OrientationChangedEventArgs args)
        {
            uniformStack.Orientation = 
                    ((args.Orientation & PageOrientation.Portrait) == 0) ? 
                        System.Windows.Controls.Orientation.Horizontal : 
                        System.Windows.Controls.Orientation.Vertical;

            base.OnOrientationChanged(args);
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            settings["text"] = txtbox.Text;
        }

        void OnUnhandledException(object sender, 
                                  ApplicationUnhandledExceptionEventArgs args)
        {
            statusText.Text = args.ExceptionObject.Message;
            args.Handled = true;
        }

        void OnXamlCruncherTextBoxXamlResult(object sender, XamlCruncherEventArgs args)
        {
            if (args.Error != null)
            {
                statusText.Text = args.Error;
            }
            else
            {
                container.Child = args.Element;
                statusText.Text = "OK";
            }
        }
    }
}