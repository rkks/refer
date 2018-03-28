using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace Petzold.Phone.Silverlight
{
    [TemplatePartAttribute(Name = "PlanePart", Type = typeof(Canvas))]
    [TemplatePartAttribute(Name = "ThumbPart", Type = typeof(Thumb))]
    public class XYSlider : Control
    {
        Canvas planePart;
        Thumb thumbPart;
        Point absoluteThumbPoint;

        public event RoutedPropertyChangedEventHandler<Point> ValueChanged;

        public static readonly DependencyProperty PlaneBackgroundProperty =
            DependencyProperty.Register("PlaneBackground",
                typeof(Brush),
                typeof(XYSlider),
                new PropertyMetadata(new SolidColorBrush(Colors.Gray)));

        public static readonly DependencyProperty ValueProperty =
            DependencyProperty.Register("Value",
                typeof(Point),
                typeof(XYSlider),
                new PropertyMetadata(new Point(0.5, 0.5), OnValueChanged));

        public XYSlider()
        {
            this.DefaultStyleKey = typeof(XYSlider);
        }

        public Brush PlaneBackground
        {
            set { SetValue(PlaneBackgroundProperty, value); }
            get { return (Brush)GetValue(PlaneBackgroundProperty); }
        }

        public Point Value
        {
            set { SetValue(ValueProperty, value); }
            get { return (Point)GetValue(ValueProperty); }
        }

        static void OnValueChanged(DependencyObject obj, 
                                   DependencyPropertyChangedEventArgs args)
        {
            (obj as XYSlider).OnValueChanged((Point)args.OldValue, (Point)args.NewValue);
        }

        protected virtual void OnValueChanged(Point oldValue, Point newValue)
        {
            if (newValue.X < 0 || newValue.X > 1 || newValue.Y < 0 || newValue.Y > 1)
                throw new ArgumentOutOfRangeException("Value", 
                    "Value property must be Point with coordinates between 0 and 1");

            ScaleValueToPlane(newValue);

            if (ValueChanged != null)
                ValueChanged(this, 
                    new RoutedPropertyChangedEventArgs<Point>(oldValue, newValue));
        }

        public override void OnApplyTemplate()
        {
            if (planePart != null)
            {
                planePart.SizeChanged -= OnPlaneSizeChanged;
            }

            if (thumbPart != null)
            {
                thumbPart.DragDelta -= OnThumbDragDelta;
            }

            planePart = GetTemplateChild("PlanePart") as Canvas;
            thumbPart = GetTemplateChild("ThumbPart") as Thumb;

            if (planePart != null && thumbPart != null)
            {
                planePart.SizeChanged += OnPlaneSizeChanged;
                thumbPart.DragStarted += OnThumbDragStarted;
                thumbPart.DragDelta += OnThumbDragDelta;
                ScaleValueToPlane(this.Value);
            }

            base.OnApplyTemplate();
        }

        void OnPlaneSizeChanged(object sender, SizeChangedEventArgs args)
        {
            ScaleValueToPlane(this.Value);
        }

        void OnThumbDragStarted(object sender, DragStartedEventArgs args)
        {
            absoluteThumbPoint = new Point(Canvas.GetLeft(thumbPart), Canvas.GetTop(thumbPart));
        }

        void OnThumbDragDelta(object sender, DragDeltaEventArgs args)
        {
            absoluteThumbPoint.X += args.HorizontalChange;
            absoluteThumbPoint.Y += args.VerticalChange;

            Value = new Point(Math.Max(0, Math.Min(1, absoluteThumbPoint.X / planePart.ActualWidth)),
                              Math.Max(0, Math.Min(1, absoluteThumbPoint.Y / planePart.ActualHeight)));
        }

        void ScaleValueToPlane(Point point)
        {
            if (planePart != null && thumbPart != null)
            {
                Canvas.SetLeft(thumbPart, planePart.ActualWidth * point.X);
                Canvas.SetTop(thumbPart, planePart.ActualHeight * point.Y);
            }
        }
    }
}
