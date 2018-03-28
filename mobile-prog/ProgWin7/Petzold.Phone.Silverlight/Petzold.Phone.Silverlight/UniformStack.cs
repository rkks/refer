using System;
using System.Windows;
using System.Windows.Controls;

namespace Petzold.Phone.Silverlight
{
    public class UniformStack : Panel
    {
        public static readonly DependencyProperty OrientationProperty =
            DependencyProperty.Register("Orientation",
                typeof(Orientation),
                typeof(UniformStack),
                new PropertyMetadata(Orientation.Vertical, OnOrientationChanged));

        public Orientation Orientation
        {
            set { SetValue(OrientationProperty, value); }
            get { return (Orientation)GetValue(OrientationProperty); }
        }

        static void OnOrientationChanged(DependencyObject obj,
                                         DependencyPropertyChangedEventArgs args)
        {
            (obj as UniformStack).InvalidateMeasure();
        }

        protected override Size MeasureOverride(Size availableSize)
        {
            if (Children.Count == 0)
                return new Size();

            Size availableChildSize = new Size();
            Size maxChildSize = new Size();
            Size compositeSize = new Size();

            // Calculate an available size for each child                
            if (Orientation == Orientation.Horizontal)
                availableChildSize = new Size(availableSize.Width / Children.Count, 
                                              availableSize.Height);
            else
                availableChildSize = new Size(availableSize.Width, 
                                              availableSize.Height / Children.Count);

            // Enumerate the children, and find the widest width and the highest height
            foreach (UIElement child in Children)
            {
                child.Measure(availableChildSize);
                maxChildSize.Width = Math.Max(maxChildSize.Width, child.DesiredSize.Width);
                maxChildSize.Height = Math.Max(maxChildSize.Height, child.DesiredSize.Height);
            }

            // Now determine a composite size that depends on infinite available width or height
            if (Orientation == Orientation.Horizontal)
            {
                if (Double.IsPositiveInfinity(availableSize.Width))
                    compositeSize = new Size(maxChildSize.Width * Children.Count, 
                                             maxChildSize.Height);
                else
                    compositeSize = new Size(availableSize.Width, maxChildSize.Height);
            }
            else
            {
                if (Double.IsPositiveInfinity(availableSize.Height))
                    compositeSize = new Size(maxChildSize.Width, 
                                             maxChildSize.Height * Children.Count);
                else
                    compositeSize = new Size(maxChildSize.Width, availableSize.Height);
            }

            return compositeSize;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            if (Children.Count > 0)
            {
                Size finalChildSize = new Size();
                double x = 0;
                double y = 0;
                
                if (Orientation == Orientation.Horizontal)
                    finalChildSize = new Size(finalSize.Width / Children.Count, 
                                              finalSize.Height);
                else
                    finalChildSize = new Size(finalSize.Width, 
                                              finalSize.Height / Children.Count);

                foreach (UIElement child in Children)
                {
                    child.Arrange(new Rect(new Point(x, y), finalChildSize));

                    if (Orientation == Orientation.Horizontal)
                        x += finalChildSize.Width;
                    else
                        y += finalChildSize.Height;
                }
            }

            return base.ArrangeOverride(finalSize);
        }
    }
}
