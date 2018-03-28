using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;

namespace StudentCardFile
{
    public partial class StudentCard : UserControl
    {
        static List<StudentCard> studentCards = new List<StudentCard>();
        static int contactTime;
        static Point contactPoint;

        public static readonly DependencyProperty IsOpenProperty =
            DependencyProperty.Register("IsOpen",
                typeof(bool),
                typeof(StudentCard),
                new PropertyMetadata(false, OnIsOpenChanged));

        static StudentCard()
        {
            Touch.FrameReported += OnTouchFrameReported;
        }

        static void OnIsOpenChanged(DependencyObject obj, 
                                    DependencyPropertyChangedEventArgs args)
        {
            (obj as StudentCard).OnIsOpenChanged(args);
        }

        static void OnTouchFrameReported(object sender, TouchFrameEventArgs args)
        {
            TouchPoint touchPoint = args.GetPrimaryTouchPoint(null);

            if (touchPoint != null && touchPoint.Action == TouchAction.Down)
            {
                contactPoint = touchPoint.Position;
                contactTime = args.Timestamp;
            }
            else if (touchPoint != null && touchPoint.Action == TouchAction.Up)
            {
                // Check if finger is directly over StudentCard or child
                DependencyObject element = touchPoint.TouchDevice.DirectlyOver;

                while (element != null && !(element is StudentCard))
                    element = VisualTreeHelper.GetParent(element);

                if (element == null)
                    return;

                // Get lift point and calculate difference
                Point liftPoint = touchPoint.Position;
                double distance = Math.Sqrt(Math.Pow(contactPoint.X - liftPoint.X, 2) +
                                            Math.Pow(contactPoint.Y - liftPoint.Y, 2));

                // Qualify as a Tap if distance < 12 pixels within 1/4 second
                if (distance < 12 && args.Timestamp - contactTime < 250)
                {
                    // Enumerate StudentCard objects and set IsOpen property
                    foreach (StudentCard studentCard in studentCards)
                        studentCard.IsOpen = (element == studentCard && !studentCard.IsOpen);
                }
            }
        }

        public StudentCard()
        {
            InitializeComponent();
            studentCards.Add(this);
        }

        bool IsOpen
        {
            set { SetValue(IsOpenProperty, value); }
            get { return (bool)GetValue(IsOpenProperty); }
        }

        void OnIsOpenChanged(DependencyPropertyChangedEventArgs args)
        {
            VisualStateManager.GoToState(this, IsOpen ? "Open" : "Normal", false);
        }
    }
}
