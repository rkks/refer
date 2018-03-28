using System;
using System.Windows;
using System.Windows.Controls;
using Microsoft.Phone.Controls;

namespace QuickNotes
{
    public partial class MainPage : PhoneApplicationPage
    {
        QuickNotesSettings appSettings = (Application.Current as App).AppSettings;
        
        public MainPage()
        {
            InitializeComponent();

            txtbox.Text = appSettings.Text;
            txtbox.FontSize = appSettings.FontSize;
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            appSettings.Text = txtbox.Text;
        }

        void OnAppBarSmallerFontClick(object sender, EventArgs args)
        {
            txtbox.FontSize = Math.Max(12, txtbox.FontSize - 1);
            appSettings.FontSize = txtbox.FontSize;
        }

        void OnAppBarLargerFontClick(object sender, EventArgs args)
        {
            txtbox.FontSize = Math.Min(48, txtbox.FontSize + 2);
            appSettings.FontSize = txtbox.FontSize;
        }
    }
}
