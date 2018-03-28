using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.Phone.Silverlight
{
    public class OverlapPanel : Panel
    {
        Size maxChildSize = new Size();

        public static readonly DependencyProperty OrientationProperty =
            DependencyProperty.Register("Orientation",
                typeof(Orientation),
                typeof(OverlapPanel),
                new PropertyMetadata(Orientation.Horizontal, OnAffectsMeasure));

        public static readonly DependencyProperty MinimumOverlapProperty =
            DependencyProperty.Register("MinimumOverlap",
                typeof(double),
                typeof(OverlapPanel),
                new PropertyMetadata(0.0, OnAffectsMeasure));

        public Orientation Orientation
        {
            set { SetValue(OrientationProperty, value); }
            get { return (Orientation)GetValue(OrientationProperty); }
        }

        public double MinimumOverlap
        {
            set { SetValue(MinimumOverlapProperty, value); }
            get { return (double)GetValue(MinimumOverlapProperty); }
        }

        static void OnAffectsMeasure(DependencyObject obj,
                                     DependencyPropertyChangedEventArgs args)
        {
            (obj as OverlapPanel).InvalidateMeasure();
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (Children.Count == 0)
                return new Size(0, 0);

            maxChildSize = new Size();

            foreach (UIElement child in Children)
            {
                if (Orientation == Orientation.Horizontal)
                    child.Measure(new Size(Double.PositiveInfinity, availableSize.Height));
                else
                    child.Measure(new Size(availableSize.Width, Double.PositiveInfinity));

                maxChildSize.Width = Math.Max(maxChildSize.Width, 
                                              child.DesiredSize.Width);

                maxChildSize.Height = Math.Max(maxChildSize.Height, 
                                               child.DesiredSize.Height);
            }

            if (Orientation == Orientation.Horizontal)
            {
                double maxTotalWidth = maxChildSize.Width * Children.Count;
                double minTotalWidth = maxChildSize.Width +
                                            MinimumOverlap * (Children.Count - 1);

                if (Double.IsPositiveInfinity(availableSize.Width))
                    return new Size(minTotalWidth, maxChildSize.Height);

                if (maxTotalWidth < availableSize.Width)
                    return new Size(maxTotalWidth, maxChildSize.Height);

                else if (minTotalWidth < availableSize.Width)
                    return new Size(availableSize.Width, maxChildSize.Height);

                return new Size(minTotalWidth, maxChildSize.Height);
            }
            // Orientation = Vertical

            double maxTotalHeight = maxChildSize.Height * Children.Count;
            double minTotalHeight = maxChildSize.Height +
                                        MinimumOverlap * (Children.Count - 1);

            if (Double.IsPositiveInfinity(availableSize.Height))
                return new Size(maxChildSize.Width, minTotalHeight);

            if (maxTotalHeight < availableSize.Height)
                return new Size(maxChildSize.Width, maxTotalHeight);

            else if (minTotalHeight < availableSize.Height)
                return new Size(maxChildSize.Width, availableSize.Height);

            return new Size(maxChildSize.Width, minTotalHeight);
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            if (Children.Count == 0)
                return finalSize;

            double increment = 0;

            if (Orientation == Orientation.Horizontal)
                increment = Math.Max(MinimumOverlap, 
                    (finalSize.Width - maxChildSize.Width) / 
                        (Children.Count - 1));
            else
                increment = Math.Max(MinimumOverlap,
                    (finalSize.Height - maxChildSize.Height) / 
                        (Children.Count - 1));

            Point ptChild = new Point();

            foreach (UIElement child in Children)
            {
                child.Arrange(new Rect(ptChild, maxChildSize));

                if (Orientation == Orientation.Horizontal)
                    ptChild.X += increment;
                else
                    ptChild.Y += increment;
            }

            return finalSize;
        }
    }
}
