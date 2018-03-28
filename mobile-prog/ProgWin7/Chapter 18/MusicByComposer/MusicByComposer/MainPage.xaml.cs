using System;
using System.Windows.Controls;
using Microsoft.Phone.Controls;

namespace MusicByComposer
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnListBoxSelectionChanged(object sender, SelectionChangedEventArgs args)
        {
            ComposerInfo composerInfo = pivot.SelectedItem as ComposerInfo;
            int composerInfoIndex = MusicPresenter.Current.Composers.IndexOf(composerInfo);

            AlbumInfo albumInfo = (sender as ListBox).SelectedItem as AlbumInfo;
            int albumInfoIndex = composerInfo.Albums.IndexOf(albumInfo);

            // Construct URI with two indices and navigate 
            string destinationUri =
                    String.Format("/AlbumPage.xaml?ComposerInfoIndex={0}&AlbumInfoIndex={1}",
                                  composerInfoIndex, albumInfoIndex);

            this.NavigationService.Navigate(new Uri(destinationUri, UriKind.Relative));
        }
    }
}
