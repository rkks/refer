using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;
using Petzold.Phone.Xna;

namespace TiltMaze
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float GRAVITY = 1000;     // pixels per second squared
        const float BOUNCE = 2f / 3;    // fraction of velocity
        const int BALL_RADIUS = 16;
        const int BALL_SCALE = 16;
        const int WALL_WIDTH = 32;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Viewport viewport;
        Texture2D tinyTexture;

        MazeGrid mazeGrid = new MazeGrid(5, 8);
        List<Line2D> borders = new List<Line2D>();

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

            // Restrict to portrait mode
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

            // Create texture for the walls of the maze
            tinyTexture = new Texture2D(this.GraphicsDevice, 1, 1);
            tinyTexture.SetData<Color>(new Color[] { Color.White });

            // Create ball 
            ball = Texture2DExtensions.CreateBall(this.GraphicsDevice,
                                                  BALL_RADIUS * BALL_SCALE);

            ballCenter = new Vector2(ball.Width / 2, ball.Height / 2);
            ballPosition = new Vector2((viewport.Width / mazeGrid.Width) / 2,
                                       (viewport.Height / mazeGrid.Height) / 2);

            // Initialize borders collection
            borders.Clear();

            // Create Line2D objects for walls of the maze
            int cellWidth = viewport.Width / mazeGrid.Width;
            int cellHeight = viewport.Height / mazeGrid.Height;
            int halfWallWidth = WALL_WIDTH / 2;

            for (int x = 0; x < mazeGrid.Width; x++)
                for (int y = 0; y < mazeGrid.Height; y++)
                {
                    MazeCell mazeCell = mazeGrid.Cells[x, y];
                    Vector2 ll = new Vector2(x * cellWidth, (y + 1) * cellHeight);
                    Vector2 ul = new Vector2(x * cellWidth, y * cellHeight);
                    Vector2 ur = new Vector2((x + 1) * cellWidth, y * cellHeight);
                    Vector2 lr = new Vector2((x + 1) * cellWidth, (y + 1) * cellHeight);
                    Vector2 right = halfWallWidth * Vector2.UnitX;
                    Vector2 left = -right;
                    Vector2 down = halfWallWidth * Vector2.UnitY;
                    Vector2 up = -down;

                    if (mazeCell.HasLeft)
                    {
                        borders.Add(new Line2D(ll + down, ll + down + right));
                        borders.Add(new Line2D(ll + down + right, ul + up + right));
                        borders.Add(new Line2D(ul + up + right, ul + up));
                    }
                    if (mazeCell.HasTop)
                    {
                        borders.Add(new Line2D(ul + left, ul + left + down));
                        borders.Add(new Line2D(ul + left + down, ur + right + down));
                        borders.Add(new Line2D(ur + right + down, ur + right));
                    }
                    if (mazeCell.HasRight)
                    {
                        borders.Add(new Line2D(ur + up, ur + up + left));
                        borders.Add(new Line2D(ur + up + left, lr + down + left));
                        borders.Add(new Line2D(lr + down + left, lr + down));
                    }
                    if (mazeCell.HasBottom)
                    {
                        borders.Add(new Line2D(lr + right, lr + right + up));
                        borders.Add(new Line2D(lr + right + up, ll + left + up));
                        borders.Add(new Line2D(ll + left + up, ll + left));
                    }
                }
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
            Vector2 oldPosition = ballPosition;
            ballPosition += ballVelocity * elapsedSeconds;

            bool needAnotherLoop = false;

            do
            {
                needAnotherLoop = false;

                foreach (Line2D line in borders)
                {
                    Line2D shiftedLine = line.ShiftOut(BALL_RADIUS * line.Normal);
                    Line2D ballTrajectory = new Line2D(oldPosition, ballPosition);
                    Vector2 intersection = shiftedLine.SegmentIntersection(ballTrajectory);
                    float angleDiff = MathHelper.WrapAngle(line.Angle - ballTrajectory.Angle);

                    if (Line2D.IsValid(intersection) && angleDiff > 0 &&
                        Line2D.IsValid(Vector2.Normalize(ballVelocity)))
                    {
                        float beyond = (ballPosition - intersection).Length();
                        ballVelocity = BOUNCE * Vector2.Reflect(ballVelocity, line.Normal);
                        ballPosition = intersection + beyond * Vector2.Normalize(ballVelocity);
                        needAnotherLoop = true;
                        break;
                    }
                }
            }
            while (needAnotherLoop);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();

            // Draw the walls of the maze
            int cellWidth = viewport.Width / mazeGrid.Width;
            int cellHeight = viewport.Height / mazeGrid.Height;
            int halfWallWidth = WALL_WIDTH / 2;

            for (int x = 0; x < mazeGrid.Width; x++)
                for (int y = 0; y < mazeGrid.Height; y++)
                {
                    MazeCell mazeCell = mazeGrid.Cells[x, y];

                    if (mazeCell.HasLeft)
                    {
                        Rectangle rect = new Rectangle(x * cellWidth, 
                                                       y * cellHeight - halfWallWidth, 
                                                       halfWallWidth, cellHeight + WALL_WIDTH);
                        spriteBatch.Draw(tinyTexture, rect, Color.Green);
                    }

                    if (mazeCell.HasRight)
                    {
                        Rectangle rect = new Rectangle((x + 1) * cellWidth - halfWallWidth,
                                                       y * cellHeight - halfWallWidth,
                                                       halfWallWidth, cellHeight + WALL_WIDTH);
                        spriteBatch.Draw(tinyTexture, rect, Color.Green);
                    }

                    if (mazeCell.HasTop)
                    {
                        Rectangle rect = new Rectangle(x * cellWidth - halfWallWidth, 
                                                       y * cellHeight, 
                                                       cellWidth + WALL_WIDTH, halfWallWidth);
                        spriteBatch.Draw(tinyTexture, rect, Color.Green);
                    }

                    if (mazeCell.HasBottom)
                    {
                        Rectangle rect = new Rectangle(x * cellWidth - halfWallWidth,
                                                       (y + 1) * cellHeight - halfWallWidth,
                                                       cellWidth + WALL_WIDTH, halfWallWidth);
                        spriteBatch.Draw(tinyTexture, rect, Color.Green);
                    }
                }

            // Draw the ball
            spriteBatch.Draw(ball, ballPosition, null, Color.Pink, 0,
                             ballCenter, 1f / BALL_SCALE, SpriteEffects.None, 0);

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
