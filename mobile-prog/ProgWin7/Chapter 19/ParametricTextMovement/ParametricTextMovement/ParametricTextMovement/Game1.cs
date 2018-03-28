using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace ParametricTextMovement
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 240f;           // pixels per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Vector2 position1;
        Vector2 pathVector;
        Vector2 textPosition;
        float lapSpeed;                     // laps per second
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
            position1 = new Vector2(viewport.Width - textSize.X, 0);
            Vector2 position2 = new Vector2(0, viewport.Height - textSize.Y);
            pathVector = position2 - position1;

            lapSpeed = SPEED / (2 * pathVector.Length());
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            tLap += lapSpeed * (float)gameTime.ElapsedGameTime.TotalSeconds;
            tLap %= 1;
            float pLap = tLap < 0.5f ? 2 * tLap : 2 - 2 * tLap;
            textPosition = position1 + pLap * pathVector;

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
