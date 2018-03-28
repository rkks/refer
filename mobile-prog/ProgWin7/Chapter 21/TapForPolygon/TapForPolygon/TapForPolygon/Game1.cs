using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Petzold.Phone.Xna;

namespace TapForPolygon
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        LineRenderer lineRenderer;
        Vector2 center;
        float radius;
        int vertexCount = 3;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Enable taps
            TouchPanel.EnabledGestures = GestureType.Tap;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Viewport viewport= this.GraphicsDevice.Viewport;
            center = new Vector2(viewport.Width / 2, viewport.Height / 2);
            radius = Math.Min(center.X, center.Y) - 10;

            lineRenderer = new LineRenderer(this.GraphicsDevice);
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            while (TouchPanel.IsGestureAvailable)
                if (TouchPanel.ReadGesture().GestureType == GestureType.Tap)
                    vertexCount++;
            
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();

            Vector2 saved = new Vector2();

            for (int vertex = 0; vertex <= vertexCount; vertex++)
            {
                double angle = vertex * 2 * Math.PI / vertexCount;
                float x = center.X + radius * (float)Math.Sin(angle);
                float y = center.Y - radius * (float)Math.Cos(angle);
                Vector2 point = new Vector2(x, y);

                if (vertex != 0)
                {
                    lineRenderer.DrawLine(spriteBatch, center, point, 3, Color.Red);
                    lineRenderer.DrawLine(spriteBatch, saved, point, 3, Color.Red);
                }
                saved = point;
            }
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
