using System;
using System.Collections.Generic;
using System.IO;
using System.IO.IsolatedStorage;
using System.Windows;
using System.Windows.Ink;
using System.Windows.Media;
using System.Xml.Serialization;

namespace Jot
{
    public class JotAppSettings
    {
        public JotAppSettings()
        {
            this.PageNumber = 0;
            this.Foreground = (Color)Application.Current.Resources["PhoneForegroundColor"];
            this.Background = (Color)Application.Current.Resources["PhoneBackgroundColor"];
            this.StrokeWidth = 3;
        }

        // Public properties -- the actual application settins
        public List<StrokeCollection> StrokeCollections { get; set; }
        public int PageNumber { set; get; }
        public Color Foreground { set; get; }
        public Color Background { set; get; }
        public int StrokeWidth { set; get; }

        public static JotAppSettings Load()
        {
            JotAppSettings settings;
            IsolatedStorageFile iso = IsolatedStorageFile.GetUserStoreForApplication();

            if (iso.FileExists("settings.xml"))
            {
                IsolatedStorageFileStream stream = iso.OpenFile("settings.xml", FileMode.Open);
                StreamReader reader = new StreamReader(stream);

                XmlSerializer ser = new XmlSerializer(typeof(JotAppSettings));
                settings = ser.Deserialize(reader) as JotAppSettings;

                reader.Close();
            }
            else
            {
                // Create and initialize new JotAppSettings object
                settings = new JotAppSettings();
                settings.StrokeCollections = new List<StrokeCollection>();
                settings.StrokeCollections.Add(new StrokeCollection());
            }

            iso.Dispose();
            return settings;
        }

        public void Save()
        {
            IsolatedStorageFile iso = IsolatedStorageFile.GetUserStoreForApplication();
            IsolatedStorageFileStream stream = iso.CreateFile("settings.xml");
            StreamWriter writer = new StreamWriter(stream);

            XmlSerializer ser = new XmlSerializer(typeof(JotAppSettings));
            ser.Serialize(writer, this);

            writer.Close();
            iso.Dispose();
        }
    }
}
