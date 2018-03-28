using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Input.Touch;
using Microsoft.Phone.Shell;

namespace PhreeCell
{
    public partial class Game1 : Microsoft.Xna.Framework.Game
    {
        static readonly TimeSpan AutoMoveDuration = TimeSpan.FromSeconds(0.25);

        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        CongratulationsComponent congratsComponent;

        Texture2D cards;
        Texture2D surface;
        Rectangle[] cardSpots = new Rectangle[16];

        Matrix displayMatrix;
        Matrix inverseMatrix;

        CardInfo[] deck = new CardInfo[52];
        List<CardInfo>[] piles = new List<CardInfo>[8];
        CardInfo[] holds = new CardInfo[4];
        List<CardInfo>[] finals = new List<CardInfo>[4];

        bool firstDragInGesture = true;
        CardInfo touchedCard;
        Vector2 touchedCardPosition;
        object touchedCardOrigin;
        int touchedCardOriginIndex;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            // Frame rate is 30 fps by default for Windows Phone.
            TargetElapsedTime = TimeSpan.FromTicks(333333);

            graphics.IsFullScreen = true;
            graphics.PreferredBackBufferWidth = 800;
            graphics.PreferredBackBufferHeight = 480;

            // Enable gestures
            TouchPanel.EnabledGestures = GestureType.Tap |
                                         GestureType.FreeDrag |
                                         GestureType.DragComplete;
        }

        protected override void Initialize()
        {
            // Initialize deck
            for (int suit = 0; suit < 4; suit++)
                for (int rank = 0; rank < 13; rank++)
                {
                    CardInfo cardInfo = new CardInfo(suit, rank);
                    deck[suit * 13 + rank] = cardInfo;
                }

            // Create the List objects for the 8 piles
            for (int pile = 0; pile < 8; pile++)
                piles[pile] = new List<CardInfo>();

            // Create the List objects for the 4 finals
            for (int final = 0; final < 4; final++)
                finals[final] = new List<CardInfo>();

            // Create congratulations component
            congratsComponent = new CongratulationsComponent(this);
            congratsComponent.Enabled = false;
            this.Components.Add(congratsComponent);
            base.Initialize();
        }

        protected override void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // Load large bitmap containing cards
            cards = this.Content.Load<Texture2D>("cards");

            // Create the 16 rectangular areas for the cards and the bitmap surface
            CreateCardSpots(cardSpots);
            surface = CreateSurface(this.GraphicsDevice, cardSpots);
        }

        protected override void OnDeactivated(object sender, EventArgs args)
        {
            PhoneApplicationService appService = PhoneApplicationService.Current;

            // Save piles integers
            List<int>[] piles = new List<int>[8];

            for (int i = 0; i < piles.Length; i++)
            {
                piles[i] = new List<int>();

                foreach (CardInfo cardInfo in this.piles[i])
                    piles[i].Add(13 * cardInfo.Suit + cardInfo.Rank);
            }
            appService.State["piles"] = piles;

            // Save finals integers
            List<int>[] finals = new List<int>[4];

            for (int i = 0; i < finals.Length; i++)
            {
                finals[i] = new List<int>();

                foreach (CardInfo cardInfo in this.finals[i])
                    finals[i].Add(13 * cardInfo.Suit + cardInfo.Rank);
            }
            appService.State["finals"] = finals;

            // Save holds integers
            int[] holds = new int[4];

            for (int i = 0; i < holds.Length; i++)
            {
                if (this.holds[i] == null)
                    holds[i] = -1;
                else
                    holds[i] = 13 * this.holds[i].Suit + this.holds[i].Rank;
            }
            appService.State["holds"] = holds;

            base.OnDeactivated(sender, args);
        }

        protected override void OnActivated(object sender, EventArgs args)
        {
            PhoneApplicationService appService = PhoneApplicationService.Current;

            if (appService.State.ContainsKey("piles"))
            {
                // Retrieve piles integers
                List<int>[] piles = appService.State["piles"] as List<int>[];

                for (int i = 0; i < piles.Length; i++)
                {
                    foreach (int cardindex in piles[i])
                        this.piles[i].Add(deck[cardindex]);
                }

                // Retrieve finals integers
                List<int>[] finals = appService.State["finals"] as List<int>[];

                for (int i = 0; i < finals.Length; i++)
                {
                    foreach (int cardindex in finals[i])
                        this.finals[i].Add(deck[cardindex]);
                }

                // Retrieve holds integers
                int[] holds = appService.State["holds"] as int[];

                for (int i = 0; i < holds.Length; i++)
                {
                    if (holds[i] != -1)
                        this.holds[i] = deck[holds[i]];
                }
                CalculateDisplayMatrix();
            }
            else
            {
                Replay();
            }
            base.OnActivated(sender, args);
        }


        protected override void UnloadContent()
        {
        }

        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();

            // Process auto-move card and perhaps initiate next auto-move
            bool checkForNextAutoMove = false;

            foreach (List<CardInfo> final in finals)
                foreach (CardInfo cardInfo in final)
                {
                    if (cardInfo.AutoMoveTime > TimeSpan.Zero)
                    {
                        cardInfo.AutoMoveTime -= gameTime.ElapsedGameTime;

                        if (cardInfo.AutoMoveTime <= TimeSpan.Zero)
                        {
                            cardInfo.AutoMoveTime = TimeSpan.Zero;
                            checkForNextAutoMove = true;
                        }
                        cardInfo.AutoMoveInterpolation = (float)cardInfo.AutoMoveTime.Ticks / 
                                                                    AutoMoveDuration.Ticks;
                    }
                }

            if (checkForNextAutoMove && !AnalyzeForAutoMove() && HasWon())
            {
                congratsComponent.Enabled = true;
            }

            while (TouchPanel.IsGestureAvailable)
            {
                GestureSample gesture = TouchPanel.ReadGesture();

                // Adjust position and delta for compressed image
                Vector2 position = Vector2.Transform(gesture.Position, inverseMatrix);
                Vector2 delta = position - Vector2.Transform(gesture.Position - gesture.Delta,
                                                             inverseMatrix);
                switch (gesture.GestureType)
                {
                    case GestureType.Tap:
                        // Check if Replay is pressed
                        if ((position - centerReplay).Length() < radiusReplay)
                        {
                            congratsComponent.Enabled = false;
                            Replay();
                        }
                        break;

                    case GestureType.FreeDrag:
                        // Continue to move a dragged card
                        if (touchedCard != null)
                        {
                            touchedCardPosition += delta;
                        }
                        // Try to pick up a card
                        else if (firstDragInGesture)
                        {
                            TryPickUpCard(position);
                        }
                        firstDragInGesture = false;
                        break;

                    case GestureType.DragComplete:
                        if (touchedCard != null && TryPutDownCard(touchedCard))
                        {
                            CalculateDisplayMatrix();

                            if (!AnalyzeForAutoMove() && HasWon())
                            {
                                congratsComponent.Enabled = true;
                            }
                        }
                        firstDragInGesture = true;
                        touchedCard = null;
                        break;
                }
            }

            base.Update(gameTime);
        }

        protected override void Draw(GameTime gameTime)
        {
            spriteBatch.Begin(SpriteSortMode.Immediate, null, null, null, null, null, 
                              displayMatrix);
            spriteBatch.Draw(surface, Vector2.Zero, Color.White);

            // Draw holds
            for (int hold = 0; hold < 4; hold++)
            {
                CardInfo cardInfo = holds[hold];

                if (cardInfo != null)
                {
                    Rectangle source = GetCardTextureSource(cardInfo);
                    Vector2 destination = new Vector2(cardSpots[hold].X, cardSpots[hold].Y);
                    spriteBatch.Draw(cards, destination, source, Color.White);
                }
            }

            // Draw piles
            for (int pile = 0; pile < 8; pile++)
            {
                Rectangle cardSpot = cardSpots[pile + 8];

                for (int card = 0; card < piles[pile].Count; card++)
                {
                    CardInfo cardInfo = piles[pile][card];
                    Rectangle source = GetCardTextureSource(cardInfo);
                    Vector2 destination = new Vector2(cardSpot.X, cardSpot.Y + card * yOverlay);
                    spriteBatch.Draw(cards, destination, source, Color.White);
                }
            }

            // Draw finals including all previous cards (for auto-move)
            for (int pass = 0; pass < 2; pass++)
            {
                for (int final = 0; final < 4; final++)
                {
                    for (int card = 0; card < finals[final].Count; card++)
                    {
                        CardInfo cardInfo = finals[final][card];

                        if (pass == 0 && cardInfo.AutoMoveInterpolation == 0 ||
                            pass == 1 && cardInfo.AutoMoveInterpolation != 0)
                        {
                            Rectangle source = GetCardTextureSource(cardInfo);
                            Vector2 destination = new Vector2(cardSpots[final + 4].X,
                                                              cardSpots[final + 4].Y) +
                                         cardInfo.AutoMoveInterpolation * cardInfo.AutoMoveOffset;
                            spriteBatch.Draw(cards, destination, source, Color.White);
                        }
                    }
                }
            }

            // Draw touched card
            if (touchedCard != null)
            {
                Rectangle source = GetCardTextureSource(touchedCard);
                spriteBatch.Draw(cards, touchedCardPosition, source, Color.White);
            }

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
