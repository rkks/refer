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

namespace XamlClickAndSpin
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }

        void OnButtonClick(object sender, RoutedEventArgs args)
        {
            if (sender == btn1)
                storyboard1.Begin();

            else if (sender == btn2)
            {
                storyboard1.Stop();
                DoubleAnimation anima = storyboard1.Children[0] as DoubleAnimation;
                Storyboard.SetTarget(anima, btn2);
                storyboard1.Begin();
            }

            else if (sender == btn3)
                storyboard3.Begin();
        }
    }
}