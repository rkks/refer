using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace VectorToRaster
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            // Get PathGeometry from resource
            Path catPath = this.Resources["catPath"] as Path;
            PathGeometry pathGeometry = catPath.Data as PathGeometry;
            catPath.Data = null;

            // Get geometry bounds
            Rect bounds = pathGeometry.Bounds;

            // Create new path for rendering on bitmap
            Path newPath = new Path
            {
                Stroke = this.Resources["PhoneForegroundBrush"] as Brush,
                StrokeThickness = 5,
                Data = pathGeometry,
            };

            // Create the WriteableBitmap
            WriteableBitmap writeableBitmap = 
                new WriteableBitmap((int)(bounds.Width + newPath.StrokeThickness),
                                    (int)(bounds.Height + newPath.StrokeThickness));

            // Color the background of the bitmap
            Color baseColor = (Color)this.Resources["PhoneAccentColor"];

            // Treat the bitmap as an ellipse:
            //  radiusX and radiusY are also the centers!
            double radiusX = writeableBitmap.PixelWidth / 2.0;
            double radiusY = writeableBitmap.PixelHeight / 2.0;

            for (int y = 0; y < writeableBitmap.PixelHeight; y++)
                for (int x = 0; x < writeableBitmap.PixelWidth; x++)
                {
                    double angle = Math.Atan2(y - radiusY, x - radiusX);
                    double ellipseX = radiusX * (1 + Math.Cos(angle));
                    double ellipseY = radiusY * (1 + Math.Sin(angle));

                    double ellipseToCenter = 
                        Math.Sqrt(Math.Pow(ellipseX - radiusX, 2) +
                                  Math.Pow(ellipseY - radiusY, 2));

                    double pointToCenter =
                        Math.Sqrt(Math.Pow(x - radiusX, 2) + Math.Pow(y - radiusY, 2));

                    double opacity = Math.Min(1, pointToCenter / ellipseToCenter);

                    byte A = (byte)(opacity * 255);
                    byte R = (byte)(opacity * baseColor.R);
                    byte G = (byte)(opacity * baseColor.G);
                    byte B = (byte)(opacity * baseColor.B);

                    int color = A << 24 | R << 16 | G << 8 | B;

                    writeableBitmap.Pixels[y * writeableBitmap.PixelWidth + x] = color;
                }

            writeableBitmap.Invalidate();

            // Find transform to move Path to edges
            TranslateTransform translate = new TranslateTransform
            {
                X = -bounds.X + newPath.StrokeThickness / 2,
                Y = -bounds.Y + newPath.StrokeThickness / 2
            };

            writeableBitmap.Render(newPath, translate);
            writeableBitmap.Invalidate();

            // Set bitmap to Image element
            img.Source = writeableBitmap;
        }
    }
}