using System;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;
using Microsoft.Xna.Framework.Media;

namespace SilverlightAccessLibrary
{
    public partial class MainPage : PhoneApplicationPage
    {
        MediaLibrary mediaLib = new MediaLibrary();
        Random rand = new Random();

        public MainPage()
        {
            InitializeComponent();
            GetRandomPicture();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            GetRandomPicture();

            args.Complete();
            base.OnManipulationStarted(args);
        }

        void GetRandomPicture()
        {
            PictureCollection pictures = mediaLib.Pictures;

            if (pictures.Count > 0)
            {
                int index = rand.Next(pictures.Count);
                Picture pic = pictures[index];

                BitmapImage bmp = new BitmapImage();
                bmp.SetSource(pic.GetImage());
                img.Source = bmp;

                txtblk.Text = String.Format("{0}\n{1}\n{2}", 
                                            pic.Name, pic.Album.Name, pic.Date);
            }
        }
    }
}