using System.Windows.Input;
using System.Windows.Media.Imaging;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Tasks;

namespace SilverlightTapToShoot
{
    public partial class MainPage : PhoneApplicationPage
    {
        CameraCaptureTask camera = new CameraCaptureTask();

        public MainPage()
        {
            InitializeComponent();

            camera.Completed += OnCameraCaptureTaskCompleted;
        }

        protected override void OnManipulationStarted(ManipulationStartedEventArgs args)
        {
            camera.Show();

            args.Complete();
            args.Handled = true;
            base.OnManipulationStarted(args);
        }

        void OnCameraCaptureTaskCompleted(object sender, PhotoResult args)
        {
            if (args.TaskResult == TaskResult.OK)
            {
                BitmapImage bmp = new BitmapImage();
                bmp.SetSource(args.ChosenPhoto);
                img.Source = bmp;
            }
        }
    }
}
