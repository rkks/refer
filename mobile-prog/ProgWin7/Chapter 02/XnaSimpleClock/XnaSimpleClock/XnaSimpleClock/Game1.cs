using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace XnaSimpleClock
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Viewport viewport;
        Vector2 textPosition;
        StringBuilder text = new StringBuilder();
        DateTime lastDateTime;

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
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            viewport = this.GraphicsDevice.Viewport;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Get DateTime with no milliseconds
            DateTime dateTime = DateTime.Now;
            dateTime = dateTime - new TimeSpan(0, 0, 0, 0, dateTime.Millisecond);

            if (dateTime != lastDateTime)
            {
                text.Remove(0, text.Length);
                text.Append(dateTime);
                Vector2 textSize = segoe14.MeasureString(text);
                textPosition = new Vector2((viewport.Width - textSize.X) / 2,
                                           (viewport.Height - textSize.Y) / 2);
                lastDateTime = dateTime;
            }
            else
            {
                SuppressDraw();
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, text, textPosition, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
