using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace Petzold.Phone.Xna
{
    public struct LineSegment : IGeometrySegment
    {
        readonly float a, b;         // as in x = ay + b

        public LineSegment(Vector2 point1, Vector2 point2) : this()
        {
            Point1 = point1;
            Point2 = point2;

            a = (Point2.X - Point1.X) / (Point2.Y - Point1.Y);
            b = Point1.X - a * Point1.Y;
        }

        public Vector2 Point1 { private set; get; }
        public Vector2 Point2 { private set; get; }

        public void GetAllX(float y, IList<float> xCollection)
        {
            if ((Point2.Y > Point1.Y && y >= Point1.Y && y < Point2.Y) ||
                (Point2.Y < Point1.Y && y <= Point1.Y && y > Point2.Y))
            {
                xCollection.Add(a * y + b);
            }
        }
    }
}
