using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Phone.Shell;
using Petzold.Phone.Xna;

namespace PhingerPaint
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D canvas;
        Vector2 canvasSize;
        Vector2 canvasPosition;
        uint[] pixels;
        List<float> xCollection = new List<float>();

        Button clearButton, saveButton;
        string filename;

        List<ColorBlock> colorBlocks = new List<ColorBlock>();
        Color drawingColor = Color.Blue;
        int? touchIdToIgnore;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Set to portrait mode but leave room for status bar
            graphics.PreferredBackBufferWidth = 480;
            graphics.PreferredBackBufferHeight = 768;
        }

        protected override void Initialize()
        {
            // Create Button components
            clearButton = new Button(this, "clear");
            clearButton.Click += OnClearButtonClick;
            this.Components.Add(clearButton);

            saveButton = new Button(this, "save");
            saveButton.Click += OnSaveButtonClick;
            this.Components.Add(saveButton);

            // Create ColorBlock components
            Color[] colors = { Color.Red, Color.Green, Color.Blue, 
                               Color.Cyan, Color.Magenta, Color.Yellow,
                               Color.Black, new Color(0.2f, 0.2f, 0.2f), 
                                            new Color(0.4f, 0.4f, 0.4f), 
                                            new Color(0.6f, 0.6f, 0.6f), 
                                            new Color(0.8f, 0.8f, 0.8f), Color.White };

            foreach (Color clr in colors)
            {
                ColorBlock colorBlock = new ColorBlock(this);
                colorBlock.Color = clr;
                colorBlocks.Add(colorBlock);
                this.Components.Add(colorBlock);
            }
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Rectangle clientBounds = this.GraphicsDevice.Viewport.Bounds;
            SpriteFont segoe14 = this.Content.Load<SpriteFont>("Segoe14");

            // Set up Button components
            clearButton.SpriteFont = segoe14;
            saveButton.SpriteFont = segoe14;

            Vector2 textSize = segoe14.MeasureString(clearButton.Text);
            int buttonWidth = (int)(2 * textSize.X);
            int buttonHeight = (int)(1.5 * textSize.Y);

            clearButton.Destination =
                new Rectangle(clientBounds.Left + 20,
                              clientBounds.Bottom - 2 - buttonHeight,
                              buttonWidth, buttonHeight);

            saveButton.Destination = 
                new Rectangle(clientBounds.Right - 20 - buttonWidth,
                              clientBounds.Bottom - 2 - buttonHeight,
                              buttonWidth, buttonHeight);

            int colorBlockSize = clientBounds.Width / (colorBlocks.Count / 2) - 2;
            int xColorBlock = 2;
            int yColorBlock = 2;

            foreach (ColorBlock colorBlock in colorBlocks)
            {
                colorBlock.Destination = new Rectangle(xColorBlock, yColorBlock, 
                                                       colorBlockSize, colorBlockSize);
                xColorBlock += colorBlockSize + 2;

                if (xColorBlock + colorBlockSize > clientBounds.Width)
                {
                    xColorBlock = 2;
                    yColorBlock += colorBlockSize + 2;
                }
            }

            canvasPosition = new Vector2(0, 2 * colorBlockSize + 6);
            canvasSize = new Vector2(clientBounds.Width,
                                     clientBounds.Height - canvasPosition.Y
                                                         - buttonHeight - 4);
        }

        protected override void OnActivated(object sender, EventArgs args)
        {
            // Recover from tombstoning
            bool newlyCreated = false;
            canvas = Texture2DExtensions.LoadFromPhoneServiceState(this.GraphicsDevice, 
                                                                   "canvas");
            if (canvas == null)
            {
                // Otherwise create new Texture2D
                canvas = new Texture2D(this.GraphicsDevice, (int)canvasSize.X, 
                                                            (int)canvasSize.Y);
                newlyCreated = true;
            }

            // Create pixels array
            pixels = new uint[canvas.Width * canvas.Height];
            canvas.GetData<uint>(pixels);

            if (newlyCreated)
                ClearPixelArray();

            // Get drawing color from State, initialize selected ColorBlock
            if (PhoneApplicationService.Current.State.ContainsKey("color"))
                drawingColor = (Color)PhoneApplicationService.Current.State["color"];

            foreach (ColorBlock colorBlock in colorBlocks)
                colorBlock.IsSelected = colorBlock.Color == drawingColor;

            base.OnActivated(sender, args);
        }

        protected override void OnDeactivated(object sender, EventArgs args)
        {
            PhoneApplicationService.Current.State["color"] = drawingColor;
            canvas.SaveToPhoneServiceState("canvas");
            base.OnDeactivated(sender, args);
        }

        void ClearPixelArray()
        {
            for (int y = 0; y < canvas.Height; y++)
                for (int x = 0; x < canvas.Width; x++)
                {
                    pixels[x + canvas.Width * y] = Color.GhostWhite.PackedValue;
                }

            canvas.SetData<uint>(pixels);
        }

        void OnClearButtonClick(object sender, EventArgs e)
        {
            ClearPixelArray();
        }

        void OnSaveButtonClick(object sender, EventArgs e)
        {
            DateTime dt = DateTime.Now;
            string filename =
                String.Format("PhingerPaint-{0:D2}-{1:D2}-{2:D2}-{3:D2}-{4:D2}-{5:D2}",
                              dt.Year % 100, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);

            Guide.BeginShowKeyboardInput(PlayerIndex.One, "phinger paint save file",
                                         "enter filename:", filename, KeyboardCallback, null);
        }

        void KeyboardCallback(IAsyncResult result)
        {
            filename = Guide.EndShowKeyboardInput(result);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // If the Save File dialog box has returned, save the image
            if (!String.IsNullOrEmpty(filename))
            {
                canvas.SaveToPhotoLibrary(filename);
                filename = null;
            }

            // Process touch input
            bool canvasNeedsUpdate = false;
            TouchCollection touches = TouchPanel.GetState();

            foreach (TouchLocation touch in touches)
            {
                // Ignore further activity of ColorBlock push
                if (touchIdToIgnore.HasValue && touch.Id == touchIdToIgnore.Value)
                    continue;

                // Let Button components have first dibs on touch
                bool touchHandled = false;

                foreach (GameComponent component in this.Components)
                    if (component is IProcessTouch &&
                        (component as IProcessTouch).ProcessTouch(touch))
                    {
                        touchHandled = true;
                        break;
                    }

                if (touchHandled)
                    continue;

                // Check for tap on ColorBlock
                if (touch.State == TouchLocationState.Pressed)
                {
                    Vector2 position = touch.Position;
                    ColorBlock newSelectedColorBlock = null;

                    foreach (ColorBlock colorBlock in colorBlocks)
                    {
                        Rectangle rect = colorBlock.Destination;

                        if (position.X >= rect.Left && position.X < rect.Right &&
                            position.Y >= rect.Top && position.Y < rect.Bottom)
                        {
                            drawingColor = colorBlock.Color;
                            newSelectedColorBlock = colorBlock;
                        }
                    }

                    if (newSelectedColorBlock != null)
                    {
                        foreach (ColorBlock colorBlock in colorBlocks)
                            colorBlock.IsSelected = colorBlock == newSelectedColorBlock;

                        touchIdToIgnore = touch.Id;
                    }
                    else
                    {
                        touchIdToIgnore = null;
                    }
                }

                // Check for drawing movement
                else if (touch.State == TouchLocationState.Moved)
                {
                    TouchLocation prevTouchLocation;
                    touch.TryGetPreviousLocation(out prevTouchLocation);

                    Vector2 point1 = prevTouchLocation.Position - canvasPosition;
                    Vector2 point2 = touch.Position - canvasPosition;

                    // Sure hope touchLocation.Pressure comes back!
                    float radius = 12;
                    RoundCappedLine line = new RoundCappedLine(point1, point2, radius);

                    int yMin = (int)(Math.Min(point1.Y, point2.Y) - radius);
                    int yMax = (int)(Math.Max(point1.Y, point2.Y) + radius);

                    yMin = Math.Max(0, Math.Min(canvas.Height, yMin));
                    yMax = Math.Max(0, Math.Min(canvas.Height, yMax));

                    for (int y = yMin; y < yMax; y++)
                    {
                        xCollection.Clear();
                        line.GetAllX(y, xCollection);

                        if (xCollection.Count == 2)
                        {
                            int xMin = (int)(Math.Min(xCollection[0], xCollection[1]) + 0.5f);
                            int xMax = (int)(Math.Max(xCollection[0], xCollection[1]) + 0.5f);

                            xMin = Math.Max(0, Math.Min(canvas.Width, xMin));
                            xMax = Math.Max(0, Math.Min(canvas.Width, xMax));

                            for (int x = xMin; x < xMax; x++)
                            {
                                pixels[y * canvas.Width + x] = drawingColor.PackedValue;
                            }
                            canvasNeedsUpdate = true;
                        }
                    }
                }
            }

            if (canvasNeedsUpdate)
                canvas.SetData<uint>(pixels);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            this.GraphicsDevice.Clear(Color.Black);

            spriteBatch.Begin();
            spriteBatch.Draw(canvas, canvasPosition, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
