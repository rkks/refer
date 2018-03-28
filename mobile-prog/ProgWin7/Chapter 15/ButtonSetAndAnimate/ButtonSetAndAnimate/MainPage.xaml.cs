using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace ButtonSetAndAnimate
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnSetToZeroClick(object sender, RoutedEventArgs args)
        {
            slider.Value = 0;
        }

        void OnSetToOneHundredClick(object sender, RoutedEventArgs args)
        {
            slider.Value = 100;
        }

        void OnAnimateTo50Click(object sender, RoutedEventArgs args)
        {
            storyboard.Begin();
        }

        void OnSetMaxTo40Click(object sender, RoutedEventArgs e)
        {
            slider.Maximum = 25;
        }
    }
}