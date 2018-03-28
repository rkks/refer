using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace OneFingerRotation
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D texture;
        Vector2 texturePosition;
        Vector2 textureCenter;
        float textureRotation;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.FreeDrag;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Viewport viewport = this.GraphicsDevice.Viewport;
            texturePosition = new Vector2(viewport.Width / 2, viewport.Height / 2);

            texture = this.Content.Load<Texture2D>("PetzoldTattoo");
            textureCenter = new Vector2(texture.Width / 2, texture.Height / 2);
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

                if (gesture.GestureType == GestureType.FreeDrag)
                {
                    Vector2 delta = gesture.Delta;
                    Vector2 newPosition = gesture.Position;
                    Vector2 oldPosition = newPosition - delta;

                    // Find vectors from center of bitmap to touch points
                    Vector2 oldVector = oldPosition - texturePosition;
                    Vector2 newVector = newPosition - texturePosition;

                    // Avoid rotation if fingers are close to center
                    if (newVector.Length() > 25 && oldVector.Length() > 25)
                    {
                        // Find angles from center of bitmap to touch points
                        float oldAngle = (float)Math.Atan2(oldVector.Y, oldVector.X);
                        float newAngle = (float)Math.Atan2(newVector.Y, newVector.X);

                        // Adjust texture rotation angle
                        textureRotation += newAngle - oldAngle;

                        // Essentially rotate the old vector
                        oldVector = oldVector.Length() / newVector.Length() * newVector;

                        // Re-calculate delta
                        delta = newVector - oldVector;
                    }
                    // Move texture
                    texturePosition += delta;
                }
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin();
            spriteBatch.Draw(texture, texturePosition, null, Color.White, 
                             textureRotation, textureCenter, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
