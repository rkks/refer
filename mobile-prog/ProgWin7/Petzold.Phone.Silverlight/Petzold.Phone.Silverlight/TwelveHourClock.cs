using System;
using System.ComponentModel;

namespace Petzold.Phone.Silverlight
{
    public class TwelveHourClock : Clock
    {
        int hour12;
        bool isam, ispm;

        public int Hour12
        {
            protected set
            {
                if (value != hour12)
                {
                    hour12 = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("Hour12"));
                }
            }
            get
            {
                return hour12;
            }
        }

        public bool IsAm
        {
            protected set
            {
                if (value != isam)
                {
                    isam = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("IsAm"));
                }
            }
            get
            {
                return isam;
            }
        }

        public bool IsPm
        {
            protected set
            {
                if (value != ispm)
                {
                    ispm = value;
                    OnPropertyChanged(new PropertyChangedEventArgs("IsPm"));
                }
            }
            get
            {
                return ispm;
            }
        }

        protected override void OnPropertyChanged(PropertyChangedEventArgs args)
        {
            if (args.PropertyName == "Hour")
            {
                Hour12 = (Hour - 1) % 12 + 1;
                IsAm = Hour < 12;
                IsPm = !IsAm;
            }

            base.OnPropertyChanged(args);
        }
    }
}
