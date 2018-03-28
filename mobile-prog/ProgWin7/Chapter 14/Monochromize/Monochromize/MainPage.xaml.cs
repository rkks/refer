using System;
using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;
using Microsoft.Phone.Shell;
using Microsoft.Xna.Framework.Media;
using Petzold.Phone.Silverlight;

namespace Monochromize
{
    public partial class MainPage : PhoneApplicationPage, ISaveFileDialogCompleted
    {
        PhoneApplicationService appService = PhoneApplicationService.Current;
        PhotoChooserTask photoChooser = new PhotoChooserTask();
        WriteableBitmap writeableBitmap;

        public MainPage()
        {
            InitializeComponent();

            appbarLoadButton = this.ApplicationBar.Buttons[0] as ApplicationBarIconButton;
            appbarSaveButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;

            photoChooser.Completed += OnPhotoChooserCompleted;
        }

        void OnAppbarLoadClick(object sender, EventArgs args)
        {
            appbarSaveButton.IsEnabled = false;
            photoChooser.Show();
        }

        void OnPhotoChooserCompleted(object sender, PhotoResult args)
        {
            if (args.Error == null && args.ChosenPhoto != null)
            {
                BitmapImage bitmapImage = new BitmapImage();
                bitmapImage.SetSource(args.ChosenPhoto);
                writeableBitmap = new WriteableBitmap(bitmapImage);

                // Monochromize
                for (int pixel = 0; pixel < writeableBitmap.Pixels.Length; pixel++)
                {
                    int color = writeableBitmap.Pixels[pixel];
                    byte A = (byte)(color & 0xFF000000 >> 24);
                    byte R = (byte)(color & 0x00FF0000 >> 16);
                    byte G = (byte)(color & 0x0000FF00 >> 8);
                    byte B = (byte)(color & 0x000000FF);
                    byte gray = (byte)(0.30 * R + 0.59 * G + 0.11 * B);

                    color = (A << 24) | (gray << 16) | (gray << 8) | gray;
                    writeableBitmap.Pixels[pixel] = color;
                }
                img.Source = writeableBitmap;
                appbarSaveButton.IsEnabled = true;
            }
        }

        void OnAppbarSaveClick(object sender, EventArgs args)
        {
            this.NavigationService.Navigate(
                    new Uri("/Petzold.Phone.Silverlight;component/SaveFileDialog.xaml", 
                        UriKind.Relative));
        }

        public void SaveFileDialogCompleted(bool okPressed, string filename)
        {
            if (okPressed)
            {
                MemoryStream memoryStream = new MemoryStream();
                writeableBitmap.SaveJpeg(memoryStream, writeableBitmap.PixelWidth, 
                                                       writeableBitmap.PixelHeight, 0, 75);
                memoryStream.Position = 0;

                MediaLibrary mediaLib = new MediaLibrary();
                mediaLib.SavePicture(filename, memoryStream);
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            if (writeableBitmap != null)
            {
                MemoryStream stream = new MemoryStream();
                writeableBitmap.SaveJpeg(stream, writeableBitmap.PixelWidth,
                                                 writeableBitmap.PixelHeight, 0, 75);
                appService.State["jpegBits"] = stream.GetBuffer();
            }

            if (args.Content is SaveFileDialog)
            {
                SaveFileDialog page = args.Content as SaveFileDialog;
                page.SetTitle(ApplicationTitle.Text);
            }

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            if (appService.State.ContainsKey("jpegBits"))
            {
                byte[] bitmapBits = (byte[])appService.State["jpegBits"];
                MemoryStream stream = new MemoryStream(bitmapBits);
                BitmapImage bitmapImage = new BitmapImage();
                bitmapImage.SetSource(stream);
                writeableBitmap = new WriteableBitmap(bitmapImage);
                img.Source = writeableBitmap;
                appbarSaveButton.IsEnabled = true;
            }

            base.OnNavigatedTo(args);
        }
    }
}