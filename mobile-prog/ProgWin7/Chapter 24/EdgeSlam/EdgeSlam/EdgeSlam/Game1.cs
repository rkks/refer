using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;
using Petzold.Phone.Xna;

namespace EdgeSlam
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float GRAVITY = 1000;     // pixels per second squared
        const float BOUNCE = 2f / 3;    // fraction of velocity
        const int BALL_RADIUS = 16;
        const int BALL_SCALE = 16;
        const int HIT = 1;
        const int PENALTY = -5;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Viewport viewport;
        Vector2 screenCenter;

        SpriteFont segoe96;
        int score;
        StringBuilder scoreText = new StringBuilder();
        Vector2 scoreCenter;

        Texture2D tinyTexture;
        int highlightedSide;
        Random rand = new Random();

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
            catch { }

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
            screenCenter = new Vector2(viewport.Width / 2, viewport.Height / 2);

            ball = Texture2DExtensions.CreateBall(this.GraphicsDevice,
                                                  BALL_RADIUS * BALL_SCALE);

            ballCenter = new Vector2(ball.Width / 2, ball.Height / 2);
            ballPosition = screenCenter;

            tinyTexture = new Texture2D(this.GraphicsDevice, 1, 1);
            tinyTexture.SetData<Color>(new Color[] { Color.White });

            segoe96 = this.Content.Load<SpriteFont>("Segoe96");
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

            // Check for bouncing off edge
            bool needAnotherLoop = false;
            bool needAnotherSide = false;

            do
            {
                needAnotherLoop = false;

                if (ballPosition.X - BALL_RADIUS < 0)
                {
                    score += highlightedSide == 0 ? HIT : PENALTY;
                    ballPosition.X = -ballPosition.X + 2 * BALL_RADIUS;
                    ballVelocity.X *= -BOUNCE;
                    needAnotherLoop = true;
                }
                else if (ballPosition.X + BALL_RADIUS > viewport.Width)
                {
                    score += highlightedSide == 2 ? HIT : PENALTY;
                    ballPosition.X = -ballPosition.X - 2 * (BALL_RADIUS - viewport.Width);
                    ballVelocity.X *= -BOUNCE;
                    needAnotherLoop = true;
                }
                else if (ballPosition.Y - BALL_RADIUS < 0)
                {
                    score += highlightedSide == 1 ? HIT : PENALTY;
                    ballPosition.Y = -ballPosition.Y + 2 * BALL_RADIUS;
                    ballVelocity.Y *= -BOUNCE;
                    needAnotherLoop = true;
                }
                else if (ballPosition.Y + BALL_RADIUS > viewport.Height)
                {
                    score += highlightedSide == 3 ? HIT : PENALTY;
                    ballPosition.Y = -ballPosition.Y - 2 * (BALL_RADIUS - viewport.Height);
                    ballVelocity.Y *= -BOUNCE;
                    needAnotherLoop = true;
                }
                needAnotherSide |= needAnotherLoop;
            }
            while (needAnotherLoop);

            if (needAnotherSide)
            {
                scoreText.Remove(0, scoreText.Length);
                scoreText.Append(score);
                scoreCenter = segoe96.MeasureString(scoreText) / 2;
                highlightedSide = rand.Next(4);
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();

            Rectangle rect = new Rectangle();

            switch (highlightedSide)
            {
                case 0: rect = new Rectangle(0, 0, 3, viewport.Height); break;
                case 1: rect = new Rectangle(0, 0, viewport.Width, 3); break;
                case 2: rect = new Rectangle(viewport.Width - 3, 0, 3, viewport.Height); break;
                case 3: rect = new Rectangle(3, viewport.Height - 3, viewport.Width, 3); break;
            }

            spriteBatch.Draw(tinyTexture, rect, Color.White);

            spriteBatch.DrawString(segoe96, scoreText, screenCenter, 
                                   Color.LightBlue, 0, 
                                   scoreCenter, 1, SpriteEffects.None, 0);

            spriteBatch.Draw(ball, ballPosition, null, Color.Pink, 0,
                             ballCenter, 1f / BALL_SCALE, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
