using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace Petzold.Phone.Xna
{
    public struct RoundCappedLine : IGeometrySegment
    {
        LineSegment lineSegment1;
        ArcSegment arcSegment1;
        LineSegment lineSegment2;
        ArcSegment arcSegment2;

        public RoundCappedLine(Vector2 point1, Vector2 point2, float radius) : this()
        {
            Point1 = point1;
            Point2 = point2;
            Radius = radius;

            Vector2 vector = point2 - point1;
            Vector2 normVect = vector;
            normVect.Normalize();

            Vector2 pt1a = Point1 + radius * new Vector2(normVect.Y, -normVect.X);
            Vector2 pt2a = pt1a + vector;
            Vector2 pt1b = Point1 + radius * new Vector2(-normVect.Y, normVect.X);
            Vector2 pt2b = pt1b + vector;

            lineSegment1 = new LineSegment(pt1a, pt2a);
            arcSegment1 = new ArcSegment(point2, radius, pt2a, pt2b);
            lineSegment2 = new LineSegment(pt2b, pt1b);
            arcSegment2 = new ArcSegment(point1, radius, pt1b, pt1a);
        }

        public Vector2 Point1 { private set; get; }
        public Vector2 Point2 { private set; get; }
        public float Radius { private set; get; }

        public void GetAllX(float y, IList<float> xCollection)
        {
            arcSegment1.GetAllX(y, xCollection);
            lineSegment1.GetAllX(y, xCollection);
            arcSegment2.GetAllX(y, xCollection);
            lineSegment2.GetAllX(y, xCollection);
        }
    }
}
