using System;
using System.Windows;

namespace Petzold.Phone.Silverlight
{
    public class XamlCruncherEventArgs : EventArgs
    {
        public XamlCruncherEventArgs(UIElement element)
        {
            Element = element;
            Error = null;
        }

        public XamlCruncherEventArgs(string error)
        {
            Error = error;
            Element = null;
        }

        public UIElement Element { set; get; }

        public string Error { set; get; }
    }
}
