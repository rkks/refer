using System;
using System.Windows;
using System.Windows.Controls;

namespace SingleCellGridDemo
{
    public class SingleCellGrid : Panel
    {
        protected override Size MeasureOverride(Size availableSize)
        {
            Size compositeSize = new Size();

            foreach (UIElement child in Children)
            {
                child.Measure(availableSize);

                compositeSize.Width = Math.Max(compositeSize.Width, child.DesiredSize.Width);
                compositeSize.Height = Math.Max(compositeSize.Height, child.DesiredSize.Height);
            }

            return compositeSize;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            foreach (UIElement child in Children)
            {
                child.Arrange(new Rect(new Point(), finalSize));
            }

            return base.ArrangeOverride(finalSize);
        }
    }
}
