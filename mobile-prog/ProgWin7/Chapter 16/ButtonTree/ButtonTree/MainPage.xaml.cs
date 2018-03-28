using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace ButtonTree
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
            stackPanel.Children.Clear();
            DumpVisualTree(btn, 0);
        }

        void DumpVisualTree(DependencyObject parent, int indent)
        {
            TextBlock txtblk = new TextBlock();
            txtblk.Text = String.Format("{0}{1}", new string(' ', 4 * indent),
                                                  parent.GetType().Name);
            stackPanel.Children.Add(txtblk);

            int numChildren = VisualTreeHelper.GetChildrenCount(parent);

            for (int childIndex = 0; childIndex < numChildren; childIndex++)
            {
                DependencyObject child = VisualTreeHelper.GetChild(parent, childIndex);
                DumpVisualTree(child, indent + 1);
            }
        }
    }
}