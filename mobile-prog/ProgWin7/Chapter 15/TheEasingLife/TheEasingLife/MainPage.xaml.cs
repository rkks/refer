using System;
using System.Windows;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Navigation;
using Microsoft.Phone.Controls;

namespace TheEasingLife
{
    public partial class MainPage : PhoneApplicationPage
    {
        PointCollection ballPoints = new PointCollection();
        bool isForward = true;

        public MainPage()
        {
            InitializeComponent(); 
            Loaded += OnMainPageLoaded;
        }

        public EasingFunctionBase EasingFunction { get; set; }

        void OnMainPageLoaded(object sender, RoutedEventArgs args)
        {
            double left = 100;
            double right = ContentPanel.ActualWidth - 100;
            double center = ContentPanel.ActualWidth / 2;
            double top = 100;
            double bottom = ContentPanel.ActualHeight - 100;

            polyline1.Points.Add(new Point(left, top));
            polyline1.Points.Add(new Point(right, top));

            polyline2.Points.Add(new Point(left, bottom));
            polyline2.Points.Add(new Point(right, bottom));

            ballPoints.Add(new Point(center, top));
            ballPoints.Add(new Point(center, bottom));

            ballGeometry.Center = ballPoints[1 - Convert.ToInt32(isForward)];
        }

        void OnAppbarPlayButtonClick(object sender, EventArgs args)
        {
            pointAnimation.From = ballPoints[1 - Convert.ToInt32(isForward)];
            pointAnimation.To = ballPoints[Convert.ToInt32(isForward)];
            pointAnimation.EasingFunction = EasingFunction;

            storyboard.Begin();
        }

        void OnStoryboardCompleted(object sender, EventArgs args)
        {
            isForward ^= true;
        }

        void OnAppbarSettingsButtonClick(object sender, EventArgs args)
        {
            NavigationService.Navigate(new Uri("/EasingFunctionDialog.xaml", UriKind.Relative));
        }

        protected override void OnNavigatedFrom(NavigationEventArgs args)
        {
            if (args.Content is EasingFunctionDialog)
            {
                (args.Content as EasingFunctionDialog).EasingFunction = EasingFunction;
            }
            base.OnNavigatedTo(args);
        }

        protected override void OnNavigatedTo(NavigationEventArgs args)
        {
            ApplicationTitle.Text = "THE EASING LIFE - " +
                (EasingFunction != null ? EasingFunction.GetType().Name : "none");

            base.OnNavigatedTo(args);
        }
    }
}