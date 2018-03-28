using System;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Xna.Framework.Media;

namespace MusicByComposer
{
    public partial class AlbumPage : PhoneApplicationPage
    {
        // Used for switching play and pause icons
        static Uri playButtonIconUri = new Uri("/Images/appbar.transport.play.rest.png", UriKind.Relative);
        static Uri pauseButtonIconUri = new Uri("/Images/appbar.transport.pause.rest.png", UriKind.Relative);

        int composerInfoIndex;
        int albumInfoIndex;

        public AlbumPage()
        {
            InitializeComponent();
            appbarPlayPauseButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            PhoneApplicationService.Current.State["ComposerInfoIndex"] = composerInfoIndex;
            PhoneApplicationService.Current.State["AlbumInfoIndex"] = albumInfoIndex;

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            // Navigating from MainPage
            if (this.NavigationContext.QueryString.ContainsKey("ComposerInfoIndex"))
            {
                composerInfoIndex = Int32.Parse(this.NavigationContext.QueryString["ComposerInfoIndex"]);
                albumInfoIndex = Int32.Parse(this.NavigationContext.QueryString["AlbumInfoIndex"]);
            }

            // Reactivating from tombstoning
            else if (PhoneApplicationService.Current.State.ContainsKey("ComposerInfoIndex"))
            {
                composerInfoIndex = (int)PhoneApplicationService.Current.State["ComposerInfoIndex"];
                albumInfoIndex = (int)PhoneApplicationService.Current.State["AlbumInfoIndex"];
            }

            ComposerInfo composerInfo = MusicPresenter.Current.Composers[composerInfoIndex];
            AlbumInfo albumInfo = composerInfo.Albums[albumInfoIndex];

            // Set page title and DataContext
            PageTitle.Text = composerInfo.Composer;
            this.DataContext = albumInfo;

            // Get the media state when it changes and also right now
            MediaPlayer.MediaStateChanged += OnMediaPlayerMediaStateChanged;
            OnMediaPlayerMediaStateChanged(null, EventArgs.Empty);

            base.OnNavigatedTo(args);
        }

        void OnMediaPlayerMediaStateChanged(object sender, EventArgs args)
        {
            switch (MediaPlayer.State)
            {
                case MediaState.Playing:
                    appbarPlayPauseButton.IconUri = pauseButtonIconUri;
                    break;

                case MediaState.Paused:
                case MediaState.Stopped:
                    appbarPlayPauseButton.IconUri = playButtonIconUri;
                    break;
            }
        }

        void OnAppbarPlayButtonClick(object sender, EventArgs args)
        {
            Album thisPagesAlbum = (this.DataContext as AlbumInfo).Album;

            switch (MediaPlayer.State)
            {
                // The MediaPlayer is currently playing so pause it.
                case MediaState.Playing:
                    MediaPlayer.Pause();
                    break;

                // The MediaPlayer is currently paused...
                case MediaState.Paused:
                    MediaQueue queue = MediaPlayer.Queue;

                    // so if we're on the same page as the paused song, resume it.
                    if (queue.ActiveSong != null &&
                        queue.ActiveSong.Album == thisPagesAlbum)
                    {
                        MediaPlayer.Resume();
                    }
                    // Otherwise, start playing this page's album.
                    else
                    {
                        goto case MediaState.Stopped;
                    }
                    break;

                // The MediaPlayer is stopped, so play this page's album.
                case MediaState.Stopped:
                    MediaPlayer.Play(thisPagesAlbum.Songs);
                    break;
            }
        }

        void OnAppbarPreviousButtonClick(object sender, EventArgs args)
        {
            MediaPlayer.MovePrevious();
        }

        void OnAppbarNextButtonClick(object sender, EventArgs args)
        {
            MediaPlayer.MoveNext();
        }
    }
}