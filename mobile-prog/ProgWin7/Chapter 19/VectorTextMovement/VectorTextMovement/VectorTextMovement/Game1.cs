using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace VectorTextMovement
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 240f;           // pixels per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Vector2 midPoint;
        Vector2 pathVector;
        Vector2 pathDirection;
        Vector2 textPosition;

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

            Vector2 position1 = new Vector2(viewport.Width - textSize.X, 0);
            Vector2 position2 = new Vector2(0, viewport.Height - textSize.Y);
            midPoint = Vector2.Lerp(position1, position2, 0.5f);

            pathVector = position2 - position1;
            pathDirection = Vector2.Normalize(pathVector);
            textPosition = position1;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float pixelChange = SPEED * (float)gameTime.ElapsedGameTime.TotalSeconds;
            textPosition += pixelChange * pathDirection;

            if ((textPosition - midPoint).LengthSquared() > (0.5f * pathVector).LengthSquared())
            {
                float excess = (textPosition - midPoint).Length() - (0.5f * pathVector).Length();
                pathDirection = -pathDirection;
                textPosition += 2 * excess * pathDirection;
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
