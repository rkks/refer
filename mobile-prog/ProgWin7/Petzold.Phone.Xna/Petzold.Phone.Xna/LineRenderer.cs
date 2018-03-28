using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Petzold.Phone.Xna
{
    public class LineRenderer
    {
        RenderTarget2D lineTexture;

        public LineRenderer(GraphicsDevice graphicsDevice)
        {
            lineTexture = new RenderTarget2D(graphicsDevice, 2, 3);

            graphicsDevice.SetRenderTarget(lineTexture);
            graphicsDevice.Clear(Color.White);
            graphicsDevice.SetRenderTarget(null);
        }

        public void DrawLine(SpriteBatch spriteBatch, 
                             Vector2 point1, Vector2 point2,
                             float thickness, Color color)
        {
            Vector2 difference = point2 - point1;
            float length = difference.Length();
            float angle = (float)Math.Atan2(difference.Y, difference.X);
            spriteBatch.Draw(lineTexture, point1, null, color, angle,
                             new Vector2(0, 1), 
                             new Vector2(length / 2, thickness / 3),
                             SpriteEffects.None, 0);
        }
    }
}
