using System;
using System.Windows;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace SelectTwoColors
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            colorScroller1.Color = Color.FromArgb(0xFF, 0xC0, 0x80, 0x40);
            colorScroller2.Color = Color.FromArgb(0xFF, 0x40, 0x80, 0xC0);
        }

        void OnColorScrollerColorChanged(object sender, 
                                         RoutedPropertyChangedEventArgs<Color> args)
        {
            Brush brush = new SolidColorBrush(args.NewValue);

            if (sender == colorScroller1)
                rectangle.Stroke = brush;

            else if (sender == colorScroller2)
                rectangle.Fill = brush;
        }
    }
}