using System;
using System.Globalization;
using System.Windows.Data;

namespace Petzold.Phone.Silverlight
{
    public class MultiplyConverter : IValueConverter
    {
        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            double multiplier;

            if (value is IConvertible &&
                    parameter is string &&
                        Double.TryParse(parameter as string, out multiplier))
            {
                return (value as IConvertible).ToDouble(culture) * multiplier;
            }
            return value;
        }

        public object ConvertBack(object value, Type targetType,
                                  object parameter, CultureInfo culture)
        {
            double divider;

            if (value is IConvertible &&
                    parameter is string &&
                        Double.TryParse(parameter as string, out divider))
            {
                return (value as IConvertible).ToDouble(culture) / divider;
            }
            return value;
        }
    }
}
