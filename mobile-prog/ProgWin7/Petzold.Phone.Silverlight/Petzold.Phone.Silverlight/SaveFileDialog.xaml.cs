using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace Petzold.Phone.Silverlight
{
    public partial class SaveFileDialog : PhoneApplicationPage
    {
        PhoneApplicationService appService = PhoneApplicationService.Current;
        bool okPressed;
        string filename;

        public SaveFileDialog()
        {
            InitializeComponent();
        }

        public void SetTitle(string appTitle)
        {
            ApplicationTitle.Text = appTitle;
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            saveButton.IsEnabled = txtbox.Text.Length > 0;
        }

        void OnSaveButtonClick(object sender, RoutedEventArgs args)
        {
            okPressed = true;
            filename = txtbox.Text;
            this.NavigationService.GoBack();
        }

        void OnCancelButtonClick(object sender, RoutedEventArgs args)
        {
            okPressed = false;
            this.NavigationService.GoBack();
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            if (appService.State.ContainsKey("filename"))
                txtbox.Text = appService.State["filename"] as string;

            if (appService.State.ContainsKey("apptitle"))
                ApplicationTitle.Text = appService.State["apptitle"] as string;

            if (this.NavigationContext.QueryString.ContainsKey("FileName"))
                txtbox.Text = this.NavigationContext.QueryString["FileName"];

            base.OnNavigatedTo(args);
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            if (!String.IsNullOrEmpty(txtbox.Text))
                appService.State["filename"] = txtbox.Text;

            appService.State["apptitle"] = ApplicationTitle.Text;

            if (args.Content is ISaveFileDialogCompleted)
                (args.Content as ISaveFileDialogCompleted).
                                        SaveFileDialogCompleted(okPressed, filename);

            base.OnNavigatedFrom(args);
        }
    }
}