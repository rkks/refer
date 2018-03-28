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

namespace VerticalStackPanelDemo
{
    public class VerticalStackPanel : Panel
    {
        protected override Size MeasureOverride(Size availableSize)
        {
            Size compositeSize = new Size();

            foreach (UIElement child in Children)
            {
                child.Measure(new Size(availableSize.Width, Double.PositiveInfinity));
                compositeSize.Width = Math.Max(compositeSize.Width, child.DesiredSize.Width);
                compositeSize.Height += child.DesiredSize.Height;
            }
            return compositeSize;
        }

        protected override Size ArrangeOverride(Size finalSize)
        {
            double x = 0, y = 0;

            foreach (UIElement child in Children)
            {
                child.Arrange(new Rect(x, y, finalSize.Width, child.DesiredSize.Height));
                y += child.DesiredSize.Height;
            }
            return base.ArrangeOverride(finalSize);
        }
    }
}
