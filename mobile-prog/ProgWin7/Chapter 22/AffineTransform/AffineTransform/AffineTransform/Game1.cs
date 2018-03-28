using System;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Petzold.Phone.Xna;

namespace NonAffineTransform
{
    public class Game1 : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        Texture2D texture;
        Matrix matrix = Matrix.Identity;
        Dragger draggerUL, draggerUR, draggerLL;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);
        }

        protected override void Initialize()
        {
            draggerUL = new Dragger(this);
            draggerUL.PositionChanged += OnDraggerPositionChanged;
            this.Components.Add(draggerUL);

            draggerUR = new Dragger(this);
            draggerUR.PositionChanged += OnDraggerPositionChanged;
            this.Components.Add(draggerUR);

            draggerLL = new Dragger(this);
            draggerLL.PositionChanged += OnDraggerPositionChanged;
            this.Components.Add(draggerLL);

            base.Initialize();
        }

        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            Viewport viewport = this.GraphicsDevice.Viewport;
            texture = this.Content.Load<Texture2D>("PetzoldTattoo");

            draggerUL.Position = new Vector2((viewport.Width - texture.Width) / 2,
                                             (viewport.Height - texture.Height) / 2);

            draggerUR.Position = draggerUL.Position + new Vector2(texture.Width, 0);
            draggerLL.Position = draggerUL.Position + new Vector2(0, texture.Height);

            OnDraggerPositionChanged(null, EventArgs.Empty);
        }

        void OnDraggerPositionChanged(object sender, EventArgs args)
        {
            matrix = MatrixHelper.ComputeMatrix(new Vector2(texture.Width, texture.Height),
                                                draggerUL.Position,
                                                draggerUR.Position,
                                                draggerLL.Position);
        }

        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            TouchCollection touches = TouchPanel.GetState();

            foreach (TouchLocation touch in touches)
            {
                bool touchHandled = false;

                foreach (GameComponent component in this.Components)
                {
                    if (component is IProcessTouch &&
                        (component as IProcessTouch).ProcessTouch(touch))
                    {
                        touchHandled = true;
                        break;
                    }
                }

                if (touchHandled == true)
                    continue;
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            spriteBatch.Begin(SpriteSortMode.Immediate, null, null, null, null, null, matrix);
            spriteBatch.Draw(texture, Vector2.Zero, Color.White);
            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
