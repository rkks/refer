using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Microsoft.Phone.Tasks;

namespace OneTimeText
{
    public partial class MainPage : PhoneApplicationPage
    {
        PhoneApplicationService appService = PhoneApplicationService.Current;
        SmsComposeTask smsTask;
        
        public MainPage()
        {
            InitializeComponent();

            smsTask = new SmsComposeTask();
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            if (sender == bodyTextBox)
                charCountText.Text = String.Format("{0}/160", bodyTextBox.Text.Length);

            sendButton.IsEnabled = toTextBox.Text.Length > 0 && bodyTextBox.Text.Length > 0;
        }

        void OnSendButtonClick(object sender, RoutedEventArgs e)
        {
            smsTask.To = toTextBox.Text;
            smsTask.Body = bodyTextBox.Text;
            smsTask.Show();
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            appService.State["toText"] = toTextBox.Text;
            appService.State["bodyText"] = bodyTextBox.Text;

            base.OnNavigatedFrom(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            object text;

            if (appService.State.TryGetValue("toText", out text))
                toTextBox.Text = text as string;

            if (appService.State.TryGetValue("bodyText", out text))
                bodyTextBox.Text = text as string;

            base.OnNavigatedTo(args);
        }
    }
}
