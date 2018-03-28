using System.ComponentModel;

namespace Petzold.Phone.Silverlight
{
    public class Adder : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;

        double augend = 0;
        double addend = 0;
        double sum = 0;

        public double Augend
        {
            set
            {
                if (augend != value)
                {
                    augend = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Augend"));
                    CalculateNewSum();
                }
            }
            get
            {
                return augend;
            }
        }

        public double Addend
        {
            set
            {
                if (addend != value)
                {
                    addend = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Addend"));
                    CalculateNewSum();
                }
            }
            get
            {
                return addend;
            }
        }

        public double Sum
        {
            protected set
            {
                if (sum != value)
                {
                    sum = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Sum"));
                }
            }

            get
            {
                return sum;
            }
        }

        void CalculateNewSum()
        {
            Sum = Augend + Addend;
        }

        protected virtual void OnPropertyChanged(PropertyChangedEventArgs args)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, args);
        }
    }
}
