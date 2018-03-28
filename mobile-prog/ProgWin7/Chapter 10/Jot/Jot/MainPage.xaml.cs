using System;
using System.Collections.Generic;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;

namespace Jot
{
    public partial class MainPage : PhoneApplicationPage
    {
        JotAppSettings appSettings = (Application.Current as App).AppSettings;
        Dictionary<int, Stroke> activeStrokes = new Dictionary<int, Stroke>();

        public MainPage()
        {
            InitializeComponent();

            inkPresenter.Strokes = appSettings.StrokeCollections[appSettings.PageNumber];
            inkPresenter.Background = new SolidColorBrush(appSettings.Background);

            // Re-assign ApplicationBar button names
            appbarLastButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;
            appbarNextButton = this.ApplicationBar.Buttons[2] as ApplicationBarIconButton;
            appbarDeleteButton = this.ApplicationBar.Buttons[3] as ApplicationBarIconButton;

            TitleAndAppbarUpdate();

            Touch.FrameReported += OnTouchFrameReported;
        }

        void OnTouchFrameReported(object sender, TouchFrameEventArgs args)
        {
            TouchPoint primaryTouchPoint = args.GetPrimaryTouchPoint(null);

            if (primaryTouchPoint != null && primaryTouchPoint.Action == TouchAction.Down)
                args.SuspendMousePromotionUntilTouchUp();

            TouchPointCollection touchPoints = args.GetTouchPoints(inkPresenter);

            foreach (TouchPoint touchPoint in touchPoints)
            {
                Point pt = touchPoint.Position;
                int id = touchPoint.TouchDevice.Id;

                switch (touchPoint.Action)
                {
                    case TouchAction.Down:
                        Stroke stroke = new Stroke();
                        stroke.DrawingAttributes.Color = appSettings.Foreground;
                        stroke.DrawingAttributes.Height = appSettings.StrokeWidth;
                        stroke.DrawingAttributes.Width = appSettings.StrokeWidth;
                        stroke.StylusPoints.Add(new StylusPoint(pt.X, pt.Y));

                        inkPresenter.Strokes.Add(stroke);
                        activeStrokes.Add(id, stroke);
                        break;

                    case TouchAction.Move:
                        activeStrokes[id].StylusPoints.Add(new StylusPoint(pt.X, pt.Y));
                        break;

                    case TouchAction.Up:
                        activeStrokes[id].StylusPoints.Add(new StylusPoint(pt.X, pt.Y));
                        activeStrokes.Remove(id);

                        TitleAndAppbarUpdate();
                        break;
                }
            }
        }

        void OnAppbarAddClick(object sender, EventArgs args)
        {
            StrokeCollection strokes = new StrokeCollection();
            appSettings.PageNumber += 1;
            appSettings.StrokeCollections.Insert(appSettings.PageNumber, strokes);
            inkPresenter.Strokes = strokes;
            TitleAndAppbarUpdate();
        }

        void OnAppbarLastClick(object sender, EventArgs args)
        {
            appSettings.PageNumber -= 1;
            inkPresenter.Strokes = appSettings.StrokeCollections[appSettings.PageNumber];
            TitleAndAppbarUpdate();
        }

        void OnAppbarNextClick(object sender, EventArgs args)
        {
            appSettings.PageNumber += 1;
            inkPresenter.Strokes = appSettings.StrokeCollections[appSettings.PageNumber];
            TitleAndAppbarUpdate();
        }

        void OnAppbarDeleteClick(object sender, EventArgs args)
        {
            MessageBoxResult result = MessageBox.Show("Delete this page?", "Jot", 
                                                       MessageBoxButton.OKCancel);

            if (result == MessageBoxResult.OK)
            {
                if (appSettings.StrokeCollections.Count == 1)
                {
                    appSettings.StrokeCollections[0].Clear();
                }
                else
                {
                    appSettings.StrokeCollections.RemoveAt(appSettings.PageNumber);

                    if (appSettings.PageNumber == appSettings.StrokeCollections.Count)
                        appSettings.PageNumber -= 1;

                    inkPresenter.Strokes = appSettings.StrokeCollections[appSettings.PageNumber];
                }
                TitleAndAppbarUpdate();
            }
        }

        void OnAppbarSwapColorsClick(object sender, EventArgs args)
        {
            Color foreground = appSettings.Background;
            appSettings.Background = appSettings.Foreground;
            appSettings.Foreground = foreground;
            inkPresenter.Background = new SolidColorBrush(appSettings.Background);

            foreach (StrokeCollection strokeCollection in appSettings.StrokeCollections)
                foreach (Stroke stroke in strokeCollection)
                    stroke.DrawingAttributes.Color = appSettings.Foreground;
        }

        void OnAppbarSetStrokeWidthClick(object sender, EventArgs args)
        {
            ApplicationBarMenuItem item = sender as ApplicationBarMenuItem;

            if (item.Text.StartsWith("light"))
                appSettings.StrokeWidth = 1;

            else if (item.Text.StartsWith("medium"))
                appSettings.StrokeWidth = 3;

            else if (item.Text.StartsWith("heavy"))
                appSettings.StrokeWidth = 5;
        }

        void TitleAndAppbarUpdate()
        {
            pageInfoTitle.Text = String.Format(" - PAGE {0} OF {1}", 
                                               appSettings.PageNumber + 1, 
                                               appSettings.StrokeCollections.Count);

            appbarLastButton.IsEnabled = appSettings.PageNumber > 0;
            appbarNextButton.IsEnabled = 
                        appSettings.PageNumber < appSettings.StrokeCollections.Count - 1;
            appbarDeleteButton.IsEnabled = (appSettings.StrokeCollections.Count > 1) ||
                                           (appSettings.StrokeCollections[0].Count > 0);
        }
    }
}
