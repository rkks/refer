using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework.Media;

namespace MusicByComposer
{
    public class MusicPresenter
    {
        // Static constructor
        static MusicPresenter()
        {
            if (Current == null)
                Current = new MusicPresenter();
        }

        // Instance constructor
        public MusicPresenter()
        {
            // Make this class a singleton
            if (MusicPresenter.Current != null)
            {
                this.Composers = MusicPresenter.Current.Composers;
                return;
            }

            MediaLibrary mediaLib = new MediaLibrary();
            Dictionary<string, List<AlbumInfo>> albumsByComposer = 
                            new Dictionary<string, List<AlbumInfo>>();

            foreach (Album album in mediaLib.Albums)
            {
                int indexOfColon = album.Name.IndexOf(':');

                // Check for pathological cases
                if (indexOfColon != -1 &&
                    // Colon at beginning of album name
                    (indexOfColon == 0 ||
                    // Colon at end of album name    
                    indexOfColon == album.Name.Length - 1 ||
                    // nothing before colon
                    album.Name.Substring(0, indexOfColon).Trim().Length == 0 ||
                    // nothing after colon
                    album.Name.Substring(indexOfColon + 1).Trim().Length == 0))
                {
                    indexOfColon = -1;
                }

                // Main logic for albums with composers
                if (indexOfColon != -1)
                {
                    string[] albumComposers = album.Name.Substring(0, indexOfColon).Split(',');
                    string shortAlbumName = album.Name.Substring(indexOfColon + 1).Trim();
                    bool atLeastOneEntry = false;

                    foreach (string composer in albumComposers)
                    {
                        string trimmedComposer = composer.Trim();

                        if (trimmedComposer.Length > 0)
                        {
                            atLeastOneEntry = true;

                            if (!albumsByComposer.ContainsKey(trimmedComposer))
                                albumsByComposer.Add(trimmedComposer, new List<AlbumInfo>());

                            albumsByComposer[trimmedComposer].Add(new AlbumInfo(shortAlbumName, album));
                        }
                    }

                    // Another pathological case: Just commas before colon
                    if (!atLeastOneEntry)
                    {
                        indexOfColon = -1;
                    }
                }

                // The "Other" category is for albums without composers
                if (indexOfColon == -1)
                {
                    if (!albumsByComposer.ContainsKey("Other"))
                        albumsByComposer.Add("Other", new List<AlbumInfo>());

                    albumsByComposer["Other"].Add(new AlbumInfo(album.Name, album));
                }
            }

            mediaLib.Dispose();

            // Transfer Dictionary keys to List for sorting
            List<string> composerList = new List<string>();

            foreach (string composer in albumsByComposer.Keys)
                composerList.Add(composer);

            (composerList as List<string>).Sort();

            // Construct Composers property
            Composers = new List<ComposerInfo>();

            foreach (string composer in composerList)
                Composers.Add(new ComposerInfo(composer, albumsByComposer[composer]));

            Current = this;
        }

        public static MusicPresenter Current { protected set; get; }

        public IList<ComposerInfo> Composers { private set; get; }
    }
}
