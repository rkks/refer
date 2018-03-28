using System;
using Microsoft.Xna.Framework;

namespace PhreeCell
{
    class CardInfo
    {
        static string[] ranks = { "Ace", "Deuce", "Three", "Four", 
                                  "Five", "Six", "Seven", "Eight", 
                                  "Nine", "Ten", "Jack", "Queen", "King" };
        static string[] suits = { "Spades", "Clubs", "Hearts", "Diamonds" };

        public int Suit { protected set; get; }
        public int Rank { protected set; get; }

        public Vector2 AutoMoveOffset { set; get; }
        public TimeSpan AutoMoveTime { set; get; }
        public float AutoMoveInterpolation { set; get; }

        public CardInfo(int suit, int rank)
        {
            Suit = suit;
            Rank = rank;
        }

        // used for debugging purposes
        public override string ToString()
        {
            return ranks[Rank] + " of " + suits[Suit];
        }
    }
}
