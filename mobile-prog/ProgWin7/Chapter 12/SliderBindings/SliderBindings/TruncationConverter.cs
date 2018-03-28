using System;
using System.Globalization;
using System.Windows.Data;

namespace SliderBindings
{
    public class TruncationConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, 
                              object parameter, CultureInfo culture)
        {
            if (value is double)
                return Math.Round((double)value);

            return value;
        }

        public object ConvertBack(object value, Type targetType, 
                                  object parameter, CultureInfo culture)
        {
            return value;
        }
    }
}
