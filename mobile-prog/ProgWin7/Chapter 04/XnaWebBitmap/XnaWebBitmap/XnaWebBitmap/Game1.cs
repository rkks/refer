using System;
using System.Net;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace XnaWebBitmap
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D helloTexture;
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
            spriteBatch = new SpriteBatch(GraphicsDevice);

            WebClient webClient = new WebClient();
            webClient.OpenReadCompleted += OnWebClientOpenReadCompleted;
            webClient.OpenReadAsync(new Uri("http://www.charlespetzold.com/Media/HelloWP7.jpg"));
        }

        void OnWebClientOpenReadCompleted(object sender, OpenReadCompletedEventArgs args)
        {
            if (!args.Cancelled && args.Error == null)
            {
                helloTexture = Texture2D.FromStream(this.GraphicsDevice, args.Result);
                Viewport viewport = this.GraphicsDevice.Viewport;
                position = new Vector2((viewport.Width - helloTexture.Width) / 2,
                                       (viewport.Height - helloTexture.Height) / 2);
            }
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            if (helloTexture != null)
            {
                spriteBatch.Begin();
                spriteBatch.Draw(helloTexture, position, Color.White);
                spriteBatch.End();
            }

            base.Draw(gameTime);
        }
    }
}
