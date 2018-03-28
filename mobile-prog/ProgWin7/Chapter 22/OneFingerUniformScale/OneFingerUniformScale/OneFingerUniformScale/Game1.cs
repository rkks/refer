using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace OneFingerUniformScale
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D texture;
        Vector2 screenCenter;
        Vector2 textureCenter;
        float textureScale = 1;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.FreeDrag;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Viewport viewport = this.GraphicsDevice.Viewport;
            screenCenter = new Vector2(viewport.Width / 2, viewport.Height / 2);

            texture = this.Content.Load<Texture2D>("PetzoldTattoo");
            textureCenter = new Vector2(texture.Width / 2, texture.Height / 2);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                if (gesture.GestureType == GestureType.FreeDrag)
                {
                    Vector2 prevPosition = gesture.Position - gesture.Delta;
                    float scale = 1;

                    if (Math.Abs(gesture.Delta.X) > Math.Abs(gesture.Delta.Y))
                    {
                        scale = (gesture.Position.X - screenCenter.X) /
                                        (prevPosition.X - screenCenter.X);
                    }
                    else if (gesture.Delta.Y != 0)
                    {
                        scale = (gesture.Position.Y - screenCenter.Y) /
                                        (prevPosition.Y - screenCenter.Y);
                    }

                    if (!float.IsInfinity(scale) && !float.IsNaN(scale))
                    {
                        textureScale = Math.Min(10, 
                                       Math.Max(0.25f, scale * textureScale));
                    }
                }
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin();
            spriteBatch.Draw(texture, screenCenter, null, Color.White, 0,
                             textureCenter, textureScale, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
