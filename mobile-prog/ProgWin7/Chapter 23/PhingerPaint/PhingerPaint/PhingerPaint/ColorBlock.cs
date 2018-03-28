using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input.Touch;

namespace PhingerPaint
{
    public class ColorBlock : DrawableGameComponent
    {
        SpriteBatch spriteBatch;
        Texture2D block;

        public ColorBlock(Game game) : base(game)
        {
        }

        public Color Color { set; get; }
        public Rectangle Destination { set; get; }
        public bool IsSelected { set; get; }

        public override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(this.GraphicsDevice);
            block = new Texture2D(this.GraphicsDevice, 1, 1);
            block.SetData<uint>(new uint[] { Color.White.PackedValue });

            base.LoadContent();
        }

        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            Rectangle rect = Destination;

            spriteBatch.Begin();
            spriteBatch.Draw(block, rect, IsSelected ? Color.White : Color.DarkGray);
            rect.Inflate(-6, -6);
            spriteBatch.Draw(block, rect, Color);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}