using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace EllipseMesh
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnCanvasSizeChanged(object sender, SizeChangedEventArgs args)
        {
            canvas.Children.Clear();

            for (double y = 0; y < args.NewSize.Height; y += 75)
                for (double x = 0; x < args.NewSize.Width; x += 75)
                {
                    Ellipse ellipse = new Ellipse
                    {
                        Width = 100,
                        Height = 100,
                        Stroke = this.Resources["PhoneAccentBrush"] as Brush,
                        StrokeThickness = 10
                    };

                    Canvas.SetLeft(ellipse, x);
                    Canvas.SetTop(ellipse, y);

                    canvas.Children.Add(ellipse);
                }
        }
    }
}
