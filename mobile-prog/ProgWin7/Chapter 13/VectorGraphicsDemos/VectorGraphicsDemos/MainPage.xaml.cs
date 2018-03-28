using System;
using System.Collections.Generic;
using System.IO;
using System.Windows;
using System.Windows.Resources;
using System.Xml;
using Microsoft.Phone.Controls;
using Microsoft.Phone.Shell;
using Petzold.Phone.Silverlight;

namespace VectorGraphicsDemos
{
    public partial class MainPage : PhoneApplicationPage
    {
        List<string> snippets = new List<string>();
        int snippetsIndex = 0;

        public MainPage()
        {
            InitializeComponent();

            AppbarBackButton = this.ApplicationBar.Buttons[0] as ApplicationBarIconButton;
            AppbarNextButton = this.ApplicationBar.Buttons[1] as ApplicationBarIconButton;

            ReadXamlSnippets();
            DisplayNextSnippet();
        }

        void ReadXamlSnippets()
        {
            StreamResourceInfo streamInfo = 
                Application.GetResourceStream(new Uri("Data/XamlSnippets.xml", 
                                                      UriKind.Relative)); 
            Stream stream = streamInfo.Stream;

            using (XmlReader xmlReader = XmlReader.Create(stream))
            {
                xmlReader.Read();
                xmlReader.ReadStartElement("XamlSnippets");

                while (xmlReader.ReadToFollowing("Snippet"))
                {
                    string strInp = xmlReader.ReadInnerXml();
                    StringReader strReader = new StringReader(strInp);
                    StringWriter strWriter = new StringWriter();
                    string strLine;
                    string strUnIndent = null;

                    while (null != (strLine = strReader.ReadLine()))
                    {
                        if (strUnIndent == null)
                        {
                            if (strLine.Trim().Length == 0)
                                continue;

                            int unIndent = strLine.Length - strLine.TrimStart().Length;
                            strUnIndent = new string(' ', unIndent);
                        }

                        if (strLine.StartsWith(strUnIndent))
                        {
                            strLine = strLine.Substring(strUnIndent.Length);
                        }

                        strWriter.WriteLine(strLine);
                    }
                    snippets.Add(strWriter.ToString());
                }
            }
        }

        void OnXamlCruncherTextBoxXamlResult(object sender, XamlCruncherEventArgs args)
        {
            if (args.Error != null)
            {
                statusText.Text = args.Error;
            }
            else
            {
                container.Child = args.Element;
                statusText.Text = "OK";
            }
        }

        void OnAppbarBackClick(object sender, EventArgs args)
        {
            snippetsIndex--;
            DisplayNextSnippet();
        }

        void OnAppbarNextClick(object sender, EventArgs args)
        {
            snippetsIndex++;
            DisplayNextSnippet();
        }

        void DisplayNextSnippet()
        {
            txtbox.Text = snippets[snippetsIndex];

            AppbarNextButton.IsEnabled = snippetsIndex < snippets.Count - 1;
            AppbarBackButton.IsEnabled = snippetsIndex > 0;
        }
    }
}