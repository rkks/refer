using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;

namespace SubdivideBitmap
{
    public partial class MainPage : PhoneApplicationPage
    {
        PhotoChooserTask photoChooser = new PhotoChooserTask();
      
        public MainPage()
        {
            InitializeComponent();
            photoChooser.Completed += OnPhotoChooserCompleted;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            int dimension = (int)Math.Min(ContentPanel.ActualWidth,
                                          ContentPanel.ActualHeight) - 8;

            photoChooser.PixelHeight = dimension;
            photoChooser.PixelWidth = dimension;
            photoChooser.Show();

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

        void OnPhotoChooserCompleted(object sender, PhotoResult args)
        {
            if (args.Error != null || args.ChosenPhoto == null)
                return;

            BitmapImage bitmapImage = new BitmapImage();
            bitmapImage.SetSource(args.ChosenPhoto);

            Image imgBase = new Image();
            imgBase.Source = bitmapImage;
            imgBase.Stretch = Stretch.None;

            // Upper-left
            WriteableBitmap writeableBitmap = 
                new WriteableBitmap(bitmapImage.PixelWidth / 2,
                                    bitmapImage.PixelHeight / 2);
            writeableBitmap.Render(imgBase, null);
            writeableBitmap.Invalidate();
            imgUL.Source = writeableBitmap;

            // Upper-right
            writeableBitmap = new WriteableBitmap(bitmapImage.PixelWidth / 2,
                                                  bitmapImage.PixelHeight / 2);
            TranslateTransform translate = new TranslateTransform();
            translate.X = -bitmapImage.PixelWidth / 2;
            writeableBitmap.Render(imgBase, translate);
            writeableBitmap.Invalidate();
            imgUR.Source = writeableBitmap;

            // Lower-left
            writeableBitmap = new WriteableBitmap(bitmapImage.PixelWidth / 2,
                                                  bitmapImage.PixelHeight / 2);
            translate.X = 0;
            translate.Y = -bitmapImage.PixelHeight / 2;
            writeableBitmap.Render(imgBase, translate);
            writeableBitmap.Invalidate();
            imgLL.Source = writeableBitmap;

            // Lower-right
            writeableBitmap = new WriteableBitmap(bitmapImage.PixelWidth / 2,
                                                  bitmapImage.PixelHeight / 2);
            translate.X = -bitmapImage.PixelWidth / 2;
            writeableBitmap.Render(imgBase, translate);
            writeableBitmap.Invalidate();
            imgLR.Source = writeableBitmap;

            txtblk.Visibility = Visibility.Collapsed;
        }
    }
}