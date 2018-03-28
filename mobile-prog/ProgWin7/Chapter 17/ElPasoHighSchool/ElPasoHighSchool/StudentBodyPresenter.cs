using System;
using System.ComponentModel;
using System.IO;
using System.Net;
using System.Windows.Threading;
using System.Xml.Serialization;

namespace ElPasoHighSchool
{
    public class StudentBodyPresenter : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        StudentBody studentBody;
        Random rand = new Random();

        public StudentBodyPresenter()
        {
            Uri uri = new Uri("http://www.charlespetzold.com/Students/students.xml"); // , UriKind.Relative);

            WebClient webClient = new WebClient();
            webClient.DownloadStringCompleted += OnDownloadStringCompleted;
            webClient.DownloadStringAsync(uri);
        }

        void OnDownloadStringCompleted(object sender, DownloadStringCompletedEventArgs args)
        {
            StringReader reader = new StringReader(args.Result);
            XmlSerializer xml = new XmlSerializer(typeof(StudentBody));
            StudentBody = xml.Deserialize(reader) as StudentBody;

            DispatcherTimer tmr = new DispatcherTimer();
            tmr.Tick += TimerOnTick;
            tmr.Interval = TimeSpan.FromMilliseconds(100);
            tmr.Start();
        }

        public StudentBody StudentBody
        {
            protected set
            {
                if (studentBody != value)
                {
                    studentBody = value;
                    OnPropertyChanged("StudentBody");
                }
            }
            get
            {
                return studentBody;
            }
        }

        protected virtual void OnPropertyChanged(string propChanged)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propChanged));
        }

        void TimerOnTick(object sender, EventArgs args)
        {
            int index = rand.Next(studentBody.Students.Count);
            Student student = studentBody.Students[index];

            double factor = 1 + (rand.NextDouble() - 0.5) / 5;

            student.GradePointAverage = 
                    Math.Max(0, Math.Min(5, Decimal.Round((decimal)factor * 
                                                student.GradePointAverage, 2)));
        }
    }
}
