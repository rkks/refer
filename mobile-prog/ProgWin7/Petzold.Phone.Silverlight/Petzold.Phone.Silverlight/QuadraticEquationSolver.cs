using System;
using System.ComponentModel;

namespace Petzold.Phone.Silverlight
{
    public class QuadraticEquationSolver : INotifyPropertyChanged
    {
        Complex solution1;
        Complex solution2;
        bool hasTwoSolutions;
        double a, b, c;

        public event PropertyChangedEventHandler PropertyChanged;

        public double A
        {
            set
            {
                if (a != value)
                {
                    a = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("A"));
                    CalculateNewSolutions();
                }
            }
            get
            {
                return a;
            }
        }

        public double B
        {
            set
            {
                if (b != value)
                {
                    b = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("B"));
                    CalculateNewSolutions();
                }
            }
            get
            {
                return b;
            }
        }

        public double C
        {
            set
            {
                if (c != value)
                {
                    c = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("C"));
                    CalculateNewSolutions();
                }
            }
            get
            {
                return c;
            }
        }

        public Complex Solution1
        {
            protected set
            {
                if (!solution1.Equals(value))
                {
                    solution1 = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Solution1"));
                }
            }

            get
            {
                return solution1;
            }
        }

        public Complex Solution2
        {
            protected set
            {
                if (!solution2.Equals(value))
                {
                    solution2 = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Solution2"));
                }
            }

            get
            {
                return solution2;
            }
        }

        public bool HasTwoSolutions
        {
            protected set
            {
                if (hasTwoSolutions != value)
                {
                    hasTwoSolutions = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("HasTwoSolutions"));
                    OnPropertyChanged(new PropertyChangedEventArgs("HasOneSolution"));
                }
            }

            get
            {
                return hasTwoSolutions;
            }
        }

        public bool HasOneSolution
        {
            get 
            {
                return !hasTwoSolutions;
            }
        }

        void CalculateNewSolutions()
        {
            if (A == 0 && B == 0 && C == 0)
            {
                Solution1 = new Complex(0, 0);
                HasTwoSolutions = false;
                return;
            }

            if (A == 0)
            {
                Solution1 = new Complex(-C / B, 0);
                HasTwoSolutions = false;
                return;
            }

            double discriminant = B * B - 4 * A * C;
            double denominator = 2 * A;
            double real = -B / denominator;
            double imaginary =
                Math.Sqrt(Math.Abs(discriminant)) / denominator;

            if (discriminant == 0)
            {
                Solution1 = new Complex(real, 0);
                HasTwoSolutions = false;
                return;
            }

            Solution1 = new Complex(real, imaginary);
            Solution2 = new Complex(real, -imaginary);
            HasTwoSolutions = true;
        }

        protected virtual void OnPropertyChanged(PropertyChangedEventArgs args)
        {
            if (PropertyChanged != null)
                PropertyChanged(this, args);
        }
    }
}
