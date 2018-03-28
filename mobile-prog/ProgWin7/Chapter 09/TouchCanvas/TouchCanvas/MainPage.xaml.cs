using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using Microsoft.Phone.Controls;

namespace TouchCanvas
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            args.ManipulationContainer = canvas;
            base.OnManipulationStarted(args);
        }

        protected override void OnManipulationDelta(ManipulationDeltaEventArgs args)
        {
            UIElement element = args.OriginalSource as UIElement;
            Point translation = args.DeltaManipulation.Translation;
            Canvas.SetLeft(element, Canvas.GetLeft(element) + translation.X);
            Canvas.SetTop(element, Canvas.GetTop(element) + translation.Y);

            args.Handled = true;
            base.OnManipulationDelta(args);
        }
    }
}
