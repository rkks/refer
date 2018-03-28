using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Media;
using Microsoft.Phone.Controls;

namespace QuadraticEquations2
{
    public partial class MainPage : PhoneApplicationPage
    {
        readonly Brush okBrush;
        static readonly Brush errorBrush = new SolidColorBrush(Colors.Red);

        public MainPage()
        {
            InitializeComponent();
            okBrush = new TextBox().Foreground;
            BindingValidationError += OnBindingValidationError;
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            TextBox txtbox = sender as TextBox;
            txtbox.Foreground = okBrush;

            BindingExpression bindingExpression = 
                txtbox.GetBindingExpression(TextBox.TextProperty);
            bindingExpression.UpdateSource();
        }

        void OnBindingValidationError(object sender, ValidationErrorEventArgs args)
        {
            TextBox txtbox = args.OriginalSource as TextBox;
            txtbox.Foreground = errorBrush;
        }
    }
}
