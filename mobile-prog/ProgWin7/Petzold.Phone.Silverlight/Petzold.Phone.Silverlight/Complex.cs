using System;

namespace Petzold.Phone.Silverlight
{
    public struct Complex : IFormattable
    {
        public double Real { get; set; }
        public double Imaginary { get; set; }

        public Complex(double real, double imaginary) : this()
        {
            Real = real;
            Imaginary = imaginary;
        }

        public override string ToString()
        {
            if (Imaginary == 0)
                return Real.ToString();

            return String.Format("{0} {1} {2}i",
                                 Real,
                                 Math.Sign(Imaginary) >= 1 ? "+" : "–",
                                 Math.Abs(Imaginary));
        }
        public string ToString(string format, IFormatProvider provider)
        {
            if (Imaginary == 0)
                return Real.ToString(format, provider);

            return String.Format(provider, 
                                 "{0} {1} {2}i",
                                 Real.ToString(format, provider),
                                 Math.Sign(Imaginary) >= 1 ? "+" : "–",
                                 Math.Abs(Imaginary).ToString(format, provider));
        }
    }
}
