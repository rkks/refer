using System;
using System.Device.Location;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace XnaLocation
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        SpriteFont segoe14;
        string text = "Obtaining location...";
        Viewport viewport;
        Vector2 textPosition;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);
        }

        protected override void Initialize()
        {
            GeoCoordinateWatcher geoWatcher = new GeoCoordinateWatcher();
            geoWatcher.PositionChanged += OnGeoWatcherPositionChanged;
            geoWatcher.Start();

            base.Initialize();
        }

        void OnGeoWatcherPositionChanged(object sender, 
                                         GeoPositionChangedEventArgs<GeoCoordinate> args)
        {
            text = String.Format("Latitude: {0:F3}\r\n" + 
                                 "Longitude: {1:F3}\r\n" +
                                 "Altitude: {2}\r\n\r\n" +
                                 "{3}",
                                 args.Position.Location.Latitude,
                                 args.Position.Location.Longitude, 
                                 args.Position.Location.Altitude, 
                                 args.Position.Timestamp);
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            segoe14 = this.Content.Load<SpriteFont>("Segoe14");
            viewport = this.GraphicsDevice.Viewport;
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            Vector2 textSize = segoe14.MeasureString(text);
            textPosition = new Vector2((viewport.Width - textSize.X) / 2,
                                       (viewport.Height - textSize.Y) / 2);
            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, text, textPosition, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
