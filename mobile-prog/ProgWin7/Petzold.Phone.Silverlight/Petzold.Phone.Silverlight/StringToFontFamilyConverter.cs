using System;
using System.Globalization;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;

namespace Petzold.Phone.Silverlight
{
    public class StringToFontFamilyConverter : IValueConverter
    {
        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            if (value != null && value is string)
                return new FontFamily(value as string);

            return new FontFamily("Portable User Interface");
        }

        public object ConvertBack(object value, Type targetType,
                                  object parameter, CultureInfo culture)
        {
            if (value != null && value is FontFamily)
                return (value as FontFamily).ToString();

            return null;
        }
    }
}
