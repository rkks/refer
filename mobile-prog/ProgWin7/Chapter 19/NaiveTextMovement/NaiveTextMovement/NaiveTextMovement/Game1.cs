using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace NaiveTextMovement
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 240f;           // pixels per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Viewport viewport;
        Vector2 textSize;
        Vector2 textPosition;
        bool isGoingUp = false;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            viewport = this.GraphicsDevice.Viewport;
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            textSize = segoe14.MeasureString(TEXT);
            textPosition = new Vector2(viewport.X + (viewport.Width - textSize.X) / 2, 0);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            if (!isGoingUp)
            {
                textPosition.Y += SPEED * (float)gameTime.ElapsedGameTime.TotalSeconds;
                if (textPosition.Y + textSize.Y > viewport.Height)
                {
                    float excess = textPosition.Y + textSize.Y - viewport.Height;
                    textPosition.Y -= 2 * excess;
                    isGoingUp = true;
                }
            }
            else
            {
                textPosition.Y -= SPEED * (float)gameTime.ElapsedGameTime.TotalSeconds;

                if (textPosition.Y < 0)
                {
                    float excess = - textPosition.Y;
                    textPosition.Y += 2 * excess;
                    isGoingUp = false;
                }
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, TEXT, textPosition, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
