using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input.Touch;

namespace Petzold.Phone.Xna
{
    public class Dragger : DrawableGameComponent, IProcessTouch
    {
        SpriteBatch spriteBatch;
        int? touchId;

        public event EventHandler PositionChanged;

        public Dragger(Game game)
            : base(game)
        {
        }

        public Texture2D Texture { set; get; }
        public Vector2 Origin { set; get; }
        public Vector2 Position { set; get; }

        public override void Initialize()
        {
            base.Initialize();
        }

        protected override void  LoadContent()
        {
            spriteBatch = new SpriteBatch(this.GraphicsDevice);

            // Create default texture
            int radius = 48;
            Texture2D texture = new Texture2D(this.GraphicsDevice, 2 * radius, 2 * radius);
            uint[] pixels = new uint[texture.Width * texture.Height];

            for (int y = 0; y < texture.Height; y++)
                for (int x = 0; x < texture.Width; x++)
                {
                    Color clr = Color.Transparent;

                    if ((x - radius) * (x - radius) +
                        (y - radius) * (y - radius) <
                        radius * radius)
                    {
                        clr = new Color(0, 128, 128, 128);
                    }
                    pixels[y * texture.Width + x] = clr.PackedValue;
                }
            texture.SetData<uint>(pixels);

            Texture = texture;
            Origin = new Vector2(radius, radius);

 	        base.LoadContent();
        }

        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
        }

        public bool ProcessTouch(TouchLocation touch)
        {
            if (Texture == null)
                return false;

            bool touchHandled = false;
                
            switch (touch.State)
            {
                case TouchLocationState.Pressed:
                    if ((touch.Position.X > Position.X - Origin.X) &&
                        (touch.Position.X < Position.X - Origin.X + Texture.Width) &&
                        (touch.Position.Y > Position.Y - Origin.Y) &&
                        (touch.Position.Y < Position.Y - Origin.Y + Texture.Height))
                    {
                        touchId = touch.Id;
                        touchHandled = true;
                    }
                    break;

                case TouchLocationState.Moved:
                    if (touchId.HasValue && touchId.Value == touch.Id)
                    {
                        TouchLocation previousTouch;
                        touch.TryGetPreviousLocation(out previousTouch);
                        Position += touch.Position - previousTouch.Position;

                        // Fire the event!
                        if (PositionChanged != null)
                            PositionChanged(this, EventArgs.Empty);

                        touchHandled = true;
                    }
                    break;

                case TouchLocationState.Released:
                    if (touchId.HasValue && touchId.Value == touch.Id)
                    {
                        touchId = null;
                        touchHandled = true;
                    }
                    break;
            }
            return touchHandled;
        }

        public override void Draw(GameTime gameTime)
        {
            if (Texture != null)
            {
                spriteBatch.Begin();
                spriteBatch.Draw(Texture, Position, null, Color.White,
                                 0, Origin, 1, SpriteEffects.None, 0);
                spriteBatch.End();
            }
            base.Draw(gameTime);
        }
    }
}
