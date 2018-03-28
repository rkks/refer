using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace ScaleTextToViewport
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 0.5f;           // laps per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Vector2 textPosition;
        Vector2 origin;
        Vector2 maxScale;
        Vector2 scale;
        float tLap;

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
            Viewport viewport = this.GraphicsDevice.Viewport;

            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            Vector2 textSize = segoe14.MeasureString(TEXT);
            textPosition = new Vector2(viewport.Width / 2, viewport.Height / 2);
            origin = new Vector2(textSize.X / 2, textSize.Y / 2);
            maxScale = new Vector2(viewport.Width / textSize.X, viewport.Height / textSize.Y);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            tLap = (SPEED * (float)gameTime.TotalGameTime.TotalSeconds) % 1;
            float pLap = (1 - (float)Math.Cos(tLap * MathHelper.TwoPi)) / 2;
            scale = Vector2.Lerp(Vector2.One, maxScale, pLap);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, TEXT, textPosition, Color.White,
                                   0, origin, scale, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
