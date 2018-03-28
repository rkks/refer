using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;

namespace AccelerometerVisualization
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const int BALL_RADIUS = 8;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Viewport viewport;
        SpriteFont segoe14;
        StringBuilder stringBuilder = new StringBuilder();

        int unitRadius;
        Vector2 screenCenter;
        Texture2D backgroundTexture;
        Vector2 backgroundTextureCenter;
        Texture2D ballTexture;
        Vector2 ballTextureCenter;
        Vector2 ballPosition;
        float ballScale;
        bool isZNegative;

        Vector3 accelerometerVector;
        object accerlerometerVectorLock = new object();

        Vector3 oldAcceleration;
        Vector3 minAcceleration = 2 * Vector3.One;
        Vector3 maxAcceleration = -2 * Vector3.One;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Landscape but leave room for the status bar
            graphics.PreferredBackBufferWidth = 728;
            graphics.PreferredBackBufferHeight = 480;
        }

        protected override void Initialize()
        {
            Accelerometer accelerometer = new Accelerometer();
            accelerometer.ReadingChanged += OnAccelerometerReadingChanged;

            try
            {
                accelerometer.Start();
            }
            catch
            {
            }

            base.Initialize();
        }

        void OnAccelerometerReadingChanged(object sender, AccelerometerReadingEventArgs args)
        {
            lock (accerlerometerVectorLock)
            {
                accelerometerVector = new Vector3((float)args.X, (float)args.Y, (float)args.Z);
            }
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(this.GraphicsDevice);

            // Get screen and font information
            viewport = this.GraphicsDevice.Viewport;
            screenCenter = new Vector2(viewport.Width / 2, viewport.Height / 2);
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");

            // This is the pixel equivalent of a vector magnitude of 1
            unitRadius = (viewport.Height - BALL_RADIUS) / 2;

            // Create and draw background texture
            backgroundTexture = 
                new Texture2D(this.GraphicsDevice, viewport.Height, viewport.Height);
            backgroundTextureCenter = 
                new Vector2(viewport.Height / 2, viewport.Height / 2);

            Color[] pixels = new Color[backgroundTexture.Width * backgroundTexture.Height];

            // Draw horizontal line
            for (int x = 0; x < backgroundTexture.Width; x++)
                SetPixel(backgroundTexture, pixels, 
                         x, backgroundTexture.Height / 2, Color.White);

            // Draw vertical line
            for (int y = 0; y < backgroundTexture.Height; y++)
                SetPixel(backgroundTexture, pixels, 
                         backgroundTexture.Width / 2, y, Color.White);

            // Draw circles
            DrawCenteredCircle(backgroundTexture, pixels, unitRadius, Color.White);
            DrawCenteredCircle(backgroundTexture, pixels, 3 * unitRadius / 4, Color.Gray);
            DrawCenteredCircle(backgroundTexture, pixels, unitRadius / 2, Color.White);
            DrawCenteredCircle(backgroundTexture, pixels, unitRadius / 4, Color.Gray);
            DrawCenteredCircle(backgroundTexture, pixels, BALL_RADIUS, Color.White);

            // Set the pixels to the background texture
            backgroundTexture.SetData<Color>(pixels);

            // Create and draw ball texture
            ballTexture = new Texture2D(this.GraphicsDevice, 
                                        2 * BALL_RADIUS, 2 * BALL_RADIUS);
            ballTextureCenter = new Vector2(BALL_RADIUS, BALL_RADIUS);
            pixels = new Color[ballTexture.Width * ballTexture.Height];
            DrawFilledCenteredCircle(ballTexture, pixels, BALL_RADIUS);
            ballTexture.SetData<Color>(pixels);
        }

        void DrawCenteredCircle(Texture2D texture, Color[] pixels, int radius, Color clr)
        {
            Point center = new Point(texture.Width / 2, texture.Height / 2);
            int halfPoint = (int)(0.707 * radius + 0.5);

            for (int y = -halfPoint; y <= halfPoint; y++)
            {
                int x1 = (int)Math.Round(Math.Sqrt(radius * radius - Math.Pow(y, 2)));
                int x2 = -x1;

                SetPixel(texture, pixels, x1 + center.X, y + center.Y, clr);
                SetPixel(texture, pixels, x2 + center.X, y + center.Y, clr);

                // Since symmetric, just swap coordinates for other piece
                SetPixel(texture, pixels, y + center.X, x1 + center.Y, clr);
                SetPixel(texture, pixels, y + center.X, x2 + center.Y, clr);
            }
        }

        void DrawFilledCenteredCircle(Texture2D texture, Color[] pixels, int radius)
        {
            Point center = new Point(texture.Width / 2, texture.Height / 2);

            for (int y = -radius; y < radius; y++)
            {
                int x1 = (int)Math.Round(Math.Sqrt(radius * radius - Math.Pow(y, 2)));

                for (int x = -x1; x < x1; x++)
                    SetPixel(texture, pixels, x + center.X, y + center.Y, Color.White);
            }
        }

        void SetPixel(Texture2D texture, Color[] pixels, int x, int y, Color clr)
        {
            pixels[y * texture.Width + x] = clr;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            Vector3 newAcceleration = Vector3.Zero;

            lock (accerlerometerVectorLock)
            {
                newAcceleration = accelerometerVector;
            }

            maxAcceleration = Vector3.Max(maxAcceleration, newAcceleration);
            minAcceleration = Vector3.Min(minAcceleration, newAcceleration);

            // Low-pass filter smoothing
            Vector3 avgAcceleration = 0.5f * oldAcceleration + 0.5f * newAcceleration;

            stringBuilder.Remove(0, stringBuilder.Length);
            stringBuilder.AppendFormat("Raw: ({0:F2}, {1:F2}, {2:F2}) = {3:F2}\n", 
                                       newAcceleration.X, newAcceleration.Y, 
                                       newAcceleration.Z, newAcceleration.Length());
            stringBuilder.AppendFormat("Avg: ({0:F2}, {1:F2}, {2:F2}) = {3:F2}\n", 
                                       avgAcceleration.X, avgAcceleration.Y, 
                                       avgAcceleration.Z, avgAcceleration.Length());
            stringBuilder.AppendFormat("Min: ({0:F2}, {1:F2}, {2:F2}) = {3:F2}\n", 
                                       minAcceleration.X, minAcceleration.Y, 
                                       minAcceleration.Z, minAcceleration.Length());
            stringBuilder.AppendFormat("Max: ({0:F2}, {1:F2}, {2:F2}) = {3:F2}", 
                                       maxAcceleration.X, maxAcceleration.Y, 
                                       maxAcceleration.Z, maxAcceleration.Length());

            ballScale = avgAcceleration.Length();
            int sign = this.Window.CurrentOrientation == 
                                            DisplayOrientation.LandscapeLeft ? 1 : -1;
            ballPosition = 
                new Vector2(screenCenter.X + sign * unitRadius * avgAcceleration.Y / ballScale,
                            screenCenter.Y + sign * unitRadius * avgAcceleration.X / ballScale);
            isZNegative = avgAcceleration.Z < 0;

            oldAcceleration = avgAcceleration;

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(backgroundTexture, screenCenter, null, Color.White, 0,
                             backgroundTextureCenter, 1, SpriteEffects.None, 0);
            spriteBatch.Draw(ballTexture, ballPosition, null,
                             isZNegative ? Color.Red : Color.Lime, 0,
                             ballTextureCenter, ballScale, SpriteEffects.None, 0);
            spriteBatch.DrawString(segoe14, stringBuilder, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
