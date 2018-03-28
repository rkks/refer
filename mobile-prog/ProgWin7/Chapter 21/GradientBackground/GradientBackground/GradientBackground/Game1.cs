using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace GradientBackground
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Rectangle viewportBounds;
        Texture2D background;

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

            viewportBounds = this.GraphicsDevice.Viewport.Bounds;
            background = new Texture2D(this.GraphicsDevice, viewportBounds.Width,
                                                            viewportBounds.Height);


            Color[] pixels = new Color[background.Width * background.Height];

            for (int x = 0; x < background.Width; x++)
            {
                Color clr = Color.Lerp(Color.Blue, Color.Red,
                                        (float)x / background.Width);

                for (int y = 0; y < background.Height; y++)
                    pixels[y * background.Width + x] = clr;
            }
            background.SetData<Color>(pixels);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();
            spriteBatch.Draw(background, viewportBounds, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
