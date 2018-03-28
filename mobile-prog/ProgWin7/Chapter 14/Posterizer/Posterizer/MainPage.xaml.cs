using System;
using System.IO;
using System.Windows;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;
using Microsoft.Xna.Framework.Media;
using Petzold.Phone.Silverlight;

namespace Posterizer
{
    public partial class MainPage : PhoneApplicationPage, ISaveFileDialogCompleted
    {
        PhoneApplicationService appService = PhoneApplicationService.Current;
        PhotoChooserTask photoChooser = new PhotoChooserTask();
        WriteableBitmap writeableBitmap;
        byte[] jpegBits;
        int[] pixels;

        public MainPage()
        {
            InitializeComponent();

            appbarLoadButton = this.ApplicationBar.Buttons[0] as ApplicationBarIconButton;
            appbarSetBitsButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;
            appbarSaveButton = this.ApplicationBar.Buttons[2] as ApplicationBarIconButton;

            photoChooser.Completed += OnPhotoChooserCompleted;
        }

        void OnAppbarLoadClick(object sender, EventArgs args)
        {
            bitSelectDialog.Visibility = Visibility.Collapsed;
            appbarSetBitsButton.IsEnabled = false;
            appbarSaveButton.IsEnabled = false;

            photoChooser.Show();
        }

        void OnPhotoChooserCompleted(object sender, PhotoResult args)
        {
            if (args.Error == null && args.ChosenPhoto != null)
            {
                jpegBits = new byte[args.ChosenPhoto.Length];
                args.ChosenPhoto.Read(jpegBits, 0, jpegBits.Length);
                LoadBitmap(jpegBits);
            }
        }

        void LoadBitmap(byte[] jpegBits)
        {
            // Create WriteableBitmap from JPEG bits
            MemoryStream memoryStream = new MemoryStream(jpegBits);
            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.SetSource(memoryStream);
            writeableBitmap = new WriteableBitmap(bitmapImage);
            img.Source = writeableBitmap;

            // Copy pixels into field array
            pixels = new int[writeableBitmap.PixelWidth * writeableBitmap.PixelHeight];

            for (int i = 0; i < pixels.Length; i++)
                pixels[i] = writeableBitmap.Pixels[i];

            appbarSetBitsButton.IsEnabled = true;
            appbarSaveButton.IsEnabled = true;
            ApplyBitSettingsToBitmap();
        }

        void OnAppbarSetBitsClick(object sender, EventArgs args)
        {
            bitSelectDialog.Visibility = 
                bitSelectDialog.Visibility == Visibility.Collapsed ?
                    Visibility.Visible : Visibility.Collapsed;
        }

        void OnBitSelectDialogColorBitsChanged(object sender, EventArgs args)
        {
            ApplyBitSettingsToBitmap();
        }

        void ApplyBitSettingsToBitmap()
        {
            if (pixels == null || writeableBitmap == null)
                return;

            int mask = -16777216;   // ie, FF000000

            for (int clr = 0; clr < 3; clr++)
                mask |= (byte)(0xFF << (8 - bitSelectDialog.ColorBits[clr])) 
                                                << (16 - 8 * clr);

            for (int i = 0; i < pixels.Length; i++)
                writeableBitmap.Pixels[i] = mask & pixels[i];

            writeableBitmap.Invalidate();
        }

        void OnAppbarSaveClick(object sender, EventArgs args)
        {
            int fileNameNumber = 0;
            MediaLibrary mediaLib = new MediaLibrary();
            PictureCollection savedPictures = mediaLib.SavedPictures;

            foreach (Picture picture in savedPictures)
                {
                    string filename = Path.GetFileNameWithoutExtension(picture.Name);
                    int num;

                    if (filename.StartsWith("Posterizer"))
                    {
                        if (Int32.TryParse(filename.Substring(10), out num))
                            fileNameNumber = Math.Max(fileNameNumber, num);
                    }
                }

            string saveFileName = String.Format("Posterizer{0:D3}", fileNameNumber + 1);

            string uri = "/Petzold.Phone.Silverlight;component/SaveFileDialog.xaml" +
                         "?FileName=" + saveFileName;

            this.NavigationService.Navigate(new Uri(uri, UriKind.Relative));

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
            appService.State["colorBits"] = bitSelectDialog.ColorBits;

            if (jpegBits != null)
            {
                appService.State["jpegBits"] = jpegBits;
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
            if (this.NavigationContext.QueryString.ContainsKey("token"))
            {
                string token = this.NavigationContext.QueryString["token"];

                MediaLibrary mediaLib = new MediaLibrary();
                Picture picture = mediaLib.GetPictureFromToken(token);
                Stream stream = picture.GetImage();
                jpegBits = new byte[stream.Length];
                stream.Read(jpegBits, 0, jpegBits.Length);
                LoadBitmap(jpegBits);
            }
            else if (appService.State.ContainsKey("colorBits"))
            {
                int[] colorBits = (int[])appService.State["colorBits"];
                bitSelectDialog.Initialize(colorBits);
            }

            if (appService.State.ContainsKey("jpegBits"))
            {
                jpegBits = (byte[])appService.State["jpegBits"];
                LoadBitmap(jpegBits);
            }
            base.OnNavigatedTo(args);
        }
    }
}
