using System;
using System.ComponentModel;

namespace ElPasoHighSchool
{
    public class Student : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        string fullName;
        string firstName;
        string middleName;
        string lastName;
        string sex;
        string photoFilename;
        decimal gradePointAverage;

        public string FullName
        {
            set
            {
                if (fullName != value)
                {
                    fullName = value;
                    OnPropertyChanged("FullName");
                }
            }
            get
            {
                return fullName;
            }
        }

        public string FirstName
        {
            set
            {
                if (firstName != value)
                {
                    firstName = value;
                    OnPropertyChanged("FirstName");
                }
            }
            get
            {
                return firstName;
            }
        }

        public string MiddleName
        {
            set
            {
                if (middleName != value)
                {
                    middleName = value;
                    OnPropertyChanged("MiddleName");
                }
            }
            get
            {
                return middleName;
            }
        }

        public string LastName
        {
            set
            {
                if (lastName != value)
                {
                    lastName = value;
                    OnPropertyChanged("LastName");
                }
            }
            get
            {
                return lastName;
            }
        }

        public string Sex
        {
            set
            {
                if (sex != value)
                {
                    sex = value;
                    OnPropertyChanged("Sex");
                }
            }
            get
            {
                return sex;
            }
        }

        public string PhotoFilename
        {
            set
            {
                if (photoFilename != value)
                {
                    photoFilename = value;
                    OnPropertyChanged("PhotoFilename");
                }
            }
            get
            {
                return photoFilename;
            }
        }

        public decimal GradePointAverage
        {
            set
            {
                if (gradePointAverage != value)
                {
                    gradePointAverage = value;
                    OnPropertyChanged("GradePointAverage");
                }
            }
            get
            {
                return gradePointAverage;
            }
        }

        protected virtual void OnPropertyChanged(string propChanged)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propChanged));
        }
    }
}
