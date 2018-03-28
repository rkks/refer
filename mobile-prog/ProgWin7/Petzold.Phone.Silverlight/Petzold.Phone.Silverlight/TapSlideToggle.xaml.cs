using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace Petzold.Phone.Silverlight
{
    public partial class TapSlideToggle : UserControl
    {
        public static readonly DependencyProperty IsCheckedProperty =
            DependencyProperty.Register("IsChecked",
                typeof(bool),
                typeof(TapSlideToggle),
                new PropertyMetadata(false, OnIsCheckedChanged));

        public event RoutedEventHandler Checked;
        public event RoutedEventHandler Unchecked;

        public TapSlideToggle()
        {
            InitializeComponent();
        }

        public bool IsChecked
        {
            set { SetValue(IsCheckedProperty, value); }
            get { return (bool)GetValue(IsCheckedProperty); }
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

        protected override void OnManipulationCompleted(ManipulationCompletedEventArgs args)
        {
            Point pt = args.ManipulationOrigin;

            if (pt.X > 0 && pt.X < this.ActualWidth && 
                pt.Y > 0 && pt.Y < this.ActualHeight)
                IsChecked ^= true;

            args.Handled = true;
            base.OnManipulationCompleted(args);
        }

        static void OnIsCheckedChanged(DependencyObject obj, 
                                       DependencyPropertyChangedEventArgs args)
        {
            (obj as TapSlideToggle).OnIsCheckedChanged(args);            
        }

        void OnIsCheckedChanged(DependencyPropertyChangedEventArgs args)
        {
            fillRectangle.Visibility = IsChecked ? Visibility.Visible : 
                                                   Visibility.Collapsed;

            slideBorder.HorizontalAlignment = IsChecked ? HorizontalAlignment.Right : 
                                                          HorizontalAlignment.Left;

            if (IsChecked && Checked != null)
                Checked(this, new RoutedEventArgs());

            if (!IsChecked && Unchecked != null)
                Unchecked(this, new RoutedEventArgs());
        }
    }
}
