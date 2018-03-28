using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CarOnRectangularCourse
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 100;            // pixels per second
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D car;
        Vector2 carCenter;
        Vector2[] turnPoints = new Vector2[4];
        int sideIndex = 0;
        Vector2 position;
        float rotation;

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
            car = this.Content.Load<Texture2D>("car");
            carCenter = new Vector2(car.Width / 2, car.Height / 2);
            float margin = car.Width;
            Viewport viewport = this.GraphicsDevice.Viewport;
            turnPoints[0] = new Vector2(margin, margin);
            turnPoints[1] = new Vector2(viewport.Width - margin, margin);
            turnPoints[2] = new Vector2(viewport.Width - margin, viewport.Height - margin);
            turnPoints[3] = new Vector2(margin, viewport.Height - margin);
            position = turnPoints[0];
            rotation = MathHelper.PiOver2;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float pixels = SPEED * (float)gameTime.ElapsedGameTime.TotalSeconds;

            switch (sideIndex)
            {
                case 0:         // top
                    position.X += pixels;

                    if (position.X > turnPoints[1].X)
                    {
                        position.X = turnPoints[1].X;
                        position.Y = turnPoints[1].Y + (position.X - turnPoints[1].X);
                        rotation = MathHelper.Pi;
                        sideIndex = 1;
                    }
                    break;

                case 1:         // right
                    position.Y += pixels;

                    if (position.Y > turnPoints[2].Y)
                    {
                        position.Y = turnPoints[2].Y;
                        position.X = turnPoints[2].X - (position.Y - turnPoints[2].Y);
                        rotation = -MathHelper.PiOver2;
                        sideIndex = 2;
                    }
                    break;

                case 2:         // bottom
                    position.X -= pixels;

                    if (position.X < turnPoints[3].X)
                    {
                        position.X = turnPoints[3].X;
                        position.Y = turnPoints[3].Y + (position.X - turnPoints[3].X);
                        rotation = 0;
                        sideIndex = 3;
                    }
                    break;

                case 3:         // left
                    position.Y -= pixels;

                    if (position.Y < turnPoints[0].Y)
                    {
                        position.Y = turnPoints[0].Y;
                        position.X = turnPoints[0].X - (position.Y - turnPoints[0].Y);
                        rotation = MathHelper.PiOver2;
                        sideIndex = 0;
                    }
                    break;
            }
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Blue);

            spriteBatch.Begin();
            spriteBatch.Draw(car, position, null, Color.White, rotation,
                             carCenter, 1, SpriteEffects.None, 0);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
