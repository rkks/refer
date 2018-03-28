using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Xml.Serialization;

namespace ElPasoHighSchool
{
    public class StudentBody : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        string school;
        ObservableCollection<Student> students = new ObservableCollection<Student>();

        public string School
        {
            set
            {
                if (school != value)
                {
                    school = value;
                    OnPropertyChanged("School");
                }
            }
            get
            {
                return school;
            }
        }

        public ObservableCollection<Student> Students
        {
            set
            {
                if (students != value)
                {
                    students = value;
                    OnPropertyChanged("Students");

                }
            }
            get
            {
                return students;
            }
        }

        protected virtual void OnPropertyChanged(string propChanged)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propChanged));
        }
    }
}