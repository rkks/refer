using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace XnaOrientableHelloPhone
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        string text = "Hello, Windows Phone 7!";
        SpriteFont segoe14;
        Vector2 textSize;
        Vector2 textPosition;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Allow portrait mode as well
            graphics.SupportedOrientations = DisplayOrientation.Portrait |
                                             DisplayOrientation.LandscapeLeft |
                                             DisplayOrientation.LandscapeRight;

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
            textSize = segoe14.MeasureString(text);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            Viewport viewport = this.GraphicsDevice.Viewport;
            textPosition = new Vector2((viewport.Width - textSize.X) / 2,
                                       (viewport.Height - textSize.Y) / 2);
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
