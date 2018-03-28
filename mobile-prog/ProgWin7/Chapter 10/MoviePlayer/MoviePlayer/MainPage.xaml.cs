using System;
using System.Windows;
using System.Windows.Media;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace MoviePlayer
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();

            // Re-assign names already in the XAML file
            appbarRewindButton = this.ApplicationBar.Buttons[0] as ApplicationBarIconButton;
            appbarPlayButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;
            appbarPauseButton = this.ApplicationBar.Buttons[2] as ApplicationBarIconButton;
            appbarEndButton = this.ApplicationBar.Buttons[3] as ApplicationBarIconButton;
        }

        // ApplicationBar buttons
        void OnAppbarRewindClick(object sender, EventArgs args)
        {
            mediaElement.Position = TimeSpan.Zero;
        }

        void OnAppbarPlayClick(object sender, EventArgs args)
        {
            mediaElement.Play();
        }

        void OnAppbarPauseClick(object sender, EventArgs args)
        {
            mediaElement.Pause();
        }

        void OnAppbarEndClick(object sender, EventArgs args)
        {
            mediaElement.Position = mediaElement.NaturalDuration.TimeSpan;
        }

        // MediaElement events
        void OnMediaElementMediaFailed(object sender, ExceptionRoutedEventArgs args)
        {
            errorText.Text = args.ErrorException.Message;
        }

        void OnMediaElementMediaOpened(object sender, RoutedEventArgs args)
        {
            appbarRewindButton.IsEnabled = true;
            appbarEndButton.IsEnabled = true;
        }

        void OnMediaElementCurrentStateChanged(object sender, RoutedEventArgs args)
        {
            statusText.Text = mediaElement.CurrentState.ToString();

            if (mediaElement.CurrentState == MediaElementState.Stopped ||
                mediaElement.CurrentState == MediaElementState.Paused)
            {
                appbarPlayButton.IsEnabled = true;
                appbarPauseButton.IsEnabled = false;
            }
            else if (mediaElement.CurrentState == MediaElementState.Playing)
            {
                appbarPlayButton.IsEnabled = false;
                appbarPauseButton.IsEnabled = true;
            }
        }
    }
}
