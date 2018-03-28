using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace DragAndPinch
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D texture;
        Matrix matrix = Matrix.Identity;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.FreeDrag | GestureType.Pinch;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            texture = this.Content.Load<Texture2D>("PetzoldTattoo");
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
                        matrix *= Matrix.CreateTranslation(gesture.Delta.X, gesture.Delta.Y, 0);
                        break;

                    case GestureType.Pinch:
                        Vector2 oldPoint1 = gesture.Position - gesture.Delta;
                        Vector2 newPoint1 = gesture.Position;
                        Vector2 oldPoint2 = gesture.Position2 - gesture.Delta2;
                        Vector2 newPoint2 = gesture.Position2;

                        matrix *= ComputeScaleMatrix(oldPoint1, oldPoint2, newPoint2);
                        matrix *= ComputeScaleMatrix(newPoint2, oldPoint1, newPoint1);
                        break;
                }
            }
            base.Update(gameTime);
        }

        Matrix ComputeScaleMatrix(Vector2 refPoint, Vector2 oldPoint, Vector2 newPoint)
        {
            float scaleX = (newPoint.X - refPoint.X) / (oldPoint.X - refPoint.X);
            float scaleY = (newPoint.Y - refPoint.Y) / (oldPoint.Y - refPoint.Y);

            if (float.IsNaN(scaleX) || float.IsInfinity(scaleX) ||
                float.IsNaN(scaleY) || float.IsInfinity(scaleY) ||
                scaleX <= 0 || scaleY <= 0)
            {
                return Matrix.Identity;
            }

            scaleX = Math.Min(1.1f, Math.Max(0.9f, scaleX));
            scaleY = Math.Min(1.1f, Math.Max(0.9f, scaleY));

            Matrix matrix = Matrix.CreateTranslation(-refPoint.X, -refPoint.Y, 0);
            matrix *= Matrix.CreateScale(scaleX, scaleY, 1);
            matrix *= Matrix.CreateTranslation(refPoint.X, refPoint.Y, 0);

            return matrix;
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin(SpriteSortMode.Deferred, null, null, null, null, null, matrix);
            spriteBatch.Draw(texture, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
