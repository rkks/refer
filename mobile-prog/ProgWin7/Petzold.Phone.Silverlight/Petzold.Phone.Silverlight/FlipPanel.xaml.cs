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

namespace Petzold.Phone.Silverlight
{
    public partial class FlipPanel : UserControl
    {
        public static readonly DependencyProperty Child1Property =
            DependencyProperty.Register("Child1",
                typeof(UIElement),
                typeof(FlipPanel),
                new PropertyMetadata(null, OnChild1Changed));

        public static readonly DependencyProperty Child2Property =
            DependencyProperty.Register("Child2",
                typeof(UIElement),
                typeof(FlipPanel),
                new PropertyMetadata(null, OnChild2Changed));

        public FlipPanel()
        {
            InitializeComponent();
        }

        public UIElement Child1
        {
            set { SetValue(Child1Property, value); }
            get { return (UIElement)GetValue(Child1Property); }
        }

        public UIElement Child2
        {
            set { SetValue(Child2Property, value); }
            get { return (UIElement)GetValue(Child2Property); }
        }

        public void Flip()
        {
            flipStoryboard.Begin();
        }

        public void FlipBack()
        {
            flipBackStoryboard.Begin();
        }

        static void OnChild1Changed(DependencyObject obj,
                                    DependencyPropertyChangedEventArgs args)
        {
            (obj as FlipPanel).child1Container.Content = args.NewValue;
        }

        static void OnChild2Changed(DependencyObject obj,
                                    DependencyPropertyChangedEventArgs args)
        {
            (obj as FlipPanel).child2Container.Content = args.NewValue;
        }
    }
}
