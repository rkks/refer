using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace PhreeCell
{
    public partial class Game1 : Microsoft.Xna.Framework.Game
    {
        const int wCard = 80;       // width of card
        const int hCard = 112;      // height of card

        // Horizontal measurements
        const int wSurface = 800;   // width of surface
        const int xGap = 16;        // space between piles
        const int xMargin = 8;      // margin on left and right

        // gap between "holds" and "finals"
        const int xMidGap = wSurface - (2 * xMargin + 8 * wCard + 6 * xGap);

        // additional margin on second row
        const int xIndent = (wSurface - (2 * xMargin + 8 * wCard + 7 * xGap)) / 2;

        // Vertical measurements
        const int yMargin = 8;      // vertical margin on top row
        const int yGap = 16;        // vertical margin between rows
        const int yOverlay = 28;    // visible top of cards in piles
        const int hSurface = 2 * yMargin + yGap + 2 * hCard + 19 * yOverlay;

        // Replay button
        const int radiusReplay = xMidGap / 2 - 8;
        static readonly Vector2 centerReplay =
                            new Vector2(wSurface / 2, xMargin + hCard / 2);

        static void CreateCardSpots(Rectangle[] cardSpots)
        {
            // Top row
            int x = xMargin;
            int y = yMargin;

            for (int i = 0; i < 8; i++)
            {
                cardSpots[i] = new Rectangle(x, y, wCard, hCard);
                x += wCard + (i == 3 ? xMidGap : xGap);
            }

            // Bottom row
            x = xMargin + xIndent;
            y += hCard + yGap;

            for (int i = 8; i < 16; i++)
            {
                cardSpots[i] = new Rectangle(x, y, wCard, hCard);
                x += wCard + xGap;
            }
        }

        static Texture2D CreateSurface(GraphicsDevice graphicsDevice, Rectangle[] cardSpots)
        {
            uint backgroundColor = new Color(0, 0, 0x60).PackedValue;
            uint outlineColor = Color.White.PackedValue;
            uint replayColor = Color.Red.PackedValue;
            Texture2D surface = new Texture2D(graphicsDevice, wSurface, hSurface);
            uint[] pixels = new uint[wSurface * hSurface];

            for (int i = 0; i < pixels.Length; i++)
            {
                if ((new Vector2(i % wSurface, i / wSurface) - centerReplay).LengthSquared() < 
                                        radiusReplay * radiusReplay)
                    pixels[i] = replayColor;
                else
                    pixels[i] = backgroundColor;
            }

            foreach (Rectangle rect in cardSpots)
            {
                // tops of rectangles
                for (int x = 0; x < wCard; x++)
                {
                    pixels[(rect.Top - 1) * wSurface + rect.Left + x] = outlineColor;
                    pixels[rect.Bottom * wSurface + rect.Left + x] = outlineColor;
                }

                // sides of rectangles
                for (int y = 0; y < hCard; y++)
                {
                    pixels[(rect.Top + y) * wSurface + rect.Left - 1] = outlineColor;
                    pixels[(rect.Top + y) * wSurface + rect.Right] = outlineColor;
                }
            }

            surface.SetData<uint>(pixels);
            return surface;
        }

        static void ShuffleDeck(CardInfo[] deck)
        {
            Random rand = new Random();

            for (int card = 0; card < 52; card++)
            {
                int random = rand.Next(52);
                CardInfo swap = deck[card];
                deck[card] = deck[random];
                deck[random] = swap;
            }
        }

        static bool IsWithinRectangle(Vector2 point, Rectangle rect)
        {
            return point.X >= rect.Left && 
                   point.X <= rect.Right && 
                   point.Y >= rect.Top && 
                   point.Y <= rect.Bottom;
        }

        static Rectangle GetCardTextureSource(CardInfo cardInfo)
        {
            return new Rectangle(wCard * cardInfo.Rank, 
                                 hCard * cardInfo.Suit, wCard, hCard);
        }

        static CardInfo TopCard(List<CardInfo> cardInfos)
        {
            if (cardInfos.Count > 0)
                return cardInfos[cardInfos.Count - 1];

            return null;
        }

        void Replay()
        {
            for (int i = 0; i < 4; i++)
                holds[i] = null;

            foreach (List<CardInfo> final in finals)
                final.Clear();

            foreach (List<CardInfo> pile in piles)
                pile.Clear();

            ShuffleDeck(deck);

            // Apportion cards to piles            
            for (int card = 0; card < 52; card++)
            {
                piles[card % 8].Add(deck[card]);
            }
            CalculateDisplayMatrix();
        }

        void CalculateDisplayMatrix()
        {
            // This will be 480 based on preferred back buffer settings
            int viewportHeight = this.GraphicsDevice.Viewport.Height;

            // Figure out the total required height and scale vertically
            int maxCardsInPiles = 0;

            foreach (List<CardInfo> pile in piles)
                maxCardsInPiles = Math.Max(maxCardsInPiles, pile.Count);

            int requiredHeight = 2 * yMargin + yGap + 2 * hCard + 
                                            yOverlay * (maxCardsInPiles - 1);

            // Set the matrix for compressed Y if needed to show all cards
            if (requiredHeight > viewportHeight)
                displayMatrix = Matrix.CreateScale(1, (float)viewportHeight / requiredHeight, 1);
            else
                displayMatrix = Matrix.Identity;

            // Find the inverse matrix for hit-testing
            inverseMatrix = Matrix.Invert(displayMatrix);
        }

        bool AnalyzeForAutoMove()
        {
            System.Diagnostics.Debug.WriteLine("Analyze for Automove");

            for (int hold = 0; hold < 4; hold++)
            {
                CardInfo cardInfo = holds[hold];

                if (cardInfo != null && CheckForAutoMove(cardInfo))
                {
                    holds[hold] = null;
                    cardInfo.AutoMoveOffset += new Vector2(cardSpots[hold].X, cardSpots[hold].Y);
                    cardInfo.AutoMoveInterpolation = 1;
                    cardInfo.AutoMoveTime = AutoMoveDuration;

                    System.Diagnostics.Debug.WriteLine("return true 1");

                    return true;
                }
            }

            for (int pile = 0; pile < 8; pile++)
            {
                CardInfo cardInfo = TopCard(piles[pile]);

                if (cardInfo != null && CheckForAutoMove(cardInfo))
                {
                    piles[pile].Remove(cardInfo);
                    cardInfo.AutoMoveOffset += new Vector2(cardSpots[pile + 8].X,
                                        cardSpots[pile + 8].Y + piles[pile].Count * yOverlay);
                    cardInfo.AutoMoveInterpolation = 1;
                    cardInfo.AutoMoveTime = AutoMoveDuration;

                    System.Diagnostics.Debug.WriteLine("return true 2");

                    return true;
                }
            }
            return false;
        }

        bool CheckForAutoMove(CardInfo cardInfo)
        {
            if (cardInfo.Rank == 0)     // ie, ace
            {
                for (int final = 0; final < 4; final++)
                    if (finals[final].Count == 0)
                    {
                        finals[final].Add(cardInfo);
                        cardInfo.AutoMoveOffset = -new Vector2(cardSpots[final + 4].X, cardSpots[final + 4].Y);
                        return true;
                    }
            }
            else if (cardInfo.Rank == 1)    // ie, deuce
            {
                for (int final = 0; final < 4; final++)
                {
                    CardInfo topCardInfo = TopCard(finals[final]);

                    if (topCardInfo != null &&
                        topCardInfo.Suit == cardInfo.Suit &&
                        topCardInfo.Rank == 0)
                    {
                        finals[final].Add(cardInfo);
                        cardInfo.AutoMoveOffset = -new Vector2(cardSpots[final + 4].X, cardSpots[final + 4].Y);
                        return true;
                    }
                }
            }
            else
            {
                int slot = -1;
                int count = 0;

                for (int final = 0; final < 4; final++)
                {
                    CardInfo topCardInfo = TopCard(finals[final]);

                    if (topCardInfo != null)
                    {
                        if (topCardInfo.Suit == cardInfo.Suit &&
                            topCardInfo.Rank == cardInfo.Rank - 1)
                        {
                            slot = final;
                        }
                        else if (topCardInfo.Suit < 2 != cardInfo.Suit < 2 &&
                                 topCardInfo.Rank >= cardInfo.Rank - 1)
                        {
                            count++;
                        }
                    }
                }
                if (slot >= 0 && count == 2)
                {
                    cardInfo.AutoMoveOffset = -new Vector2(cardSpots[slot + 4].X, cardSpots[slot + 4].Y);
                    finals[slot].Add(cardInfo);
                    return true;
                }
            }
            return false;
        }

        bool TryPickUpCard(Vector2 position)
        {
            for (int hold = 0; hold < 4; hold++)
            {
                if (holds[hold] != null && IsWithinRectangle(position, cardSpots[hold]))
                {
                    Point pt = cardSpots[hold].Location;

                    touchedCard = holds[hold];
                    touchedCardOrigin = holds;
                    touchedCardOriginIndex = hold;
                    touchedCardPosition = new Vector2(pt.X, pt.Y);
                    holds[hold] = null;
                    return true;
                }
            }

            for (int pile = 0; pile < 8; pile++)
            {
                if (piles[pile].Count > 0)
                {
                    Rectangle pileSpot = cardSpots[pile + 8];
                    pileSpot.Offset(0, yOverlay * (piles[pile].Count - 1));

                    if (IsWithinRectangle(position, pileSpot))
                    {
                        Point pt = pileSpot.Location;
                        int pileIndex = piles[pile].Count - 1;

                        touchedCard = piles[pile][pileIndex];
                        touchedCardOrigin = piles;
                        touchedCardOriginIndex = pile;
                        touchedCardPosition = new Vector2(pt.X, pt.Y);
                        piles[pile].RemoveAt(pileIndex);
                        return true;
                    }
                }
            }

            return false;
        }

        bool TryPutDownCard(CardInfo touchedCard)
        {
            Vector2 cardCenter = new Vector2(touchedCardPosition.X + wCard / 2,
                                             touchedCardPosition.Y + hCard / 2);

            for (int cardSpot = 0; cardSpot < 16; cardSpot++)
            {
                Rectangle rect = cardSpots[cardSpot];

                // Greatly expand the card-spot rectangle for the piles
                if (cardSpot >= 8)
                    rect.Inflate(0, hSurface - rect.Bottom);

                if (IsWithinRectangle(cardCenter, rect))
                {
                    // Check if the hold is empty
                    if (cardSpot < 4)
                    {
                        int hold = cardSpot;

                        if (holds[hold] == null)
                        {
                            holds[hold] = touchedCard;
                            return true;
                        }
                    }

                    else if (cardSpot < 8)
                    {
                        int final = cardSpot - 4;

                        if (TopCard(finals[final]) == null)
                        {
                            if (touchedCard.Rank == 0)  // ie, an ace
                            {
                                finals[final].Add(touchedCard);
                                return true;
                            }
                        }
                        else if (touchedCard.Suit == TopCard(finals[final]).Suit &&
                                 touchedCard.Rank == TopCard(finals[final]).Rank + 1)
                        {
                            finals[final].Add(touchedCard);
                            return true;
                        }
                    }
                    else
                    {
                        int pile = cardSpot - 8;

                        if (piles[pile].Count == 0)
                        {
                            piles[pile].Add(touchedCard);
                            return true;
                        }
                        else
                        {
                            CardInfo topCard = TopCard(piles[pile]);

                            if (touchedCard.Suit < 2 != topCard.Suit < 2 &&
                                touchedCard.Rank == topCard.Rank - 1)
                            {
                                piles[pile].Add(touchedCard);
                                return true;
                            }
                        }
                    }

                    // The card was in a card-spot rectangle but wasn't a legal drop
                    break;
                }
            }

            // Restore the card to its original place
            if (touchedCardOrigin is CardInfo[])
            {
                (touchedCardOrigin as CardInfo[])[touchedCardOriginIndex] = touchedCard;
            }
            else
            {
                ((touchedCardOrigin as List<CardInfo>[])[touchedCardOriginIndex]).Add(touchedCard);
            }
            return false;
        }

        bool HasWon()
        {
            bool hasWon = true;

            foreach (List<CardInfo> cardInfos in finals)
                hasWon &= cardInfos.Count > 0 && TopCard(cardInfos).Rank == 12;

            return hasWon;
        }
    }
}
