using System;
using System.Windows;
using Microsoft.Phone.Controls;

namespace PerspectiveRotation
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void RotateXClick(object sender, RoutedEventArgs args)
        {
            rotateX.Begin();
        }

        void RotateYClick(object sender, RoutedEventArgs args)
        {
            rotateY.Begin();
        }

        void RotateZClick(object sender, RoutedEventArgs args)
        {
            rotateZ.Begin();
        }
    }
}