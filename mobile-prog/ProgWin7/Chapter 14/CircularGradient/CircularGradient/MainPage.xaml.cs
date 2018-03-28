using System;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;

namespace CircularGradient
{
    public partial class MainPage : PhoneApplicationPage
    {
        const int RADIUS = 200;

        public MainPage()
        {
            InitializeComponent();

            WriteableBitmap writeableBitmap = new WriteableBitmap(2 * RADIUS, 2 * RADIUS);

            for (int y = 0; y < writeableBitmap.PixelWidth; y++)
                for (int x = 0; x < writeableBitmap.PixelHeight; x++)
                {
                    if (Math.Sqrt(Math.Pow(x - RADIUS, 2) + Math.Pow(y - RADIUS, 2)) < RADIUS)
                    {
                        double angle = Math.Atan2(y - RADIUS, x - RADIUS);
                        byte R = (byte)(255 * Math.Abs(angle) / Math.PI);
                        byte B = (byte)(255 - R);
                        int color = 255 << 24 | R << 16 | B;
                        writeableBitmap.Pixels[y * writeableBitmap.PixelWidth + x] = color;
                    }
                }

            writeableBitmap.Invalidate();
            img.Source = writeableBitmap;
        }
    }
}