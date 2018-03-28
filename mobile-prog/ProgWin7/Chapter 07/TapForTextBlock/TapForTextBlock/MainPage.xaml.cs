using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.Phone.Controls;

namespace TapForTextBlock
{
    public partial class MainPage : PhoneApplicationPage
    {
        Random rand = new Random();

        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            TextBlock newTextBlock = new TextBlock();
            newTextBlock.Text = "Hello, Windows Phone 7!";
            newTextBlock.HorizontalAlignment = HorizontalAlignment.Left;
            newTextBlock.VerticalAlignment = VerticalAlignment.Top;
            newTextBlock.Margin = new Thickness(
                (ContentPanel.ActualWidth - txtblk.ActualWidth) * rand.NextDouble(),
                (ContentPanel.ActualHeight - txtblk.ActualHeight) * rand.NextDouble(),
                0, 0);

            ContentPanel.Children.Add(newTextBlock);

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }
    }
}
