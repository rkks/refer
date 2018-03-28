using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace FlickInertia
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        const float DECELERATION = 1000; // pixels per second squared

        Texture2D texture;
        Vector2 position = Vector2.Zero;
        Vector2 velocity;
        SpriteFont segoe14;
        StringBuilder text = new StringBuilder();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.Flick;
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
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Set velocity from Flick gesture
            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                if (gesture.GestureType == GestureType.Flick)
                    velocity += gesture.Delta;
            }

            // Use velocity to adjust position and decelerate
            if (velocity != Vector2.Zero)
            {
                float elapsedSeconds = (float)gameTime.ElapsedGameTime.TotalSeconds;
                position += velocity * elapsedSeconds;
                float newMagnitude = velocity.Length() - DECELERATION * elapsedSeconds;
                velocity.Normalize();
                velocity *= Math.Max(0, newMagnitude);
            }

            // Display current position and velocity
            text.Remove(0, text.Length);
            text.AppendFormat("Position: {0} Velocity: {1}", position, velocity);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin();
            spriteBatch.Draw(texture, position, Color.White);
            spriteBatch.DrawString(segoe14, text, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
