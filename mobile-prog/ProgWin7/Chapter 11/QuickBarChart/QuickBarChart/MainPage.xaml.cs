using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace QuickBarChart
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();

        public MainPage()
        {
            InitializeComponent();
        }

        void OnButtonClick(object sender, RoutedEventArgs args)
        {
            for (int i = 0; i < 10; i++)
            {
                Rectangle rect = new Rectangle();
                rect.Fill = this.Resources["PhoneAccentBrush"] as Brush;
                rect.VerticalAlignment = VerticalAlignment.Bottom;
                rect.Height = barChartPanel.ActualHeight * rand.NextDouble();
                rect.Margin = new Thickness(0, 0, 0.5, 0);

                barChartPanel.Children.Add(rect);
            }

            txtblk.Text = barChartPanel.Children.Count.ToString();
        }
    }
}