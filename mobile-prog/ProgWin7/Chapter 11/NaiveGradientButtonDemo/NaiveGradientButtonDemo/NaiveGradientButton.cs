using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;

namespace NaiveGradientButtonDemo
{
    public class NaiveGradientButton : Button
    {
        GradientStop gradientStop1, gradientStop2;

        public NaiveGradientButton()
        {
            LinearGradientBrush brush = new LinearGradientBrush();
            brush.StartPoint = new Point(0, 0);
            brush.EndPoint = new Point(1, 0);

            gradientStop1 = new GradientStop();
            gradientStop1.Offset = 0;
            brush.GradientStops.Add(gradientStop1);

            gradientStop2 = new GradientStop();
            gradientStop2.Offset = 1;
            brush.GradientStops.Add(gradientStop2);

            Foreground = brush;
        }

        public Color Color1
        {
            set { gradientStop1.Color = value; }
            get { return (Color)gradientStop1.Color; }
        }

        public Color Color2
        {
            set { gradientStop2.Color = value; }
            get { return (Color)gradientStop2.Color; }
        }
    }
}
