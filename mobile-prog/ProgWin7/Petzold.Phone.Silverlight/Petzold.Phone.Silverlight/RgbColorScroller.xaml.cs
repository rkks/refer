using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace Petzold.Phone.Silverlight
{
    public partial class RgbColorScroller : UserControl
    {
        public static readonly DependencyProperty ColorProperty =
            DependencyProperty.Register("Color",
                typeof(Color),
                typeof(RgbColorScroller),
                new PropertyMetadata(Colors.Gray, OnColorChanged));

        public event RoutedPropertyChangedEventHandler<Color> ColorChanged;

        public RgbColorScroller()
        {
            InitializeComponent();
        }

        public Color Color
        {
            set { SetValue(ColorProperty, value); }
            get { return (Color)GetValue(ColorProperty); }
        }

        void OnColorColumnValueChanged(object sender, 
                        RoutedPropertyChangedEventArgs<byte> args)
        {
            Color = Color.FromArgb(255, redColumn.Value, 
                                        greenColumn.Value, 
                                        blueColumn.Value);
        }

        static void OnColorChanged(DependencyObject obj, 
                                   DependencyPropertyChangedEventArgs args)
        {
            (obj as RgbColorScroller).OnColorChanged((Color)args.OldValue, 
                                                     (Color)args.NewValue);
        }

        protected virtual void OnColorChanged(Color oldValue, Color newValue)
        {
            redColumn.Value = newValue.R;
            greenColumn.Value = newValue.G;
            blueColumn.Value = newValue.B;

            if (ColorChanged != null)
                ColorChanged(this, 
                    new RoutedPropertyChangedEventArgs<Color>(oldValue, newValue));
        }
    }
}
