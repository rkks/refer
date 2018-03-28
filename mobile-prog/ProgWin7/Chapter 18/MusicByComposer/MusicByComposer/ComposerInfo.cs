using System;
using System.Collections.Generic;

namespace MusicByComposer
{
    public class ComposerInfo
    {
        public ComposerInfo(string composer, List<AlbumInfo> albums)
        {
            Composer = composer;
            albums.Sort();
            Albums = albums;
        }

        public string Composer { protected set; get; }

        public IList<AlbumInfo> Albums { protected set; get; }
    }
}
