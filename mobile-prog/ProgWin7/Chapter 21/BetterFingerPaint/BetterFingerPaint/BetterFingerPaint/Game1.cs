using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Petzold.Phone.Xna;

namespace BetterFingerPaint
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D canvas;
        Color[] pixels;
        List<float> xCollection = new List<float>();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Enable FreeDrag gestures
            TouchPanel.EnabledGestures = GestureType.FreeDrag;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Rectangle viewportBounds = this.GraphicsDevice.Viewport.Bounds;
            canvas = new Texture2D(this.GraphicsDevice, viewportBounds.Width,
                                                        viewportBounds.Height);

            pixels = new Color[canvas.Width * canvas.Height];

            for (int i = 0; i < pixels.Length; i++)
                pixels[i] = Color.Navy;

            canvas.SetData<Color>(pixels);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            bool canvasNeedsUpdate = false;
            int yMinUpdate = Int32.MaxValue, yMaxUpdate = 0;

            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                if (gesture.GestureType == GestureType.FreeDrag &&
                    gesture.Delta != Vector2.Zero)
                {
                    Vector2 point1 = gesture.Position - gesture.Delta;
                    Vector2 point2 = gesture.Position;
                    float radius = 12;

                    RoundCappedLine line = new RoundCappedLine(point1, point2, radius);

                    int yMin = (int)(Math.Min(point1.Y, point2.Y) - radius - 1);
                    int yMax = (int)(Math.Max(point1.Y, point2.Y) + radius + 1);

                    yMin = Math.Max(0, Math.Min(canvas.Height, yMin));
                    yMax = Math.Max(0, Math.Min(canvas.Height, yMax));

                    for (int y = yMin; y < yMax; y++)
                    {
                        xCollection.Clear();
                        line.GetAllX(y, xCollection);

                        if (xCollection.Count == 2)
                        {
                            int xMin = (int)(Math.Min(xCollection[0], 
                                                      xCollection[1]) + 0.5f);
                            int xMax = (int)(Math.Max(xCollection[0], 
                                                      xCollection[1]) + 0.5f);

                            xMin = Math.Max(0, Math.Min(canvas.Width, xMin));
                            xMax = Math.Max(0, Math.Min(canvas.Width, xMax));

                            for (int x = xMin; x < xMax; x++)
                            {
                                pixels[y * canvas.Width + x] = Color.Red;
                            }
                            yMinUpdate = Math.Min(yMinUpdate, yMin);
                            yMaxUpdate = Math.Max(yMaxUpdate, yMax);
                            canvasNeedsUpdate = true;
                        }
                    }
                }
            }

            if (canvasNeedsUpdate)
            {
                this.GraphicsDevice.Textures[0] = null;

                int height = yMaxUpdate - yMinUpdate;
                Rectangle rect = new Rectangle(0, yMinUpdate, canvas.Width, height);
                canvas.SetData<Color>(0, rect, pixels, 
                            yMinUpdate * canvas.Width, height * canvas.Width);
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();
            spriteBatch.Draw(canvas, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
