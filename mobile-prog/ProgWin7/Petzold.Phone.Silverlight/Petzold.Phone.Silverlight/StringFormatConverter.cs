using System;
using System.Globalization;
using System.Windows.Data;

namespace Petzold.Phone.Silverlight
{
    public class StringFormatConverter : IValueConverter
    {
        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            if (targetType == typeof(string) && parameter is string)
                return String.Format(culture, parameter as string, value);

            return value;
        }

        public object ConvertBack(object value, Type targetType,
                                  object parameter, CultureInfo culture)
        {
            return value;
        }
    }
}
