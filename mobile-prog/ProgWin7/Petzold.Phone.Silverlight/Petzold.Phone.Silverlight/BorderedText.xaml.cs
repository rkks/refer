using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.Phone.Silverlight
{
    public partial class BorderedText : UserControl
    {
        public static readonly DependencyProperty TextProperty =
            DependencyProperty.Register("Text",
                typeof(string),
                typeof(BorderedText),
                new PropertyMetadata(null));

        public static readonly DependencyProperty TextAlignmentProperty =
            DependencyProperty.Register("TextAlignment",
                typeof(TextAlignment),
                typeof(BorderedText),
                new PropertyMetadata(TextAlignment.Left));

        public static readonly DependencyProperty TextDecorationsProperty =
            DependencyProperty.Register("TextDecorations",
                typeof(TextDecorationCollection),
                typeof(BorderedText),
                new PropertyMetadata(null));

        public static readonly DependencyProperty TextWrappingProperty =
            DependencyProperty.Register("TextWrapping",
                typeof(TextWrapping),
                typeof(BorderedText),
                new PropertyMetadata(TextWrapping.NoWrap));

        public static readonly DependencyProperty CornerRadiusProperty =
            DependencyProperty.Register("CornerRadius",
                typeof(CornerRadius),
                typeof(BorderedText),
                new PropertyMetadata(new CornerRadius()));

        public BorderedText()
        {
            InitializeComponent();
        }

        public string Text
        {
            set { SetValue(TextProperty, value); }
            get { return (string)GetValue(TextProperty); }
        }

        public TextAlignment TextAlignment
        {
            set { SetValue(TextAlignmentProperty, value); }
            get { return (TextAlignment)GetValue(TextAlignmentProperty); }
        }

        public TextDecorationCollection TextDecorations
        {
            set { SetValue(TextDecorationsProperty, value); }
            get { return (TextDecorationCollection)GetValue(TextDecorationsProperty); }
        }

        public TextWrapping TextWrapping
        {
            set { SetValue(TextWrappingProperty, value); }
            get { return (TextWrapping)GetValue(TextWrappingProperty); }
        }

        public CornerRadius CornerRadius
        {
            set { SetValue(CornerRadiusProperty, value); }
            get { return (CornerRadius)GetValue(CornerRadiusProperty); }
        }
    }
}
