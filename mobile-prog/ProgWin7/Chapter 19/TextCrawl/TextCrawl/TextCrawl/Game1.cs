using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace TextCrawl
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 0.1f;           // laps per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Viewport viewport;
        Vector2 textSize;
        Vector2 textPosition;
        float tCorner;          // height / perimeter
        float tLap;
        float angle;

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
            tCorner = 0.5f * viewport.Height / (viewport.Width + viewport.Height);
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            textSize = segoe14.MeasureString(TEXT);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            tLap = (tLap + SPEED * (float)gameTime.ElapsedGameTime.TotalSeconds) % 1;

            if (tLap < tCorner)             // down left side of screen
            {
                textPosition.X = 0;
                textPosition.Y = (tLap / tCorner) * viewport.Height;
                angle = -MathHelper.PiOver2;

                if (textPosition.Y < textSize.X)
                    angle += (float)Math.Acos(textPosition.Y / textSize.X);
            }
            else if (tLap < 0.5f)           // across bottom of screen
            {
                textPosition.X = ((tLap - tCorner) / (0.5f - tCorner)) * viewport.Width;
                textPosition.Y = viewport.Height;
                angle = MathHelper.Pi;

                if (textPosition.X < textSize.X)
                    angle += (float)Math.Acos(textPosition.X / textSize.X);
            }
            else if (tLap < 0.5f + tCorner) // up right side of screen
            {
                textPosition.X = viewport.Width;
                textPosition.Y = (1 - (tLap - 0.5f) / tCorner) * viewport.Height;
                angle = MathHelper.PiOver2;

                if (textPosition.Y + textSize.X > viewport.Height)
                    angle += (float)Math.Acos((viewport.Height - textPosition.Y) / textSize.X);
            }
            else                            // across top of screen
            {
                textPosition.X = (1 - (tLap - 0.5f - tCorner) / (0.5f - tCorner)) * viewport.Width;
                textPosition.Y = 0;
                angle = 0;

                if (textPosition.X + textSize.X > viewport.Width)
                    angle += (float)Math.Acos((viewport.Width - textPosition.X) / textSize.X);
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue); GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, TEXT, textPosition, Color.White,
                                   angle, Vector2.Zero, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
