using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace DragAndDraw
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        struct RectangleInfo
        {
            public Vector2 point1;
            public Vector2 point2;
            public Color color;
        }

        List<RectangleInfo> rectangles = new List<RectangleInfo>();
        Random rand = new Random();
        RenderTarget2D tinyTexture;
        bool isDragging;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Enable dragging gestures
            TouchPanel.EnabledGestures = GestureType.FreeDrag | 
                                         GestureType.DragComplete;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // Create a white 1x1 bitmap
            tinyTexture = new RenderTarget2D(this.GraphicsDevice, 1, 1);
            this.GraphicsDevice.SetRenderTarget(tinyTexture);
            this.GraphicsDevice.Clear(Color.White);
            this.GraphicsDevice.SetRenderTarget(null);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                switch (gesture.GestureType)
                {
                    case GestureType.FreeDrag:
                        if (!isDragging)
                        {
                            RectangleInfo rectInfo = new RectangleInfo();
                            rectInfo.point1 = gesture.Position;
                            rectInfo.point2 = gesture.Position;
                            rectInfo.color = new Color(rand.Next(256), 
                                                       rand.Next(256), 
                                                       rand.Next(256));
                            rectangles.Add(rectInfo);
                            isDragging = true;
                        }
                        else
                        {
                            RectangleInfo rectInfo = rectangles[rectangles.Count - 1];
                            rectInfo.point2 = gesture.Position;
                            rectangles[rectangles.Count - 1] = rectInfo;
                        }
                        break;

                    case GestureType.DragComplete:
                        if (isDragging)
                            isDragging = false;
                        break;
                }
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();

            foreach (RectangleInfo rectInfo in rectangles)
            {
                Rectangle rect = 
                    new Rectangle((int)Math.Min(rectInfo.point1.X, rectInfo.point2.X),
                                  (int)Math.Min(rectInfo.point1.Y, rectInfo.point2.Y),
                                  (int)Math.Abs(rectInfo.point2.X - rectInfo.point1.X),
                                  (int)Math.Abs(rectInfo.point2.Y - rectInfo.point1.Y));

                spriteBatch.Draw(tinyTexture, rect, rectInfo.color);
            }

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
