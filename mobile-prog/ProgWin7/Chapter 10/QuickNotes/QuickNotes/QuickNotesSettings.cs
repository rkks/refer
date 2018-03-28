using System;
using System.IO.IsolatedStorage;
using System.Windows;

namespace QuickNotes
{
    public class QuickNotesSettings
    {
        public QuickNotesSettings()
        {
            this.Text = "";
            this.FontSize = (double)Application.Current.Resources["PhoneFontSizeMediumLarge"];
        }

        public string Text { set; get; }
        public double FontSize { set; get; }

        public static QuickNotesSettings Load()
        {
            IsolatedStorageSettings isoSettings = IsolatedStorageSettings.ApplicationSettings;
            QuickNotesSettings settings;

            if (!isoSettings.TryGetValue<QuickNotesSettings>("settings", out settings))
                settings = new QuickNotesSettings();

            return settings;
        }

        public void Save()
        {
            IsolatedStorageSettings isoSettings = IsolatedStorageSettings.ApplicationSettings;
            isoSettings["settings"] = this;
        }
    }
}
