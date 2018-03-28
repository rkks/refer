using System;
using System.Windows.Media.Imaging;
using Microsoft.Xna.Framework.Media;

namespace MusicByComposer
{
    public class AlbumInfo : IComparable<AlbumInfo>
    {
        BitmapImage albumArt;
        BitmapImage thumbnailArt;

        public AlbumInfo(string shortAlbumName, Album album)
        {
            this.ShortAlbumName = shortAlbumName;
            this.Album = album;
        }

        public string ShortAlbumName { protected set; get; }

        public Album Album { protected set; get; }

        public BitmapSource AlbumArt
        {
            get
            {
                if (albumArt == null && Album.HasArt)
                {
                    BitmapImage bitmapImage = new BitmapImage();
                    bitmapImage.SetSource(Album.GetAlbumArt());
                    albumArt = bitmapImage;
                }
                return albumArt;
            }
        }

        public BitmapSource ThumbnailArt
        {
            get
            {
                if (thumbnailArt == null && Album.HasArt)
                {
                    BitmapImage bitmapImage = new BitmapImage();
                    bitmapImage.SetSource(Album.GetThumbnail());
                    thumbnailArt = bitmapImage;
                }
                return thumbnailArt;
            }
        }

        public int CompareTo(AlbumInfo albumInfo)
        {
            return ShortAlbumName.CompareTo(albumInfo.ShortAlbumName);
        }
    }
}
