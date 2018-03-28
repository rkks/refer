using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input.Touch;

namespace Petzold.Phone.Xna
{
    public class Button : DrawableGameComponent, IProcessTouch
    {
        SpriteBatch spriteBatch;
        Texture2D tinyTexture;
        Vector2 textPosition;
        bool isPressed;
        int? touchId = null;

        public event EventHandler Click;

        public Button(Game game, string text)
            : base(game)
        {
            Text = text;
        }

        public Rectangle Destination { set; get; }
        public SpriteFont SpriteFont { set; get; }
        public string Text { set; get; }

        public override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(this.GraphicsDevice);

            tinyTexture = new Texture2D(this.GraphicsDevice, 1, 1);
            tinyTexture.SetData<uint>(new uint[] { Color.White.PackedValue });

            base.LoadContent();
        }

        public override void Update(GameTime gameTime)
        {
            if (SpriteFont != null && !String.IsNullOrEmpty(Text))
            {
                Vector2 textSize = SpriteFont.MeasureString(Text);
                textPosition = 
                    new Vector2((int)(Destination.Left + (Destination.Width - textSize.X) / 2), 
                                (int)(Destination.Top + (Destination.Height - textSize.Y) / 2));
            }
            base.Update(gameTime);
        }

        public bool ProcessTouch(TouchLocation touch)
        {
            bool touchHandled = false;
            bool isInside = Destination.Contains((int)touch.Position.X,
                                                 (int)touch.Position.Y);
            switch (touch.State)
            {
                case TouchLocationState.Pressed:
                    if (isInside)
                    {
                        isPressed = true;
                        touchId = touch.Id;
                        touchHandled = true;
                    }
                    break;

                case TouchLocationState.Moved:
                    if (touchId.HasValue && touchId.Value == touch.Id)
                    {
                        isPressed = isInside;
                        touchHandled = true;
                    }
                    break;

                case TouchLocationState.Released:
                    if (touchId.HasValue && touchId.Value == touch.Id)
                    {
                        if (isInside && Click != null)
                            Click(this, EventArgs.Empty);

                        touchId = null;
                        isPressed = false;
                        touchHandled = true;
                    }
                    break;
            }
            return touchHandled;
        }

        public override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();

            if (isPressed)
            {
                // Draw reverse-video background
                spriteBatch.Draw(tinyTexture, Destination, Color.White);
            }
            else
            {
                // Draw button border and background
                Rectangle rect = Destination;

                spriteBatch.Draw(tinyTexture, rect, Color.White);
                rect.Inflate(-3, -3);
                spriteBatch.Draw(tinyTexture, rect, Color.Black);
            }

            // Draw button text
            if (SpriteFont != null && !String.IsNullOrEmpty(Text))
                spriteBatch.DrawString(SpriteFont, Text, textPosition, 
                                       isPressed ? Color.Black : Color.White);

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
