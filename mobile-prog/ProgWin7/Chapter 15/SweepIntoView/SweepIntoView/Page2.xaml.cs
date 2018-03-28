using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using Microsoft.Phone.Controls;

namespace SweepIntoView
{
    public partial class Page2 : PhoneApplicationPage
    {
        public Page2()
        {
            InitializeComponent();
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            this.NavigationService.GoBack();

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }
    }
}