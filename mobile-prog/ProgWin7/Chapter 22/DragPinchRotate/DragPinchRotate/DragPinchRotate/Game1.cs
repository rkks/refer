using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Xna.Framework.Media;

namespace DragPinchRotate
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
                        Vector2 newPoint = gesture.Position;
                        Vector2 oldPoint = newPoint - gesture.Delta;

                        Vector2 textureCenter = new Vector2(texture.Width / 2, texture.Height / 2);
                        Vector2 refPoint = Vector2.Transform(textureCenter, matrix);

                        matrix *= ComputeRotateAndTranslateMatrix(refPoint, oldPoint, newPoint);
                        break;

                    case GestureType.Pinch:
                        Vector2 oldPoint1 = gesture.Position - gesture.Delta;
                        Vector2 newPoint1 = gesture.Position;
                        Vector2 oldPoint2 = gesture.Position2 - gesture.Delta2;
                        Vector2 newPoint2 = gesture.Position2;

                        matrix *= ComputeScaleAndRotateMatrix(oldPoint1, oldPoint2, newPoint2);
                        matrix *= ComputeScaleAndRotateMatrix(newPoint2, oldPoint1, newPoint1);
                        break;
                }
            }
            base.Update(gameTime);
        }

        Matrix ComputeRotateAndTranslateMatrix(Vector2 refPoint, Vector2 oldPoint, Vector2 newPoint)
        {
            Matrix matrix = Matrix.Identity;
            Vector2 delta = newPoint - oldPoint;
            Vector2 oldVector = oldPoint - refPoint;
            Vector2 newVector = newPoint - refPoint;

            // Avoid rotation if fingers are close to center
            if (newVector.Length() > 25 && oldVector.Length() > 25)
            {
                // Find angles from center of bitmap to touch points
                float oldAngle = (float)Math.Atan2(oldVector.Y, oldVector.X);
                float newAngle = (float)Math.Atan2(newVector.Y, newVector.X);

                // Calculate rotation matrix
                float angle = newAngle - oldAngle;
                matrix *= Matrix.CreateTranslation(-refPoint.X, -refPoint.Y, 0);
                matrix *= Matrix.CreateRotationZ(angle);
                matrix *= Matrix.CreateTranslation(refPoint.X, refPoint.Y, 0);

                // Essentially rotate the old vector
                oldVector = oldVector.Length() / newVector.Length() * newVector;

                // Re-calculate delta
                delta = newVector - oldVector;
            }
            // Include translation
            matrix *= Matrix.CreateTranslation(delta.X, delta.Y, 0);
            return matrix;
        }

        Matrix ComputeScaleAndRotateMatrix(Vector2 refPoint, Vector2 oldPoint, Vector2 newPoint)
        {
            Matrix matrix = Matrix.Identity;
            Vector2 oldVector = oldPoint - refPoint;
            Vector2 newVector = newPoint - refPoint;

            // Find angles from reference point to touch points
            float oldAngle = (float)Math.Atan2(oldVector.Y, oldVector.X);
            float newAngle = (float)Math.Atan2(newVector.Y, newVector.X);

            // Calculate rotation matrix
            float angle = newAngle - oldAngle;
            matrix *= Matrix.CreateTranslation(-refPoint.X, -refPoint.Y, 0);
            matrix *= Matrix.CreateRotationZ(angle);
            matrix *= Matrix.CreateTranslation(refPoint.X, refPoint.Y, 0);

            // Essentially rotate the old vector
            oldVector = oldVector.Length() / newVector.Length() * newVector;
            float scale = 1;

            // Determine scaling from dominating delta
            if (Math.Abs(newVector.X - oldVector.X) > Math.Abs(newVector.Y - oldVector.Y))
                scale = newVector.X / oldVector.X;
            else
                scale = newVector.Y / oldVector.Y;

            // Calculation scale matrix
            if (!float.IsNaN(scale) && !float.IsInfinity(scale) && scale > 0)
            {
                scale = Math.Min(1.1f, Math.Max(0.9f, scale));

                matrix *= Matrix.CreateTranslation(-refPoint.X, -refPoint.Y, 0);
                matrix *= Matrix.CreateScale(scale, scale, 1);
                matrix *= Matrix.CreateTranslation(refPoint.X, refPoint.Y, 0);
            }
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
