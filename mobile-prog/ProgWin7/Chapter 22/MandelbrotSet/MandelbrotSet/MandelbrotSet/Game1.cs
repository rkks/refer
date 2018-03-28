using System;
using System.IO;
using System.Text;
using System.Threading;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Phone.Shell;
using Petzold.Phone.Xna;

namespace MandelbrotSet
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Viewport viewport;
        Texture2D texture;
        uint[] pixels;
        PixelInfo[] pixelInfos;
        Matrix drawMatrix = Matrix.Identity;
        int globalIteration = 0;
        object pixelInfosLock = new object();

        SpriteFont segoe14;
        StringBuilder upperLeftCoordText = new StringBuilder();
        StringBuilder lowerRightCoordText = new StringBuilder();
        StringBuilder upperRightStatusText = new StringBuilder();
        Vector2 lowerRightCoordPosition, upperRightStatusPosition;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Set full screen & enable gestures
            graphics.IsFullScreen = true;
            TouchPanel.EnabledGestures = GestureType.FreeDrag | GestureType.DragComplete |
                                         GestureType.Pinch | GestureType.PinchComplete;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            viewport = this.GraphicsDevice.Viewport;
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
        }

        protected override void OnActivated(object sender, EventArgs args)
        {
            PhoneApplicationService appService = PhoneApplicationService.Current;

            if (appService.State.ContainsKey("xOrigin") &&
                appService.State.ContainsKey("yOrigin") &&
                appService.State.ContainsKey("resolution"))
            {
                PixelInfo.xPixelCoordAtComplexOrigin = (double)appService.State["xOrigin"];
                PixelInfo.yPixelCoordAtComplexOrigin = (double)appService.State["yOrigin"];
                PixelInfo.unitsPerPixel = (double)appService.State["resolution"];
            }
            else
            {
                // Program running from beginning
                PixelInfo.xPixelCoordAtComplexOrigin = 2 * viewport.Width / 3f;
                PixelInfo.yPixelCoordAtComplexOrigin = viewport.Height / 2;
                PixelInfo.unitsPerPixel = Math.Max(2.5 / viewport.Height, 
                                                   3.0 / viewport.Width);
            }

            UpdateCoordinateText();

            // Restore bitmap from tombstoning or recreate it
            texture = Texture2DExtensions.LoadFromPhoneServiceState(this.GraphicsDevice, 
                                                                    "mandelbrotBitmap");
            if (texture == null)
                texture = new Texture2D(this.GraphicsDevice, viewport.Width, viewport.Height);

            // Get texture information and pixels array
            PixelInfo.pixelWidth = texture.Width;
            PixelInfo.pixelHeight = texture.Height;
            int numPixels = PixelInfo.pixelWidth * PixelInfo.pixelHeight;
            pixels = new uint[numPixels];
            texture.GetData<uint>(pixels);

            // Create and initialize PixelInfo array
            pixelInfos = new PixelInfo[numPixels];
            InitializePixelInfo(pixels);

            // Start up the calculation thread
            Thread thread = new Thread(PixelSetterThread);
            thread.Start();

            base.OnActivated(sender, args);
        }

        protected override void OnDeactivated(object sender, EventArgs args)
        {
            PhoneApplicationService.Current.State["xOrigin"] = PixelInfo.xPixelCoordAtComplexOrigin;
            PhoneApplicationService.Current.State["yOrigin"] = PixelInfo.yPixelCoordAtComplexOrigin;
            PhoneApplicationService.Current.State["resolution"] = PixelInfo.unitsPerPixel;

            texture.SaveToPhoneServiceState("mandelbrotBitmap");

            base.OnDeactivated(sender, args);
        }

        void InitializePixelInfo(uint[] pixels)
        {
            for (int index = 0; index < pixelInfos.Length; index++)
            {
                pixelInfos[index] = new PixelInfo(index, pixels);
            }

            PixelInfo.hasNewColors = true;
            PixelInfo.firstNewIndex = 0;
            PixelInfo.lastNewIndex = pixelInfos.Length - 1;
        }

        void PixelSetterThread()
        {
            int pixelIndex = 0;

            while (true)
            {
                lock (pixelInfosLock)
                {
                    if (!pixelInfos[pixelIndex].finished)
                    {
                        if (pixelInfos[pixelIndex].Iterate())
                        {
                            int iteration = pixelInfos[pixelIndex].iteration;
                            pixelInfos[pixelIndex].packedColor = 
                                                    GetPixelColor(iteration).PackedValue;
 
                            PixelInfo.hasNewColors = true;
                            PixelInfo.firstNewIndex = Math.Min(PixelInfo.firstNewIndex, pixelIndex);
                            PixelInfo.lastNewIndex = Math.Max(PixelInfo.lastNewIndex, pixelIndex);
                        }
                        else
                        {
                            // Special case: On scale up, prevent blocks of color from 
                            //      remaining inside the Mandelbrot Set
                            if (pixelInfos[pixelIndex].iteration == 500 && 
                                pixelInfos[pixelIndex].packedColor != Color.Black.PackedValue)
                            {
                                pixelInfos[pixelIndex].packedColor = Color.Black.PackedValue;

                                PixelInfo.hasNewColors = true;
                                PixelInfo.firstNewIndex = 
                                                Math.Min(PixelInfo.firstNewIndex, pixelIndex);
                                PixelInfo.lastNewIndex = 
                                                Math.Max(PixelInfo.lastNewIndex, pixelIndex);
                            }
                        }
                    }

                    if (++pixelIndex == pixelInfos.Length)
                    {
                        pixelIndex = 0;
                        globalIteration++;
                    }
                }
            }
        }

        Color GetPixelColor(int iteration)
        {
            float proportion = (iteration / 32f) % 1;

            if (proportion < 0.5)
                return new Color(1 - 2 * proportion, 0, 2 * proportion);

            proportion = 2 * (proportion - 0.5f);

            return new Color(0, proportion, 1 - proportion);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Update texture from pixels array from pixelInfos array
            if (PixelInfo.hasNewColors)
            {
                lock (pixelInfosLock)
                {
                    // Transfer new colors to pixels array
                    for (int pixelIndex = PixelInfo.firstNewIndex;
                             pixelIndex <= PixelInfo.lastNewIndex;
                             pixelIndex++)
                    {
                        pixels[pixelIndex] = pixelInfos[pixelIndex].packedColor;
                    }

                    // Transfer new pixels to texture
                    int firstRow = PixelInfo.firstNewIndex / texture.Width;
                    int numRows = PixelInfo.lastNewIndex / texture.Width - firstRow + 1;
                    Rectangle rect = new Rectangle(0, firstRow, texture.Width, numRows);
                    texture.SetData<uint>(0, rect, pixels, firstRow * texture.Width, 
                                                           numRows * texture.Width);

                    // Reset PixelInfo 
                    PixelInfo.hasNewColors = false;
                    PixelInfo.firstNewIndex = Int32.MaxValue;
                    PixelInfo.lastNewIndex = 0;
                }
            }

            // Update globalIteration display
            upperRightStatusText.Remove(0, upperRightStatusText.Length);
            upperRightStatusText.AppendFormat("{0}", globalIteration + 1);
            Vector2 textSize = segoe14.MeasureString(upperRightStatusText);
            upperRightStatusPosition = new Vector2(viewport.Width - textSize.X, 0);

            // Read touch gestures
            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                switch (gesture.GestureType)
                {
                    case GestureType.FreeDrag:
                        // Adjust drawMatrix for shifting
                        drawMatrix.M41 += gesture.Delta.X;
                        drawMatrix.M42 += gesture.Delta.Y;
                        break;

                    case GestureType.DragComplete:
                        // Update texture from pixels from shifted pixelInfos
                        lock (pixelInfosLock)
                        {
                            pixelInfos = TranslatePixelInfo(pixelInfos, drawMatrix);

                            for (int pixelIndex = 0; pixelIndex < pixelInfos.Length; pixelIndex++)
                                pixels[pixelIndex] = pixelInfos[pixelIndex].packedColor;

                            PixelInfo.hasNewColors = false;
                            PixelInfo.firstNewIndex = Int32.MaxValue;
                            PixelInfo.lastNewIndex = 0;
                        }
                        texture.SetData<uint>(pixels);

                        drawMatrix = Matrix.Identity;
                        globalIteration = 0;
                        break;

                    case GestureType.Pinch:
                        bool xDominates = Math.Abs(gesture.Delta.X) + Math.Abs(gesture.Delta2.X) > 
                                          Math.Abs(gesture.Delta.Y) + Math.Abs(gesture.Delta2.Y);

                        Vector2 oldPoint1 = gesture.Position - gesture.Delta;
                        Vector2 newPoint1 = gesture.Position;
                        Vector2 oldPoint2 = gesture.Position2 - gesture.Delta2;
                        Vector2 newPoint2 = gesture.Position2;

                        drawMatrix *= ComputeScaleMatrix(oldPoint1, oldPoint2, newPoint2, xDominates);
                        drawMatrix *= ComputeScaleMatrix(newPoint2, oldPoint1, newPoint1, xDominates);
                        break;

                    case GestureType.PinchComplete:
                        // Set texture from zoomed pixels
                        pixels = ZoomPixels(pixels, drawMatrix);
                        texture.SetData<uint>(pixels);

                        // Set new PixelInfo parameters
                        PixelInfo.xPixelCoordAtComplexOrigin *= drawMatrix.M11;
                        PixelInfo.xPixelCoordAtComplexOrigin += drawMatrix.M41;
                        PixelInfo.yPixelCoordAtComplexOrigin *= drawMatrix.M22;
                        PixelInfo.yPixelCoordAtComplexOrigin += drawMatrix.M42;
                        PixelInfo.unitsPerPixel /= drawMatrix.M11;

                        // Reinitialize PpixelInfos
                        lock (pixelInfosLock)
                        {
                            InitializePixelInfo(pixels);
                        }

                        drawMatrix = Matrix.Identity;
                        globalIteration = 0;
                        break;
                }
                UpdateCoordinateText();
            }
            base.Update(gameTime);
        }

        PixelInfo[] TranslatePixelInfo(PixelInfo[] srcPixelInfos, Matrix drawMatrix)
        {
            int x = (int)(drawMatrix.M41 + 0.5);
            int y = (int)(drawMatrix.M42 + 0.5);
            PixelInfo.xPixelCoordAtComplexOrigin += x;
            PixelInfo.yPixelCoordAtComplexOrigin += y;
            PixelInfo[] dstPixelInfos = new PixelInfo[srcPixelInfos.Length];

            for (int dstY = 0; dstY < PixelInfo.pixelHeight; dstY++)
            {
                int srcY = dstY - y;
                int srcRow = srcY * PixelInfo.pixelWidth;
                int dstRow = dstY * PixelInfo.pixelWidth;

                for (int dstX = 0; dstX < PixelInfo.pixelWidth; dstX++)
                {
                    int srcX = dstX - x;
                    int dstIndex = dstRow + dstX;

                    if (srcX >= 0 && srcX < PixelInfo.pixelWidth &&
                        srcY >= 0 && srcY < PixelInfo.pixelHeight)
                    {
                        int srcIndex = srcRow + srcX;
                        dstPixelInfos[dstIndex] = pixelInfos[srcIndex];
                    }
                    else
                    {
                        dstPixelInfos[dstIndex] = new PixelInfo(dstIndex, null);
                    }
                }
            }
            return dstPixelInfos;
        }

        Matrix ComputeScaleMatrix(Vector2 refPoint, Vector2 oldPoint, Vector2 newPoint, 
                                  bool xDominates)
        {
            float scale = 1;

            if (xDominates)
                scale = (newPoint.X - refPoint.X) / (oldPoint.X - refPoint.X);
            else
                scale = (newPoint.Y - refPoint.Y) / (oldPoint.Y - refPoint.Y);

            if (float.IsNaN(scale) || float.IsInfinity(scale) || scale < 0)
            {
                return Matrix.Identity;
            }

            scale = Math.Min(1.1f, Math.Max(0.9f, scale));

            Matrix matrix = Matrix.CreateTranslation(-refPoint.X, -refPoint.Y, 0);
            matrix *= Matrix.CreateScale(scale, scale, 1);
            matrix *= Matrix.CreateTranslation(refPoint.X, refPoint.Y, 0);

            return matrix;
        }

        uint[] ZoomPixels(uint[] srcPixels, Matrix matrix)
        {
            Matrix invMatrix = Matrix.Invert(matrix);
            uint[] dstPixels = new uint[srcPixels.Length];

            for (int dstY = 0; dstY < PixelInfo.pixelHeight; dstY++)
            {
                int dstRow = dstY * PixelInfo.pixelWidth;

                for (int dstX = 0; dstX < PixelInfo.pixelWidth; dstX++)
                {
                    int dstIndex = dstRow + dstX;
                    Vector2 dst = new Vector2(dstX, dstY);
                    Vector2 src = Vector2.Transform(dst, invMatrix);
                    int srcX = (int)(src.X + 0.5f);
                    int srcY = (int)(src.Y + 0.5f);

                    if (srcX >= 0 && srcX < PixelInfo.pixelWidth &&
                        srcY >= 0 && srcY < PixelInfo.pixelHeight)
                    {
                        int srcIndex = srcY * PixelInfo.pixelWidth + srcX;
                        dstPixels[dstIndex] = srcPixels[srcIndex];
                    }
                    else
                    {
                        dstPixels[dstIndex] = Color.Black.PackedValue;
                    }
                }
            }
            return dstPixels;
        }

        void UpdateCoordinateText()
        {
            double xAdjustedPixelCoord = 
                PixelInfo.xPixelCoordAtComplexOrigin * drawMatrix.M11 + drawMatrix.M41;
            double yAdjustedPixelCoord =
                PixelInfo.yPixelCoordAtComplexOrigin * drawMatrix.M22 + drawMatrix.M42;
            double adjustedUnitsPerPixel = PixelInfo.unitsPerPixel / drawMatrix.M11;

            double xUpperLeft = -adjustedUnitsPerPixel * xAdjustedPixelCoord;
            double yUpperLeft = adjustedUnitsPerPixel * yAdjustedPixelCoord;

            upperLeftCoordText.Remove(0, upperLeftCoordText.Length);
            upperLeftCoordText.AppendFormat("X:{0} Y:{1}", xUpperLeft, yUpperLeft);

            double xLowerRight = xUpperLeft + PixelInfo.pixelWidth * adjustedUnitsPerPixel;
            double yLowerRight = -yUpperLeft + PixelInfo.pixelHeight * adjustedUnitsPerPixel;

            lowerRightCoordText.Remove(0, lowerRightCoordText.Length);
            lowerRightCoordText.AppendFormat("X:{0} Y:{1}", xLowerRight, yLowerRight);

            Vector2 textSize = segoe14.MeasureString(lowerRightCoordText);
            lowerRightCoordPosition = new Vector2(viewport.Width - textSize.X, 
                                                  viewport.Height - textSize.Y);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Black);

            // Draw Mandelbrot Set image
            spriteBatch.Begin(SpriteSortMode.Immediate, null, null, null, null, null, drawMatrix);
            spriteBatch.Draw(texture, Vector2.Zero, null, Color.White,
                             0, Vector2.Zero, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            // Draw coordinate and status text
            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, upperLeftCoordText, Vector2.Zero, Color.White);
            spriteBatch.DrawString(segoe14, lowerRightCoordText, lowerRightCoordPosition, Color.White);
            spriteBatch.DrawString(segoe14, upperRightStatusText, upperRightStatusPosition, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
