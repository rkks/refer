using System.Windows.Input;
using Microsoft.Phone.Controls;

namespace StackPanelWithFourElements
{
    public partial class MainPage : PhoneApplicationPage
    {
        public MainPage()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            stackPanel.Orientation = 
                stackPanel.Orientation == System.Windows.Controls.Orientation.Vertical ?
                                            System.Windows.Controls.Orientation.Horizontal : 
                                            System.Windows.Controls.Orientation.Vertical;
            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }
    }
}
