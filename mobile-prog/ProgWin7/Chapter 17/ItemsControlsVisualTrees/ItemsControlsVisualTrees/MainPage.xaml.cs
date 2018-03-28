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

namespace ItemsControlsVisualTrees
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();

            FillItUp(itemsControl);
            FillItUp(listBox);
        }

        void FillItUp(ItemsControl itemsControl)
        {
            string[] fontFamilies = 
            {
                "Arial", "Arial Black", "Calibri", "Comic Sans MS",
                "Courier New", "Georgia", "Lucida Sans Unicode",
                "Portable User Interface", "Segoe WP", "Segoe WP Black",
                "Segoe WP Bold", "Segoe WP Light", "Segoe WP Semibold",
                "Segoe WP SemiLight", "Tahoma", "Times New Roman",
                "Trebuchet MS", "Verdana", "Webdings"
            };

            foreach (string fontFamily in fontFamilies)
                itemsControl.Items.Add(new FontFamily(fontFamily));
        }

        void OnItemsControlDumpClick(object sender, RoutedEventArgs args)
        {
            dumpTreeItemsControl.Items.Clear();
            DumpVisualTree(itemsControl, 0);
        }

        void OnListBoxDumpClick(object sender, RoutedEventArgs args)
        {
            dumpTreeItemsControl.Items.Clear();
            DumpVisualTree(listBox, 0);
        }

        void DumpVisualTree(DependencyObject parent, int indent)
        {
            TextBlock txtblk = new TextBlock();
            txtblk.Text = String.Format("{0}{1}", new string(' ', 4 * indent),
                                                  parent.GetType().Name);
            dumpTreeItemsControl.Items.Add(txtblk);

            int numChildren = VisualTreeHelper.GetChildrenCount(parent);

            for (int childIndex = 0; childIndex < numChildren; childIndex++)
            {
                DependencyObject child = VisualTreeHelper.GetChild(parent, childIndex);
                DumpVisualTree(child, indent + 1);
            }
        }
    }
}