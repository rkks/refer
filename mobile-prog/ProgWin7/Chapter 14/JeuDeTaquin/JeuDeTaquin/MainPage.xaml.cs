using System;
using System.IO;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;

namespace JeuDeTaquin
{
    public partial class MainPage : PhoneApplicationPage
    {
        const int HORZ_TILES = 4;
        const int VERT_TILES = 4;
        const int MARGIN = 2;

        PhoneApplicationService appService = PhoneApplicationService.Current;
        PhotoChooserTask photoChooser = new PhotoChooserTask();
        Random rand = new Random();

        Image[,] tileImages = new Image[VERT_TILES, HORZ_TILES];
        bool haveValidTileImages;
        int emptyRow, emptyCol;
        int scrambleCountdown;

        public MainPage()
        {
            InitializeComponent();

            for (int col = 0; col < HORZ_TILES; col++)
            {
                ColumnDefinition coldef = new ColumnDefinition();
                coldef.Width = new GridLength(1, GridUnitType.Star);
                playGrid.ColumnDefinitions.Add(coldef);
            }

            for (int row = 0; row < VERT_TILES; row++)
            {
                RowDefinition rowdef = new RowDefinition();
                rowdef.Height = new GridLength(1, GridUnitType.Star);
                playGrid.RowDefinitions.Add(rowdef);
            }

            appbarScrambleButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;

            photoChooser.Completed += OnPhotoChooserCompleted;
        }

        void OnLoadClick(object sender, EventArgs args)
        {
            int tileSize = (int)Math.Min(ContentPanel.ActualWidth / HORZ_TILES,
                                         ContentPanel.ActualHeight / VERT_TILES) 
                           - 2 * MARGIN;

            photoChooser.PixelWidth = tileSize * HORZ_TILES;
            photoChooser.PixelHeight = tileSize * VERT_TILES;
            photoChooser.Show();
        }

        void OnScrambleClick(object sender, EventArgs args)
        {
            scrambleCountdown = 10 * VERT_TILES * HORZ_TILES;
            scrambleButton.IsEnabled = false;
            appbarScrambleButton.IsEnabled = false;
            CompositionTarget.Rendering += OnCompositionTargetRendering;
        }

        void OnCompositionTargetRendering(object sender, EventArgs args)
        {
            MoveTile(tileImages[emptyRow, rand.Next(HORZ_TILES)]);
            MoveTile(tileImages[rand.Next(VERT_TILES), emptyCol]);

            if (--scrambleCountdown == 0)
            {
                CompositionTarget.Rendering -= OnCompositionTargetRendering;
                scrambleButton.IsEnabled = true;
                appbarScrambleButton.IsEnabled = true;
            }
        }

        void OnPhotoChooserCompleted(object sender, PhotoResult args)
        {
            if (args.Error == null && args.ChosenPhoto != null)
            {
                BitmapImage bitmapImage = new BitmapImage();
                bitmapImage.SetSource(args.ChosenPhoto);
                WriteableBitmap writeableBitmap = new WriteableBitmap(bitmapImage);
                int tileSize = writeableBitmap.PixelWidth / HORZ_TILES;

                emptyCol = HORZ_TILES - 1;
                emptyRow = VERT_TILES - 1;

                for (int row = 0; row < VERT_TILES; row++)
                    for (int col = 0; col < HORZ_TILES; col++)
                        if (row != emptyRow || col != emptyCol)
                        {
                            WriteableBitmap tile = new WriteableBitmap(tileSize, tileSize);

                            for (int y = 0; y < tileSize; y++)
                                for (int x = 0; x < tileSize; x++)
                                {
                                    int yBit = row * tileSize + y;
                                    int xBit = col * tileSize + x;

                                    tile.Pixels[y * tileSize + x] =
                                        writeableBitmap.Pixels[yBit * writeableBitmap.PixelWidth + xBit];
                                }
                            GenerateImageTile(tile, row, col);
                        }

                haveValidTileImages = true;
                scrambleButton.IsEnabled = true;
                appbarScrambleButton.IsEnabled = true;
            }
        }

        void GenerateImageTile(BitmapSource tile, int row, int col)
        {
            Image img = new Image();
            img.Stretch = Stretch.None;
            img.Source = tile;
            img.Margin = new Thickness(MARGIN);
            tileImages[row, col] = img;

            Grid.SetRow(img, row);
            Grid.SetColumn(img, col);
            playGrid.Children.Add(img);
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            if (args.OriginalSource is Image)
            {
                Image img = args.OriginalSource as Image;
                MoveTile(img);
                args.Complete();
                args.Handled = true;
            }
            base.OnManipulationStarted(args);
        }

        void MoveTile(Image img)
        {
            int touchedRow = -1, touchedCol = -1;

            for (int y = 0; y < VERT_TILES; y++)
                for (int x = 0; x < HORZ_TILES; x++)
                    if (tileImages[y, x] == img)
                    {
                        touchedRow = y;
                        touchedCol = x;
                    }

            if (touchedRow == emptyRow)
            {
                int sign = Math.Sign(touchedCol - emptyCol);

                for (int x = emptyCol; x != touchedCol; x += sign)
                {
                    tileImages[touchedRow, x] = tileImages[touchedRow, x + sign];
                    Grid.SetColumn(tileImages[touchedRow, x], x);
                }
                tileImages[touchedRow, touchedCol] = null;
                emptyCol = touchedCol;
            }
            else if (touchedCol == emptyCol)
            {
                int sign = Math.Sign(touchedRow - emptyRow);

                for (int y = emptyRow; y != touchedRow; y += sign)
                {
                    tileImages[y, touchedCol] = tileImages[y + sign, touchedCol];
                    Grid.SetRow(tileImages[y, touchedCol], y);
                }
                tileImages[touchedRow, touchedCol] = null;
                emptyRow = touchedRow;
            }
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            appService.State["haveValidTileImages"] = haveValidTileImages;

            if (haveValidTileImages)
            {
                appService.State["emptyRow"] = emptyRow;
                appService.State["emptyCol"] = emptyCol;

                for (int row = 0; row < VERT_TILES; row++)
                    for (int col = 0; col < HORZ_TILES; col++)
                        if (col != emptyCol || row != emptyRow)
                        {
                            WriteableBitmap tile = tileImages[row, col].Source as WriteableBitmap;
                            MemoryStream stream = new MemoryStream();
                            tile.SaveJpeg(stream, tile.PixelWidth, tile.PixelHeight, 0, 75);
                            appService.State[TileKey(row, col)] = stream.GetBuffer();
                        }
            }
            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            object objHaveValidTileImages;

            if (appService.State.TryGetValue("haveValidTileImages", out objHaveValidTileImages) &&
                (bool)objHaveValidTileImages)
            {
                emptyRow = (int)appService.State["emptyRow"];
                emptyCol = (int)appService.State["emptyCol"];

                for (int row = 0; row < VERT_TILES; row++)
                    for (int col = 0; col < HORZ_TILES; col++)
                        if (col != emptyCol || row != emptyRow)
                        {
                            byte[] buffer = (byte[])appService.State[TileKey(row, col)];
                            MemoryStream stream = new MemoryStream(buffer);
                            BitmapImage bitmapImage = new BitmapImage();
                            bitmapImage.SetSource(stream);
                            WriteableBitmap tile = new WriteableBitmap(bitmapImage);
                            GenerateImageTile(tile, row, col);
                        }

                haveValidTileImages = true;
                appbarScrambleButton.IsEnabled = true;
            }

            base.OnNavigatedTo(args);
        }

        string TileKey(int row, int col)
        {
            return String.Format("tile {0} {1}", row, col);
        }
    }
}