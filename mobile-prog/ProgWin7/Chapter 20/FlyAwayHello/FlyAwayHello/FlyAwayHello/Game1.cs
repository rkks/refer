using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;

namespace FlyAwayHello
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        static readonly TimeSpan ANIMATION_DURATION = TimeSpan.FromSeconds(5);
        const int CHAR_SPACING = 5;

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Viewport viewport;
        List<SpriteInfo> spriteInfos = new List<SpriteInfo>();
        Random rand = new Random();
        bool isAnimationGoing;
        TimeSpan animationStartTime;

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
            viewport = this.GraphicsDevice.Viewport;

            Texture2D horzBar = Content.Load<Texture2D>("HorzBar");
            Texture2D vertBar = Content.Load<Texture2D>("VertBar");

            int x = (viewport.Width - 5 * horzBar.Width - 4 * CHAR_SPACING) / 2;
            int y = (viewport.Height - vertBar.Height) / 2;
            int xRight = horzBar.Width - vertBar.Width;
            int yMiddle = (vertBar.Height - horzBar.Height) / 2;
            int yBottom = vertBar.Height - horzBar.Height;

            // H
            spriteInfos.Add(new SpriteInfo(vertBar, x, y));
            spriteInfos.Add(new SpriteInfo(vertBar, x + xRight, y));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y + yMiddle));

            // E
            x += horzBar.Width + CHAR_SPACING;
            spriteInfos.Add(new SpriteInfo(vertBar, x, y));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y + yMiddle));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y + yBottom));

            // LL
            for (int i = 0; i < 2; i++)
            {
                x += horzBar.Width + CHAR_SPACING;
                spriteInfos.Add(new SpriteInfo(vertBar, x, y));
                spriteInfos.Add(new SpriteInfo(horzBar, x, y + yBottom));
            }

            // O
            x += horzBar.Width + CHAR_SPACING;
            spriteInfos.Add(new SpriteInfo(vertBar, x, y));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y));
            spriteInfos.Add(new SpriteInfo(horzBar, x, y + yBottom));
            spriteInfos.Add(new SpriteInfo(vertBar, x + xRight, y));
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            if (isAnimationGoing)
            {
                TimeSpan animationTime = gameTime.TotalGameTime - animationStartTime;
                double fractionTime = (double)animationTime.Ticks / ANIMATION_DURATION.Ticks;

                if (fractionTime >= 1)
                {
                    isAnimationGoing = false;
                    fractionTime = 1;
                }

                SpriteInfo.InterpolationFactor = (float)Math.Sin(Math.PI * fractionTime);
            }
            else
            {
                TouchCollection touchCollection = TouchPanel.GetState();
                bool atLeastOneTouchPointPressed = false;

                foreach (TouchLocation touchLocation in touchCollection)
                    atLeastOneTouchPointPressed |=
                        touchLocation.State == TouchLocationState.Pressed;

                if (atLeastOneTouchPointPressed)
                {
                    foreach (SpriteInfo spriteInfo in spriteInfos)
                    {
                        float r1 = (float)rand.NextDouble() - 0.5f;
                        float r2 = (float)rand.NextDouble() - 0.5f;
                        float r3 = (float)rand.NextDouble();

                        spriteInfo.PositionOffset = new Vector2(r1 * viewport.Width,
                                                                r2 * viewport.Height);
                        spriteInfo.MaximumRotation = 2 * (float)Math.PI * r3;
                    }
                    animationStartTime = gameTime.TotalGameTime;
                    isAnimationGoing = true;
                }
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);
            spriteBatch.Begin();

            foreach (SpriteInfo spriteInfo in spriteInfos)
            {
                spriteBatch.Draw(spriteInfo.Texture2D, spriteInfo.Position, null,
                    Color.Lerp(Color.Blue, Color.Red, SpriteInfo.InterpolationFactor),
                    spriteInfo.Rotation, Vector2.Zero, 1, SpriteEffects.None, 0);
            }

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
