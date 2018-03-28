using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Petzold.Phone.Xna;

namespace SpinPaint
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        // Fields involved with spinning disk texture
        Texture2D diskTexture;
        uint[] pixels;
        Vector2 displayCenter;
        Vector2 textureCenter;
        int radius;
        Color currentColor;

        // Touch information and line-drawing fields
        class TouchInfo
        {
            public Vector2 PreviousPosition;
            public Vector2 CurrentPosition;
        }
        Dictionary<int, TouchInfo> touchDictionary = new Dictionary<int, TouchInfo>();
        float currentAngle;
        float previousAngle;
        List<float> xCollection = new List<float>();

        // Buttons and titles
        Button clearButton, saveButton;
        SpriteFont segoe14;
        SpriteFont segoe48;
        string titleText = "spin paint";
        Vector2 titlePosition;
        string filename;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Portrait, but allow room for status bar at top
            graphics.PreferredBackBufferWidth = 480;
            graphics.PreferredBackBufferHeight = 768;
        }

        protected override void Initialize()
        {
            // Create button components
            clearButton = new Button(this, "clear");
            clearButton.Click += OnClearButtonClick;
            this.Components.Add(clearButton);

            saveButton = new Button(this, "save");
            saveButton.Click += OnSaveButtonClick;
            this.Components.Add(saveButton);

            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // Get display information
            Rectangle clientBounds = this.GraphicsDevice.Viewport.Bounds;
            displayCenter = new Vector2(clientBounds.Center.X, clientBounds.Center.Y);

            // Load fonts and calculate title position
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            segoe48 = this.Content.Load<SpriteFont>("Segoe48");
            titlePosition = new Vector2((int)((clientBounds.Width - 
                                        segoe48.MeasureString(titleText).X) / 2), 20);

            // Set button fonts and destinations
            clearButton.SpriteFont = segoe14;
            saveButton.SpriteFont = segoe14;
            Vector2 textSize = segoe14.MeasureString(clearButton.Text);
            int buttonWidth = (int)(2 * textSize.X);
            int buttonHeight = (int)(1.5 * textSize.Y);

            clearButton.Destination = 
                new Rectangle(clientBounds.Left + 20, 
                              clientBounds.Bottom - 20 - buttonHeight, 
                              buttonWidth, buttonHeight);
            saveButton.Destination = 
                new Rectangle(clientBounds.Right - 20 - buttonWidth, 
                              clientBounds.Bottom - 20 - buttonHeight, 
                              buttonWidth, buttonHeight);
        }

        protected override void OnActivated(object sender, EventArgs args)
        {
            // Recover from tombstoning
            bool newlyCreated = false;
            diskTexture = Texture2DExtensions.LoadFromPhoneServiceState(this.GraphicsDevice,
                                                                        "disk");
            // Or create the Texture2D
            if (diskTexture == null)
            {
                Rectangle clientBounds = this.GraphicsDevice.Viewport.Bounds;
                int textureDimension = Math.Min(clientBounds.Width, clientBounds.Height);
                diskTexture = new Texture2D(this.GraphicsDevice, textureDimension, 
                                                                 textureDimension);
                newlyCreated = true;
            }

            pixels = new uint[diskTexture.Width * diskTexture.Height];
            radius = diskTexture.Width / 2;
            textureCenter = new Vector2(radius, radius);

            if (newlyCreated)
            {
                ClearPixelArray();
            }
            else
            {
                diskTexture.GetData<uint>(pixels);
            }

            base.OnActivated(sender, args);
        }

        protected override void OnDeactivated(object sender, EventArgs args)
        {
            diskTexture.SaveToPhoneServiceState("disk");
            base.OnDeactivated(sender, args);
        }

        // Clear the disk texture
        void ClearPixelArray()
        {
            for (int y = 0; y < diskTexture.Height; y++)
                for (int x = 0; x < diskTexture.Width; x++)
                    if (IsWithinCircle(x, y))
                    {
                        Color clr = Color.White;

                        // Lines that criss cross quadrants
                        if (x == diskTexture.Width / 2 || y == diskTexture.Height / 2)
                            clr = Color.LightGray;

                        pixels[y * diskTexture.Width + x] = clr.PackedValue;
                    }
            diskTexture.SetData<uint>(pixels);
        }

        bool IsWithinCircle(int x, int y)
        {
            x -= diskTexture.Width / 2;
            y -= diskTexture.Height / 2;

            return x * x + y * y < radius * radius;
        }

        void OnClearButtonClick(object sender, EventArgs args)
        {
            ClearPixelArray();
        }

        void OnSaveButtonClick(object sender, EventArgs args)
        {
            DateTime dt = DateTime.Now;
            string filename = 
                String.Format("spinpaint-{0:D2}-{1:D2}-{2:D2}-{3:D2}-{4:D2}-{5:D2}", 
                              dt.Year % 100, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second);

            Guide.BeginShowKeyboardInput(PlayerIndex.One, "spin paint save file", 
                                         "enter filename:", filename, KeyboardCallback, null);
        }

        void KeyboardCallback(IAsyncResult result)
        {
            filename = Guide.EndShowKeyboardInput(result);
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // If the Save File dialog has returned, save the image
            if (!String.IsNullOrEmpty(filename))
            {
                diskTexture.SaveToPhotoLibrary(filename);
                filename = null;
            }

            // Disk rotates every 5 seconds
            double seconds = gameTime.TotalGameTime.TotalSeconds;
            currentAngle = (float)(2 * Math.PI * seconds / 5);

            // Colors cycle every 10 seconds
            float fraction = (float)(6 * (seconds % 10) / 10);
            
            if (fraction < 1)
                currentColor = new Color(1, fraction, 0);
            else if (fraction < 2)
                currentColor = new Color(2 - fraction, 1, 0);
            else if (fraction < 3)
                currentColor = new Color(0, 1, fraction - 2);
            else if (fraction < 4)
                currentColor = new Color(0, 4 - fraction, 1);
            else if (fraction < 5)
                currentColor = new Color(fraction - 4, 0, 1);
            else
                currentColor = new Color(1, 0, 6 - fraction);

            // First assume no finger movement
            foreach (TouchInfo touchInfo in touchDictionary.Values)
                touchInfo.CurrentPosition = touchInfo.PreviousPosition;

            // Get all touches
            TouchCollection touches = TouchPanel.GetState();

            foreach (TouchLocation touch in touches)
            {
                // Let Button components have first dibs on touch
                bool touchHandled = false;

                foreach (GameComponent component in this.Components)
                {
                    if (component is IProcessTouch &&
                        (component as IProcessTouch).ProcessTouch(touch))
                    {
                        touchHandled = true;
                        break;
                    }
                }

                if (touchHandled)
                    continue;

                // Set TouchInfo items from touch information
                int id = touch.Id;

                switch (touch.State)
                {
                    case TouchLocationState.Pressed:
                        if (!touchDictionary.ContainsKey(id))
                            touchDictionary.Add(id, new TouchInfo());

                        touchDictionary[id].PreviousPosition = TranslateToTexture(touch.Position);
                        touchDictionary[id].CurrentPosition = TranslateToTexture(touch.Position);
                        break;

                    case TouchLocationState.Moved:
                        if (touchDictionary.ContainsKey(id))
                            touchDictionary[id].CurrentPosition = 
                                        TranslateToTexture(touch.Position);
                        break;

                    case TouchLocationState.Released:
                        if (touchDictionary.ContainsKey(id))
                            touchDictionary.Remove(id);
                        break;
                }
            }

            // Calculate transforms for rotation
            Matrix translate1 = Matrix.CreateTranslation(-textureCenter.X, -textureCenter.Y, 0);
            Matrix translate2 = Matrix.CreateTranslation(textureCenter.X, textureCenter.Y, 0);

            Matrix previousRotation = translate1 * 
                                            Matrix.CreateRotationZ(-previousAngle) * 
                                                    translate2;
            Matrix currentRotation = translate1 * 
                                            Matrix.CreateRotationZ(-currentAngle) * 
                                                    translate2;

            bool textureNeedsUpdate = false;

            foreach (TouchInfo touchInfo in touchDictionary.Values)
            {
                // Now draw from previous to current points
                Vector2 point1 = Vector2.Transform(touchInfo.PreviousPosition, previousRotation);
                Vector2 point2 = Vector2.Transform(touchInfo.CurrentPosition, currentRotation);
                float radius = 6;

                RoundCappedLine line = new RoundCappedLine(point1, point2, radius);

                int yMin = (int)(Math.Min(point1.Y, point2.Y) - radius);
                int yMax = (int)(Math.Max(point1.Y, point2.Y) + radius);

                yMin = Math.Max(0, Math.Min(diskTexture.Height, yMin));
                yMax = Math.Max(0, Math.Min(diskTexture.Height, yMax));

                for (int y = yMin; y < yMax; y++)
                {
                    xCollection.Clear();
                    line.GetAllX(y, xCollection);

                    if (xCollection.Count == 2)
                    {
                        int xMin = (int)(Math.Min(xCollection[0], xCollection[1]) + 0.5f);
                        int xMax = (int)(Math.Max(xCollection[0], xCollection[1]) + 0.5f);

                        xMin = Math.Max(0, Math.Min(diskTexture.Width, xMin));
                        xMax = Math.Max(0, Math.Min(diskTexture.Width, xMax));

                        for (int x = xMin; x < xMax; x++)
                        {
                            if (IsWithinCircle(x, y))
                            {
                                // Draw pixel in four quadrants
                                int xFlip = diskTexture.Width - x;
                                int yFlip = diskTexture.Height - y;

                                pixels[y * diskTexture.Width + x] = currentColor.PackedValue;
                                pixels[y * diskTexture.Width + xFlip] = currentColor.PackedValue;
                                pixels[yFlip * diskTexture.Width + x] = currentColor.PackedValue;
                                pixels[yFlip * diskTexture.Width + xFlip] = 
                                                                    currentColor.PackedValue;
                            }
                        }
                        textureNeedsUpdate = true;
                    }
                }
            }

            if (textureNeedsUpdate)
            {
                // Update the texture from the pixels array
                this.GraphicsDevice.Textures[0] = null;
                diskTexture.SetData<uint>(pixels);
            }

            // Prepare for next time through
            foreach (TouchInfo touchInfo in touchDictionary.Values)
                touchInfo.PreviousPosition = touchInfo.CurrentPosition;
            
            previousAngle = currentAngle;

            base.Update(gameTime);
        }

        Vector2 TranslateToTexture(Vector2 point)
        {
            return point - displayCenter + textureCenter;
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(diskTexture, displayCenter, null, Color.White, 
                             currentAngle, textureCenter, 1, SpriteEffects.None, 0);
            spriteBatch.DrawString(segoe48, titleText, titlePosition, currentColor);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
