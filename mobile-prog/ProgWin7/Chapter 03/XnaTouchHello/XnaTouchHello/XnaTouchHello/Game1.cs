using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace XnaTouchHello
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Random rand = new Random();
        string text = "Hello, Windows Phone 7!";
        SpriteFont segoe36;
        Vector2 textSize;
        Vector2 textPosition;
        Color textColor = Color.White;

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

            segoe36 = this.Content.Load<SpriteFont>("Segoe36");
            textSize = segoe36.MeasureString(text);
            Viewport viewport = this.GraphicsDevice.Viewport;
            textPosition = new Vector2((viewport.Width - textSize.X) / 2,
                                       (viewport.Height - textSize.Y) / 2);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            TouchCollection touchLocations = TouchPanel.GetState();

            foreach (TouchLocation touchLocation in touchLocations)
            {
                if (touchLocation.State == TouchLocationState.Pressed)
                {
                    Vector2 touchPosition = touchLocation.Position;

                    if (touchPosition.X >= textPosition.X &&
                        touchPosition.X < textPosition.X + textSize.X &&
                        touchPosition.Y >= textPosition.Y &&
                        touchPosition.Y < textPosition.Y + textSize.Y)
                    {
                        textColor = new Color((byte)rand.Next(256),
                                              (byte)rand.Next(256),
                                              (byte)rand.Next(256));
                    }
                    else
                    {
                        textColor = Color.White;
                    }
                }
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            this.GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe36, text, textPosition, textColor);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
