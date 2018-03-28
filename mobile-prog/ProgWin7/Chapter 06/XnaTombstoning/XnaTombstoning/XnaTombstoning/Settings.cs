using System;
using System.IO;
using System.IO.IsolatedStorage;
using System.Xml.Serialization;
using Microsoft.Xna.Framework;

namespace XnaTombstoning
{
    public class Settings
    {
        const string filename = "settings.xml";

        // Application settings
        public Color BackgroundColor { set; get; }

        public Settings()
        {
            BackgroundColor = Color.Navy;
        }

        public void Save()
        {
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            IsolatedStorageFileStream stream = storage.CreateFile(filename);
            XmlSerializer xml = new XmlSerializer(GetType());
            xml.Serialize(stream, this);
            stream.Close();
            stream.Dispose();
        }

        public static Settings Load()
        {
            IsolatedStorageFile storage = IsolatedStorageFile.GetUserStoreForApplication();
            Settings settings;

            if (storage.FileExists(filename))
            {
                IsolatedStorageFileStream stream = storage.OpenFile("settings.xml", FileMode.Open);
                XmlSerializer xml = new XmlSerializer(typeof(Settings));
                settings = xml.Deserialize(stream) as Settings;
                stream.Close();
                stream.Dispose();
            }
            else
            {
                settings = new Settings();
            }

            return settings;
        }
    }
}
