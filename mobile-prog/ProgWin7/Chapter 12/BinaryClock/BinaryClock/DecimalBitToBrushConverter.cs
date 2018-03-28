using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace BinaryClock
{
    public class DecimalBitToBrushConverter : IValueConverter
    {
        public Brush ZeroBitBrush { set; get; }
        public Brush OneBitBrush { set; get; }

        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            int number = (int)value;
            int bit = Int32.Parse(parameter as string);
            int digit = number / PowerOfTen(bit / 4) % 10;

            return ((digit & (1 << (bit % 4))) == 0) ? ZeroBitBrush : OneBitBrush;
        }

        public object ConvertBack(object value, Type targetType,
                                  object parameter, CultureInfo culture)
        {
            return null;
        }

        int PowerOfTen(int exp)
        {
            int value = 1;

            for (int i = 0; i < exp; i++)
                value *= 10;

            return value;
        }
    }
}
