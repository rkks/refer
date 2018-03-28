using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Markup;

namespace Petzold.Phone.Silverlight
{
    public class XamlCruncherTextBox : TextBox
    {
        public event EventHandler<XamlCruncherEventArgs> XamlResult;

        public XamlCruncherTextBox()
        {
            this.AcceptsReturn = true;
            this.TextWrapping = TextWrapping.NoWrap;
            this.HorizontalScrollBarVisibility = ScrollBarVisibility.Auto;
            this.VerticalScrollBarVisibility = ScrollBarVisibility.Auto;

            TextChanged += OnTextBoxTextChanged;
        }

        void OnTextBoxTextChanged(object sender, TextChangedEventArgs args)
        {
            string xaml =
                "<UserControl " +
                " xmlns='http://schemas.microsoft.com/winfx/2006/xaml/presentation'\r" +
                " xmlns:phone='clr-namespace:Microsoft.Phone.Controls;" + 
                "assembly=Microsoft.Phone'\r" +
                " xmlns:shell='clr-namespace:Microsoft.Phone.Shell;" + 
                "assembly=Microsoft.Phone'\r" +
                " xmlns:system='clr-namespace:System;assembly=mscorlib'\r" +
                " xmlns:petzold='clr-namespace:Petzold.Phone.Silverlight;" + 
                "assembly=Petzold.Phone.Silverlight'>\r" +
                "    " + this.Text + "\r" +
                "</UserControl>";

            UserControl ctrl = null;

            try
            {
                ctrl = XamlReader.Load(xaml) as UserControl;
            }
            catch (Exception exc)
            {
                OnXamlResult(new XamlCruncherEventArgs(exc.Message));
                return;
            }

            if (ctrl == null)
            {
                OnXamlResult(new XamlCruncherEventArgs("null result"));
                return;
            }

            OnXamlResult(new XamlCruncherEventArgs(ctrl));
        }

        protected virtual void OnXamlResult(XamlCruncherEventArgs args)
        {
            if (XamlResult != null)
                XamlResult(this, args);
        }
    }
}
