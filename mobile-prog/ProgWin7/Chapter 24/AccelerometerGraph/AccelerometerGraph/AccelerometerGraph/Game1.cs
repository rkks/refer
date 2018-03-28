using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Devices.Sensors;

namespace AccelerometerGraph
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        int displayWidth, displayHeight;
        Texture2D backgroundTexture;
        Texture2D graphTexture;
        uint[] pixels;
        int totalTicks;
        int oldInsertRow;
        Vector3 oldAcceleration;

        Vector3 accelerometerVector;
        object accelerometerVectorLock = new object();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            graphics.SupportedOrientations = DisplayOrientation.Portrait;
            graphics.PreferredBackBufferWidth = 480;
            graphics.PreferredBackBufferHeight = 768;
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
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            displayWidth = this.GraphicsDevice.Viewport.Width;
            displayHeight = this.GraphicsDevice.Viewport.Height;

            // Create background texture and initialize it
            int ticksPerSecond = 1000 / this.TargetElapsedTime.Milliseconds;
            int ticksPerFifth = ticksPerSecond / 5;
            backgroundTexture = new Texture2D(this.GraphicsDevice, displayWidth, ticksPerSecond);
            pixels = new uint[backgroundTexture.Width * backgroundTexture.Height];

            for (int y = 0; y < backgroundTexture.Height; y++)
                for (int x = 0; x < backgroundTexture.Width; x++)
                {
                    Color clr = Color.Black;

                    if (y == 0 || x == backgroundTexture.Width / 2 ||
                                  x == backgroundTexture.Width / 4 ||
                                  x == 3 * backgroundTexture.Width / 4)
                    {
                        clr = new Color(128, 128, 128);
                    }
                    else if (y % ticksPerFifth == 0 || 
                            ((x - backgroundTexture.Width / 2) % 
                                                (backgroundTexture.Width / 16) == 0))
                    {
                        clr = new Color(64, 64, 64);
                    }

                    pixels[y * backgroundTexture.Width + x] = clr.PackedValue;
                }
            backgroundTexture.SetData<uint>(pixels);

            // Create graph texture
            graphTexture = new Texture2D(this.GraphicsDevice, displayWidth, displayHeight);
            pixels = new uint[graphTexture.Width * graphTexture.Height];

            // Initialize
            oldInsertRow = graphTexture.Height - 2;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            Vector3 acceleration;

            lock (accelerometerVectorLock)
            {
                acceleration = accelerometerVector;
            }

            totalTicks = (int)Math.Round(gameTime.TotalGameTime.TotalSeconds / 
                                                    this.TargetElapsedTime.TotalSeconds);
            int insertRow = (totalTicks + graphTexture.Height - 1) % graphTexture.Height;

            // newInsertRow is always guaranteed to be greater than oldInsertRow,
            //  but might also be greater than the graphTexture height!
            int newInsertRow = insertRow < oldInsertRow ? insertRow + graphTexture.Height : 
                                                          insertRow;
            // Zero out pixels first
            for (int y = oldInsertRow + 1; y <= newInsertRow; y++)
                for (int x = 0; x < graphTexture.Width; x++)
                    pixels[(y % graphTexture.Height) * graphTexture.Width + x] = 0;

            // Draw three lines based on old and new acceleration values
            DrawLines(graphTexture, pixels, oldInsertRow, newInsertRow, 
                      oldAcceleration, acceleration);

            this.GraphicsDevice.Textures[0] = null;

            if (newInsertRow >= graphTexture.Height)
            {
                graphTexture.SetData<uint>(pixels);
            }
            else
            {
                Rectangle rect = new Rectangle(0, oldInsertRow, 
                                    graphTexture.Width, newInsertRow - oldInsertRow + 1);
                graphTexture.SetData<uint>(0, rect, 
                                    pixels, rect.Y * rect.Width, rect.Height * rect.Width);
            }

            oldInsertRow = insertRow;
            oldAcceleration = acceleration;

            base.Update(gameTime);
        }

        // Draw red, green, and blue lines for X, Y, and Z components
        void DrawLines(Texture2D texture, uint[] pixels, int oldRow, int newRow, 
                       Vector3 oldAcc, Vector3 newAcc)
        {
            DrawLine(texture, pixels, oldRow, newRow, oldAcc.X, newAcc.X, Color.Red);
            DrawLine(texture, pixels, oldRow, newRow, oldAcc.Y, newAcc.Y, Color.Green);
            DrawLine(texture, pixels, oldRow, newRow, oldAcc.Z, newAcc.Z, Color.Blue);
        }

        // Convert acceleration values to X 
        void DrawLine(Texture2D texture, uint[] pixels, int oldRow, int newRow, 
                      float oldAcc, float newAcc, Color clr)
        {
            DrawLine(texture, pixels, 
                     texture.Width / 2 + (int)(oldAcc * texture.Width / 4), oldRow,
                     texture.Width / 2 + (int)(newAcc * texture.Width / 4), newRow, clr);
        }

        // Simple line-drawing
        void DrawLine(Texture2D texture, uint[] pixels, 
                      int x1, int y1, int x2, int y2, Color clr)
        {
            if (x1 == x2 && y1 == y2)
            {
                return;
            }

            else if (Math.Abs(y2 - y1) > Math.Abs(x2 - x1))
            {
                int sign = Math.Sign(y2 - y1);

                for (int y = y1; y != y2; y += sign)
                {
                    float t = (float)(y - y1) / (y2 - y1);
                    int x = (int)(x1 + t * (x2 - x1) + 0.5f);
                    SetPixel(texture, pixels, x, y, clr);
                }
            }
            else
            {
                int sign = Math.Sign(x2 - x1);

                for (int x = x1; x != x2; x += sign)
                {
                    float t = (float)(x - x1) / (x2 - x1);
                    int y = (int)(y1 + t * (y2 - y1) + 0.5f);
                    SetPixel(texture, pixels, x, y, clr);
                }
            }
        }

        // Note adjustment of Y and use of bitwise OR!
        void SetPixel(Texture2D texture, uint[] pixels, int x, int y, Color clr)
        {
            pixels[(y % texture.Height) * texture.Width + x] |= clr.PackedValue;
        }

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();

            // Draw background texture
            int displayRow = -totalTicks % backgroundTexture.Height;

            while (displayRow < displayHeight)
            {
                spriteBatch.Draw(backgroundTexture, new Vector2(0, displayRow), Color.White);
                displayRow += backgroundTexture.Height;
            }

            // Draw graph texture
            displayRow = -totalTicks % graphTexture.Height;

            while (displayRow < displayHeight)
            {
                spriteBatch.Draw(graphTexture, new Vector2(0, displayRow), Color.White);
                displayRow += graphTexture.Height;
            }
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
