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

namespace SilverlightWhatSize
{
    public partial class MainPage : PhoneApplicationPage
    {
        // Constructor
        public MainPage()
        {
            InitializeComponent();
        }

        private void ContentPanel_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            txtblk.Text = String.Format("ContentPanel size: {0}\n" +
                                        "TitlePanel size: {1}\n" +
                                        "LayoutRoot size: {2}\n" +
                                        "MainPage size: {3}\n" +
                                        "Frame size: {4}",
                                        e.NewSize,
                                        new Size(TitlePanel.ActualWidth, TitlePanel.ActualHeight),
                                        new Size(LayoutRoot.ActualWidth, LayoutRoot.ActualHeight),
                                        new Size(this.ActualWidth, this.ActualHeight),
                                        Application.Current.RootVisual.RenderSize);

        }
    }
}