using System;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace DragAndScale
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        void OnEllipseManipulationDelta(object sender, ManipulationDeltaEventArgs args)
        {
            Ellipse ellipse = sender as Ellipse;
            CompositeTransform xform = ellipse.RenderTransform as CompositeTransform;

            if (args.DeltaManipulation.Scale.X > 0 || args.DeltaManipulation.Scale.Y > 0)
            {
                double maxScale = Math.Max(args.DeltaManipulation.Scale.X,
                                           args.DeltaManipulation.Scale.Y);
                xform.ScaleX *= maxScale;
                xform.ScaleY *= maxScale;
            }

            xform.TranslateX += args.DeltaManipulation.Translation.X;
            xform.TranslateY += args.DeltaManipulation.Translation.Y;

            args.Handled = true;
        }
    }
}
