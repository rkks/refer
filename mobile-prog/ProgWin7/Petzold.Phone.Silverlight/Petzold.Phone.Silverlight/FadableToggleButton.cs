using System.Windows;
using System.Windows.Controls.Primitives;

namespace Petzold.Phone.Silverlight
{
    public class FadableToggleButton : ToggleButton
    {
        public static readonly DependencyProperty CheckedContentProperty =
            DependencyProperty.Register("CheckedContent",
            typeof(object),
            typeof(FadableToggleButton),
            new PropertyMetadata(null));

        public FadableToggleButton()
        {
            this.DefaultStyleKey = typeof(FadableToggleButton);
        }

        public object CheckedContent
        {
            set { SetValue(CheckedContentProperty, value); }
            get { return (object)GetValue(CheckedContentProperty); }
        }
    }
}
