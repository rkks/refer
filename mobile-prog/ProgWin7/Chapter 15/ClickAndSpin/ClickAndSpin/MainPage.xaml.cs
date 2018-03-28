using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace ClickAndSpin
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnButtonClick(object sender, RoutedEventArgs args)
        {
            Button btn = sender as Button;
            RotateTransform rotateTransform = btn.RenderTransform as RotateTransform;

            // Create and define animation
            DoubleAnimation anima = new DoubleAnimation();
            anima.From = 0;
            anima.To = 360;
            anima.Duration = new Duration(TimeSpan.FromSeconds(0.5));

            // Set attached properties
            Storyboard.SetTarget(anima, rotateTransform);
            Storyboard.SetTargetProperty(anima, new PropertyPath(RotateTransform.AngleProperty));

            // Create storyboard, add animation, and fire it up!
            Storyboard storyboard = new Storyboard();
            storyboard.Children.Add(anima);
            storyboard.Begin();
        }
    }
}