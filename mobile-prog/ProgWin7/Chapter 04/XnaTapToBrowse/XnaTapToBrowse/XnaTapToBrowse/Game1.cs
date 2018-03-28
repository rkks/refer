using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Phone.Tasks;

namespace XnaTapToBrowse
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        Texture2D texture;
        PhotoChooserTask photoChooser = new PhotoChooserTask();

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            TouchPanel.EnabledGestures = GestureType.Tap;
            photoChooser.Completed += OnPhotoChooserCompleted;
        }

        protected override void Initialize()
        {
            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
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
                    photoChooser.Show();

            base.Update(gameTime);
        }

        void OnPhotoChooserCompleted(object sender, PhotoResult args)
        {
            if (args.TaskResult == TaskResult.OK)
                texture = Texture2D.FromStream(this.GraphicsDevice, args.ChosenPhoto);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.Navy);

            if (texture != null)
            {
                spriteBatch.Begin();
                spriteBatch.Draw(texture, this.GraphicsDevice.Viewport.Bounds, Color.White);
                spriteBatch.End();
            }

            base.Draw(gameTime);
        }
    }
}
