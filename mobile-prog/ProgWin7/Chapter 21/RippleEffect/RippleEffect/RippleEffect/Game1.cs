using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace RippleEffect
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        const int RIPPLE = 10;
        Texture2D srcTexture;
        Texture2D dstTexture;
        uint[] srcPixels;
        uint[] dstPixels;
        Vector2 position;

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
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            srcTexture = this.Content.Load<Texture2D>("PetzoldTattoo");
            srcPixels = new uint[srcTexture.Width * srcTexture.Height];
            srcTexture.GetData<uint>(srcPixels);

            dstTexture = new Texture2D(this.GraphicsDevice, 
                                       srcTexture.Width, 
                                       srcTexture.Height + 2 * RIPPLE);
            dstPixels = new uint[dstTexture.Width * dstTexture.Height];

            Viewport viewport = this.GraphicsDevice.Viewport;
            position = new Vector2((viewport.Width - dstTexture.Width) / 2, 
                                   (viewport.Height - dstTexture.Height) / 2);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float phase = 
                (MathHelper.TwoPi * (float)gameTime.TotalGameTime.TotalSeconds) % 
                                                                    MathHelper.TwoPi;

            for (int xDst = 0; xDst < dstTexture.Width; xDst++)
            {
                int xSrc = xDst;
                float angle = phase - xDst * MathHelper.TwoPi / 100;
                int offset = (int)(RIPPLE * Math.Sin(angle));

                for (int yDst = 0; yDst < dstTexture.Height; yDst++)
                {
                    int dstIndex = yDst * dstTexture.Width + xDst;
                    int ySrc = yDst - RIPPLE + offset;
                    int srcIndex = ySrc * dstTexture.Width + xSrc;

                    if (ySrc < 0 || ySrc >= srcTexture.Height)
                        dstPixels[dstIndex] = Color.Transparent.PackedValue;
                    else
                        dstPixels[dstIndex] = srcPixels[srcIndex];
                }
            }

            this.GraphicsDevice.Textures[0] = null;
            dstTexture.SetData<uint>(dstPixels);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(dstTexture, position, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
