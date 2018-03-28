using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace TouchToStopRevolution
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float ACCELERATION = 1;       // revs per second squared
        const float MAXSPEED = 30;          // revs per second
        const string TEXT = "Hello, Windows Phone 7!";

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        Vector2 textPosition;
        Vector2 origin;
        Vector2 statusPosition;
        float speed;
        float angle;
        StringBuilder strBuilder = new StringBuilder();

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
            textPosition = new Vector2(viewport.Width / 2, viewport.Height / 2);

            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            Vector2 textSize = segoe14.MeasureString(TEXT);
            origin = new Vector2(textSize.X / 2, textSize.Y / 2);
            statusPosition = new Vector2(viewport.Width - textSize.X, 
                                         viewport.Height - textSize.Y);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            if (TouchPanel.GetState().Count == 0)
            {
                speed += ACCELERATION * (float)gameTime.ElapsedGameTime.TotalSeconds;
                speed = Math.Min(MAXSPEED, speed);
                angle += MathHelper.TwoPi * speed * (float)gameTime.ElapsedGameTime.TotalSeconds;
                angle %= MathHelper.TwoPi;
            }
            else
            {
                if (speed == 0)
                    SuppressDraw();

                speed = 0;
            }
            strBuilder.Remove(0, strBuilder.Length);
            strBuilder.AppendFormat(" {0:F1} revolutions/second", speed);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, strBuilder, statusPosition, Color.White);
            spriteBatch.DrawString(segoe14, TEXT, textPosition, Color.White,
                                   angle, origin, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
