using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace Petzold.Phone.Silverlight
{
    public class ValueToBrushConverter : IValueConverter
    {
        public double Criterion { set; get; }
        public Brush GreaterThanBrush { get; set; }
        public Brush EqualToBrush { get; set; }
        public Brush LessThanBrush { get; set; }

        public object Convert(object value, Type targetType,
                              object parameter, CultureInfo culture)
        {
            double doubleVal = (value as IConvertible).ToDouble(culture);
            return doubleVal >= Criterion ? doubleVal == Criterion ? EqualToBrush : 
                                                                     GreaterThanBrush : 
                                                                     LessThanBrush;
        }

        public object ConvertBack(object value, Type targetType,
                                 object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
