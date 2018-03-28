using System;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace CubicBezier
{
    public partial class PointDragger : UserControl
    {
        public static readonly DependencyProperty PointProperty =
            DependencyProperty.Register("Point",
                typeof(Point),
                typeof(PointDragger),
                new PropertyMetadata(OnPointChanged));

        public event RoutedPropertyChangedEventHandler<Point> PointChanged;

        public PointDragger()
        {
            InitializeComponent();
        }

        public Point Point
        {
            set { SetValue(PointProperty, value); }
            get { return (Point)GetValue(PointProperty); }
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            args.ManipulationContainer = VisualTreeHelper.GetParent(this) as UIElement;
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

        protected override void OnManipulationDelta(ManipulationDeltaEventArgs args)
        {
            Point translate = args.DeltaManipulation.Translation;
            this.Point = new Point(this.Point.X + translate.X, this.Point.Y + translate.Y);
            args.Handled = true;
            base.OnManipulationDelta(args);
        }

        static void OnPointChanged(DependencyObject obj,
                                   DependencyPropertyChangedEventArgs args)
        {
            (obj as PointDragger).OnPointChanged((Point)args.OldValue, 
                                                 (Point)args.NewValue);
        }

        protected virtual void OnPointChanged(Point oldValue, Point newValue)
        {
            ellipseGeometry.Center = newValue;

            if (PointChanged != null)
                PointChanged(this, 
                    new RoutedPropertyChangedEventArgs<Point>(oldValue, newValue));
        }
    }
}
