using System;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Phone.Shell;

namespace XnaTombstoning
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Settings settings;
        SpriteFont segoe14;
        Viewport viewport;
        Random rand = new Random();
        StringBuilder text = new StringBuilder();
        Vector2 position;
        int numTaps = 0;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.Tap;

            PhoneApplicationService appService = PhoneApplicationService.Current;
            appService.Launching += OnAppServiceLaunching;
            appService.Activated += OnAppServiceActivated;
            appService.Deactivated += OnAppServiceDeactivated;
            appService.Closing += OnAppServiceClosing;
        }

        protected override void Initialize()
        {
            base.Initialize();
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

            while (TouchPanel.IsGestureAvailable)
                if (TouchPanel.ReadGesture().GestureType == GestureType.Tap)
                {
                    numTaps++;
                    settings.BackgroundColor =  new Color((byte)rand.Next(255), 
                                                          (byte)rand.Next(255), 
                                                          (byte)rand.Next(255));
                }

            text.Remove(0, text.Length);
            text.AppendFormat("{0} taps total", numTaps);
            Vector2 textSize = segoe14.MeasureString(text.ToString());
            position = new Vector2((viewport.Width - textSize.X) / 2,
                                   (viewport.Height - textSize.Y) / 2);

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(settings.BackgroundColor);

            spriteBatch.Begin();
            spriteBatch.DrawString(segoe14, text, position, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }

        protected override void OnActivated(object sender, EventArgs args)
        {
            if (PhoneApplicationService.Current.State.ContainsKey("numTaps"))
                numTaps = (int)PhoneApplicationService.Current.State["numTaps"];

            base.OnActivated(sender, args);
        }

        protected override void OnDeactivated(object sender, EventArgs args)
        {
            PhoneApplicationService.Current.State["numTaps"] = numTaps;
            base.OnDeactivated(sender, args);
        }

        void OnAppServiceLaunching(object sender, LaunchingEventArgs args)
        {
            settings = Settings.Load();
        }

        void OnAppServiceActivated(object sender, ActivatedEventArgs args)
        {
            settings = Settings.Load();
        }

        void OnAppServiceDeactivated(object sender, DeactivatedEventArgs args)
        {
            settings.Save();
        }

        void OnAppServiceClosing(object sender, ClosingEventArgs args)
        {
            settings.Save();
        }
    }
}
