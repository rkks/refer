using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CarOnPolylineCourse
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 0.25f;          // laps per second
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D car;
        Vector2 carCenter;
        PolylineInterpolator polylineInterpolator = new PolylineInterpolator();
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
            car = this.Content.Load<Texture2D>("Car");
            carCenter = new Vector2(car.Width / 2, car.Height / 2);
            float margin = car.Width;
            Viewport viewport = this.GraphicsDevice.Viewport;

            polylineInterpolator.Vertices.Add(
                        new Vector2(car.Width, car.Width));
            polylineInterpolator.Vertices.Add(
                        new Vector2(viewport.Width - car.Width, car.Width));
            polylineInterpolator.Vertices.Add(
                        new Vector2(car.Width, viewport.Height - car.Width));
            polylineInterpolator.Vertices.Add(
                        new Vector2(viewport.Width - car.Width, viewport.Height - car.Width));
            polylineInterpolator.Vertices.Add(
                        new Vector2(car.Width, car.Width));
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float t = (SPEED * (float)gameTime.TotalGameTime.TotalSeconds) % 1;
            float angle;
            position = polylineInterpolator.GetValue(t, false, out angle);
            rotation = angle + MathHelper.PiOver2;

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
