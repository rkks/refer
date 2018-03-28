using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace ColorScroll
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            redSlider.Value = 128;
            greenSlider.Value = 128;
            blueSlider.Value = 128;
        }

        void OnSliderValueChanged(object sender, RoutedPropertyChangedEventArgs<double> args)
        {
            Color clr = Color.FromArgb(255, (byte)redSlider.Value,
                                            (byte)greenSlider.Value,
                                            (byte)blueSlider.Value);

            rect.Fill = new SolidColorBrush(clr);

            redText.Text = clr.R.ToString("X2");
            greenText.Text = clr.G.ToString("X2");
            blueText.Text = clr.B.ToString("X2");
        }

        protected override void OnOrientationChanged(OrientationChangedEventArgs args)
        {
            ContentPanel.RowDefinitions.Clear();
            ContentPanel.ColumnDefinitions.Clear();

            // Landscape
            if ((args.Orientation & PageOrientation.Landscape) != 0)
            {
                ColumnDefinition coldef = new ColumnDefinition();
                coldef.Width = new GridLength(1, GridUnitType.Star);
                ContentPanel.ColumnDefinitions.Add(coldef);

                coldef = new ColumnDefinition();
                coldef.Width = new GridLength(1, GridUnitType.Star);
                ContentPanel.ColumnDefinitions.Add(coldef);

                Grid.SetRow(controlGrid, 0);
                Grid.SetColumn(controlGrid, 1);
            }
            // Portrait
            else
            {
                RowDefinition rowdef = new RowDefinition();
                rowdef.Height = new GridLength(1, GridUnitType.Star);
                ContentPanel.RowDefinitions.Add(rowdef);

                rowdef = new RowDefinition();
                rowdef.Height = new GridLength(1, GridUnitType.Star);
                ContentPanel.RowDefinitions.Add(rowdef);

                Grid.SetRow(controlGrid, 1);
                Grid.SetColumn(controlGrid, 0);
            }
            base.OnOrientationChanged(args);
        }
    }
}
