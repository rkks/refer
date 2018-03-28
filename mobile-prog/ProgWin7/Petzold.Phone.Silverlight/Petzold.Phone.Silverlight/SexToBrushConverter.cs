using System;
using System.Globalization;
using System.Windows.Data;
using System.Windows.Media;

namespace Petzold.Phone.Silverlight
{
    public class SexToBrushConverter : IValueConverter
    {
        public Brush MaleBrush { get; set; }
        public Brush FemaleBrush { get; set; }

        public object Convert(object value, Type targetType, 
                              object parameter, CultureInfo culture)
        {
            string sex = value as string;

            switch (sex)
            {
                case "Male": return MaleBrush;
                case "Female": return FemaleBrush;
            }

            return null;
        }

        public object ConvertBack(object value, Type targetType,
                                 object parameter, CultureInfo culture)
        {
            return null;
        }
    }
}
