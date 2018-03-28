using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;
using Petzold.Phone.Xna;

namespace TiltAndRoll
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float GRAVITY = 1000;     // pixels per second squared
        const int BALL_RADIUS = 16;
        const int BALL_SCALE = 16;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Viewport viewport;
        Texture2D ball;
        Vector2 ballCenter;
        Vector2 ballPosition;
        Vector2 ballVelocity = Vector2.Zero;
        Vector3 oldAcceleration, acceleration;
        object accelerationLock = new object();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Restrict orientation to portrait
            graphics.SupportedOrientations = DisplayOrientation.Portrait;
            graphics.PreferredBackBufferWidth = 480;
            graphics.PreferredBackBufferHeight = 768;
        }

        protected override void Initialize()
        {
            Accelerometer accelerometer = new Accelerometer();
            accelerometer.ReadingChanged += OnAccelerometerReadingChanged;

            try { accelerometer.Start(); }
            catch {}

            base.Initialize();
        }

        void OnAccelerometerReadingChanged(object sender, AccelerometerReadingEventArgs args)
        {
            lock (accelerationLock)
            {
                acceleration = 0.5f * oldAcceleration +
                               0.5f * new Vector3((float)args.X, (float)args.Y, (float)args.Z);
                oldAcceleration = acceleration;
            }
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            viewport = this.GraphicsDevice.Viewport;
            ball = Texture2DExtensions.CreateBall(this.GraphicsDevice, 
                                                  BALL_RADIUS * BALL_SCALE);

            ballCenter = new Vector2(ball.Width / 2, ball.Height / 2);
            ballPosition = new Vector2(viewport.Width / 2, viewport.Height / 2);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Calculate new velocity and position
            Vector2 acceleration2D = Vector2.Zero;

            lock (accelerationLock)
            {
                acceleration2D = new Vector2(acceleration.X, -acceleration.Y);
            }
            float elapsedSeconds = (float)gameTime.ElapsedGameTime.TotalSeconds;
            ballVelocity += GRAVITY * acceleration2D * elapsedSeconds;
            ballPosition += ballVelocity * elapsedSeconds;

            // Check for hitting edge
            if (ballPosition.X - BALL_RADIUS < 0)
            {
                ballPosition.X = BALL_RADIUS;
                ballVelocity.X = 0;
            }
            if (ballPosition.X + BALL_RADIUS > viewport.Width)
            {
                ballPosition.X = viewport.Width - BALL_RADIUS;
                ballVelocity.X = 0;
            }
            if (ballPosition.Y - BALL_RADIUS < 0)
            {
                ballPosition.Y = BALL_RADIUS;
                ballVelocity.Y = 0;
            }
            if (ballPosition.Y + BALL_RADIUS > viewport.Height)
            {
                ballPosition.Y = viewport.Height - BALL_RADIUS;
                ballVelocity.Y = 0;
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(ball, ballPosition, null, Color.Pink, 0, 
                             ballCenter, 1f / BALL_SCALE, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
