using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Threading;
using Microsoft.Xna.Framework.Media;

namespace MusicByComposer
{
    public partial class SongTitleControl : UserControl
    {
        DispatcherTimer timer = new DispatcherTimer();

        public static readonly DependencyProperty SongProperty =
            DependencyProperty.Register("Song",
                typeof(Song),
                typeof(SongTitleControl),
                new PropertyMetadata(OnSongChanged));

        public SongTitleControl()
        {
            InitializeComponent();
            timer.Interval = TimeSpan.FromSeconds(0.25);
            timer.Tick += OnTimerTick;
        }

        public Song Song
        {
            set { SetValue(SongProperty, value); }
            get { return (Song)GetValue(SongProperty); }
        }

        static void OnSongChanged(DependencyObject obj, DependencyPropertyChangedEventArgs args)
        {
            (obj as SongTitleControl).OnSongChanged(args);
        }

        void OnSongChanged(DependencyPropertyChangedEventArgs args)
        {
            if (Song != null)
                MediaPlayer.ActiveSongChanged += OnMediaPlayerActiveSongChanged;
            else
                MediaPlayer.ActiveSongChanged -= OnMediaPlayerActiveSongChanged;

            OnMediaPlayerActiveSongChanged(null, EventArgs.Empty);
        }

        void OnMediaPlayerActiveSongChanged(object sender, EventArgs args)
        {
            if (this.Song == MediaPlayer.Queue.ActiveSong)
            {
                txtblkTitle.FontWeight = FontWeights.Bold;
                txtblkTitle.Foreground = this.Resources["PhoneAccentBrush"] as Brush;
                txtblkTime.Visibility = Visibility.Visible;
                timer.Start();
            }
            else
            {
                txtblkTitle.FontWeight = FontWeights.Normal;
                txtblkTitle.Foreground = this.Resources["PhoneForegroundBrush"] as Brush;
                txtblkTime.Visibility = Visibility.Collapsed;
                timer.Stop();
            }
        }

        void OnTimerTick(object sender, EventArgs args)
        {
            TimeSpan dur = this.Song.Duration;
            TimeSpan pos = MediaPlayer.PlayPosition;

            txtblkTime.Text = String.Format("{0}:{1:D2} / {2}:{3:D2}", 
                                            (int)pos.TotalMinutes, pos.Seconds, 
                                            (int)dur.TotalMinutes, dur.Seconds);
        }
    }
}
