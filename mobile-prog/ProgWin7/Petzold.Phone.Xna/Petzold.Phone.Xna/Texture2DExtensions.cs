using System;
using System.IO;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Media;
using Microsoft.Phone.Shell;

namespace Petzold.Phone.Xna
{
    public static class Texture2DExtensions
    {
        public static void SaveToPhoneServiceState(this Texture2D texture, string key)
        {
            MemoryStream memoryStream = new MemoryStream();
            texture.SaveAsPng(memoryStream, texture.Width, texture.Height);
            PhoneApplicationService.Current.State[key] = memoryStream.GetBuffer();
        }

        public static Texture2D LoadFromPhoneServiceState(GraphicsDevice graphicsDevice, string key)
        {
            Texture2D texture = null;

            if (PhoneApplicationService.Current.State.ContainsKey(key))
            {
                byte[] buffer = PhoneApplicationService.Current.State[key] as byte[];
                MemoryStream memoryStream = new MemoryStream(buffer);
                texture = Texture2D.FromStream(graphicsDevice, memoryStream);
                memoryStream.Close();
            }
            return texture;
        }

        public static void SaveToPhotoLibrary(this Texture2D texture, string filename)
        {
            MemoryStream memoryStream = new MemoryStream();
            texture.SaveAsJpeg(memoryStream, texture.Width, texture.Height);
            memoryStream.Position = 0;
            MediaLibrary mediaLibrary = new MediaLibrary();
            mediaLibrary.SavePicture(filename, memoryStream);
            memoryStream.Close();
        }

        public static Texture2D CreateBall(GraphicsDevice graphicsDevice, int radius)
        {
            Texture2D ball = new Texture2D(graphicsDevice, 2 * radius, 2 * radius);
            Color[] pixels = new Color[ball.Width * ball.Height];
            int radiusSquared = radius * radius;

            for (int y = -radius; y < radius; y++)
            {
                int x2 = (int)Math.Round(Math.Sqrt(Math.Pow(radius, 2) - y * y));
                int x1 = -x2;

                for (int x = x1; x < x2; x++)
                    pixels[(int)(ball.Width * (y + radius) + x + radius)] = Color.White;
            }
            ball.SetData<Color>(pixels);
            return ball;
        }
    }
}
