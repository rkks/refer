using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CarOnOvalCourse
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 0.25f;          // laps per second
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D car;
        Vector2 carCenter;
        Point ellipseCenter;
        float ellipseRadiusX, ellipseRadiusY;
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
            Viewport viewport = this.GraphicsDevice.Viewport;
            ellipseCenter = viewport.Bounds.Center;
            ellipseRadiusX = viewport.Width / 2 - car.Width;
            ellipseRadiusY = viewport.Height / 2 - car.Width;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float t = (SPEED * (float)gameTime.TotalGameTime.TotalSeconds) % 1;
            float ellipseAngle = MathHelper.TwoPi * t;
            float x = ellipseCenter.X + ellipseRadiusX * (float)Math.Cos(ellipseAngle);
            float y = ellipseCenter.Y + ellipseRadiusY * (float)Math.Sin(ellipseAngle);
            position = new Vector2(x, y);

            float dxdt = -ellipseRadiusX * (float)Math.Sin(ellipseAngle);
            float dydt = ellipseRadiusY * (float)Math.Cos(ellipseAngle);
            rotation = MathHelper.PiOver2 + (float)Math.Atan2(dydt, dxdt);

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
