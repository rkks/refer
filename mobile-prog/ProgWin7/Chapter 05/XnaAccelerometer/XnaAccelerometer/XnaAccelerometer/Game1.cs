using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;

namespace XnaAccelerometer
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float BUBBLE_RADIUS_MAX = 25;
        const float BUBBLE_RADIUS_MIN = 12;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Vector2 screenCenter;
        float screenRadius;     // less BUBBLE_RADIUS_MAX

        Texture2D bubbleTexture;
        Vector2 bubbleCenter;
        Vector2 bubblePosition;
        float bubbleScale;

        Vector3 accelerometerVector;
        object accelerometerVectorLock = new object();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);
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
            lock (accelerometerVectorLock)
            {
                accelerometerVector = new Vector3((float)args.X, (float)args.Y, (float)args.Z);
            }
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Viewport viewport = this.GraphicsDevice.Viewport;
            screenCenter = new Vector2(viewport.Width / 2, viewport.Height / 2);
            screenRadius = Math.Min(screenCenter.X, screenCenter.Y) - BUBBLE_RADIUS_MAX;

            bubbleTexture = this.Content.Load<Texture2D>("Bubble");
            bubbleCenter = new Vector2(bubbleTexture.Width / 2, bubbleTexture.Height / 2);
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            Vector3 accVector;

            lock (accelerometerVectorLock)
            {
                accVector = accelerometerVector;
            }

            int sign = this.Window.CurrentOrientation == 
                                            DisplayOrientation.LandscapeLeft ? 1 : -1;
            
            bubblePosition = new Vector2(screenCenter.X + sign * screenRadius * accVector.Y, 
                                         screenCenter.Y + sign * screenRadius * accVector.X);

            float bubbleRadius = BUBBLE_RADIUS_MIN + (1 - accVector.Z) / 2 *
                                    (BUBBLE_RADIUS_MAX - BUBBLE_RADIUS_MIN);
            bubbleScale = bubbleRadius / (bubbleTexture.Width / 2);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(bubbleTexture, bubblePosition, null, Color.White, 0, 
                             bubbleCenter, bubbleScale, SpriteEffects.None, 0);
            spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
