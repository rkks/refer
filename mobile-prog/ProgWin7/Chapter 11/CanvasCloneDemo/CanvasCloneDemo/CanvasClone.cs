using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace CanvasCloneDemo
{
    public class CanvasClone : Panel
    {
        public static readonly DependencyProperty LeftProperty =
            DependencyProperty.RegisterAttached("Left",
                typeof(double),
                typeof(CanvasClone),
                new PropertyMetadata(0.0, OnLeftOrTopPropertyChanged));

        public static readonly DependencyProperty TopProperty =
            DependencyProperty.RegisterAttached("Top",
                typeof(double),
                typeof(CanvasClone),
                new PropertyMetadata(0.0, OnLeftOrTopPropertyChanged));

        public static void SetLeft(DependencyObject obj, double value)
        {
            obj.SetValue(LeftProperty, value);
        }

        public static double GetLeft(DependencyObject obj)
        {
            return (double)obj.GetValue(LeftProperty);
        }

        public static void SetTop(DependencyObject obj, double value)
        {
            obj.SetValue(TopProperty, value);
        }

        public static double GetTop(DependencyObject obj)
        {
            return (double)obj.GetValue(TopProperty);
        }

        static void OnLeftOrTopPropertyChanged(DependencyObject obj,
                                               DependencyPropertyChangedEventArgs args)
        {
            CanvasClone parent = VisualTreeHelper.GetParent(obj) as CanvasClone;

            if (parent != null)
                parent.InvalidateArrange();
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            foreach (UIElement child in Children)
                child.Measure(new Size(Double.PositiveInfinity,
                                       Double.PositiveInfinity));
            return Size.Empty;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            foreach (UIElement child in Children)
                child.Arrange(new Rect(
                    new Point(GetLeft(child), GetTop(child)), child.DesiredSize));

            return base.ArrangeOverride(finalSize);
        }
    }
}
