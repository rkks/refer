using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace CarOnInfinityCourse
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        const float SPEED = 0.1f;           // laps per second
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Viewport viewport;
        Texture2D car;
        Vector2 carCenter;
        Curve xCurve = new Curve();
        Curve yCurve = new Curve();
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
            float[] xValues = { 0, 0.293f, 1, 1.707f, 2, 2.293f, 3, 3.707f, 
                                4, 3.707f, 3, 2.293f, 2, 1.707f, 1, 0.293f };
            float[] yValues = { 1, 0.293f, 0, 0.293f, 1, 1.707f, 2, 1.707f, 
                                1, 0.293f, 0, 0.293f, 1, 1.707f, 2, 1.707f };

            for (int i = -1; i < 18; i++)
            {
                int index = (i + 16) % 16;
                float t = 0.0625f * i;
                xCurve.Keys.Add(new CurveKey(t, xValues[index]));
                yCurve.Keys.Add(new CurveKey(t, yValues[index]));
            }
            xCurve.ComputeTangents(CurveTangent.Smooth);
            yCurve.ComputeTangents(CurveTangent.Smooth);
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            viewport = this.GraphicsDevice.Viewport;
            car = this.Content.Load<Texture2D>("Car");
            carCenter = new Vector2(car.Width / 2, car.Height / 2);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            float t = (SPEED * (float)gameTime.TotalGameTime.TotalSeconds) % 1;
            float x = GetValue(t, true);
            float y = GetValue(t, false);
            position = new Vector2(x, y);

            rotation = MathHelper.PiOver2 + (float)
                Math.Atan2(GetValue(t + 0.001f, false) - GetValue(t - 0.001f, false),
                           GetValue(t + 0.001f, true) - GetValue(t - 0.001f, true));

            base.Update(gameTime);
        }

        float GetValue(float t, bool isX)
        {
            if (isX)
                return xCurve.Evaluate(t) * (viewport.Width - 2 * car.Width) / 4 + car.Width;

            return yCurve.Evaluate(t) * (viewport.Height - 2 * car.Width) / 2 + car.Width;
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
