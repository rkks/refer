using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Petzold.Phone.Xna;

namespace FlawedFingerPaint
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        RenderTarget2D renderTarget;
        LineRenderer vectorRenderer;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            // Enable gestures
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

            renderTarget = new RenderTarget2D(
                        this.GraphicsDevice,
                        this.GraphicsDevice.PresentationParameters.BackBufferWidth,
                        this.GraphicsDevice.PresentationParameters.BackBufferHeight,
                        false,
                        this.GraphicsDevice.PresentationParameters.BackBufferFormat,
                        DepthFormat.None, 0, RenderTargetUsage.PreserveContents);

            this.GraphicsDevice.SetRenderTarget(renderTarget);
            this.GraphicsDevice.Clear(Color.Navy);
            this.GraphicsDevice.SetRenderTarget(null);

            vectorRenderer = new LineRenderer(this.GraphicsDevice);
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

                if (gesture.GestureType == GestureType.FreeDrag &&
                    gesture.Delta != Vector2.Zero)
                {
                    this.GraphicsDevice.SetRenderTarget(renderTarget);
                    spriteBatch.Begin();
                    vectorRenderer.DrawLine(spriteBatch, 
                                            gesture.Position, 
                                            gesture.Position - gesture.Delta, 
                                            25, Color.Red);    
                    spriteBatch.End();
                    this.GraphicsDevice.SetRenderTarget(null);
                }
            }
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
