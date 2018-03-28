using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace BetterGradientButtonDemo
{
    public class BetterGradientButton : Button
    {
        GradientStop gradientStop1, gradientStop2;

        public static readonly DependencyProperty Color1Property =
            DependencyProperty.Register("Color1",
                typeof(Color),
                typeof(BetterGradientButton),
                new PropertyMetadata(Colors.Black, OnColorChanged));

        public static readonly DependencyProperty Color2Property =
            DependencyProperty.Register("Color2",
                typeof(Color),
                typeof(BetterGradientButton),
                new PropertyMetadata(Colors.White, OnColorChanged));

        public BetterGradientButton()
        {
            LinearGradientBrush brush = new LinearGradientBrush();
            brush.StartPoint = new Point(0, 0);
            brush.EndPoint = new Point(1, 0);

            gradientStop1 = new GradientStop();
            gradientStop1.Offset = 0;
            gradientStop1.Color = Color1;
            brush.GradientStops.Add(gradientStop1);

            gradientStop2 = new GradientStop();
            gradientStop2.Offset = 1;
            gradientStop2.Color = Color2;
            brush.GradientStops.Add(gradientStop2);

            Foreground = brush;
        }

        public Color Color1
        {
            set { SetValue(Color1Property, value); }
            get { return (Color)GetValue(Color1Property); }
        }

        public Color Color2
        {
            set { SetValue(Color2Property, value); }
            get { return (Color)GetValue(Color2Property); }
        }

        static void OnColorChanged(DependencyObject obj,
                                   DependencyPropertyChangedEventArgs args)
        {
            BetterGradientButton btn = obj as BetterGradientButton;

            if (args.Property == Color1Property)
                btn.gradientStop1.Color = (Color)args.NewValue;

            if (args.Property == Color2Property)
                btn.gradientStop2.Color = (Color)args.NewValue;
        }
    }
}
