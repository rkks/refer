using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace PinwheelText
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Vector2 screenCenter;
        RenderTarget2D renderTarget;
        Vector2 textureCenter;
        float rotationAngle;

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

            // Get viewport info
            Viewport viewport = this.GraphicsDevice.Viewport;
            screenCenter = new Vector2(viewport.Width / 2, viewport.Height / 2);

            // Load font and get text size
            SpriteFont segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            string text = " Windows Phone 7";
            Vector2 textSize = segoe14.MeasureString(text);

            // Create RenderTarget2D
            renderTarget = 
                new RenderTarget2D(this.GraphicsDevice, 2 * (int)textSize.X, 
                                                        2 * (int)textSize.X);

            // Find center
            textureCenter = new Vector2(renderTarget.Width / 2,
                                        renderTarget.Height / 2);

            Vector2 textOrigin = new Vector2(0, textSize.Y / 2);

            // Set the RenderTarget2D to the GraphicsDevice        
            this.GraphicsDevice.SetRenderTarget(renderTarget);

            // Clear the RenderTarget2D and render the text
            this.GraphicsDevice.Clear(Color.Transparent);
            spriteBatch.Begin();

            for (float t = 0; t < 1; t += 1f / 32)
            {
                float angle = t * MathHelper.TwoPi;
                Color clr = Color.Lerp(Color.Cyan, Color.Yellow, t);
                spriteBatch.DrawString(segoe14, text, textureCenter, clr, 
                                       angle, textOrigin, 1, SpriteEffects.None, 0);
            }

            spriteBatch.End();

            // Restore the GraphicsDevice back to normal
            this.GraphicsDevice.SetRenderTarget(null);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            rotationAngle = 
                (MathHelper.TwoPi * (float) gameTime.TotalGameTime.TotalSeconds / 8) % 
                                                        MathHelper.TwoPi;
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.Draw(renderTarget, screenCenter, null, Color.White, 
                             rotationAngle, textureCenter, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
