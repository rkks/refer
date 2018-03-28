using System;
using System.Windows;
using Microsoft.Phone.Controls;
using Petzold.Phone.Silverlight;

namespace TapSlideToggleDemo
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
            slideToggle2.IsChecked = true;
        }

        void OnSlideToggle1Checked(object sender, RoutedEventArgs args)
        {
            TapSlideToggle toggle = sender as TapSlideToggle;
            option1TextBlock.Text = toggle.IsChecked ? "on" : "off";
        }

        void OnSlideToggle2Checked(object sender, RoutedEventArgs args)
        {
            TapSlideToggle toggle = sender as TapSlideToggle;
            option2TextBlock.Text = toggle.IsChecked ? "on" : "off";
        }
    }
}