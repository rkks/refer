using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace RandomRectangles
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Random rand = new Random();
        RenderTarget2D tinyTexture;
        RenderTarget2D renderTarget;

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

            tinyTexture = new RenderTarget2D(this.GraphicsDevice, 1, 1);
            this.GraphicsDevice.SetRenderTarget(tinyTexture);
            this.GraphicsDevice.Clear(Color.White);
            this.GraphicsDevice.SetRenderTarget(null);

            renderTarget = new RenderTarget2D(
                        this.GraphicsDevice, 
                        this.GraphicsDevice.PresentationParameters.BackBufferWidth, 
                        this.GraphicsDevice.PresentationParameters.BackBufferHeight, 
                        false, 
                        this.GraphicsDevice.PresentationParameters.BackBufferFormat, 
                        DepthFormat.None, 0, RenderTargetUsage.PreserveContents);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            int x1 = rand.Next(renderTarget.Width);
            int x2 = rand.Next(renderTarget.Width);
            int y1 = rand.Next(renderTarget.Height);
            int y2 = rand.Next(renderTarget.Height);
            int r = rand.Next(256);
            int g = rand.Next(256);
            int b = rand.Next(256);
            int a = rand.Next(256);

            Rectangle rect = new Rectangle(Math.Min(x1, x2), Math.Min(y1, y2), 
                                           Math.Abs(x2 - x1), Math.Abs(y2 - y1));
            Color clr = new Color(r, g, b, a);

            this.GraphicsDevice.SetRenderTarget(renderTarget);
            spriteBatch.Begin();
            spriteBatch.Draw(tinyTexture, rect, clr);
            spriteBatch.End();
            this.GraphicsDevice.SetRenderTarget(null);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin();
            spriteBatch.Draw(renderTarget, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
