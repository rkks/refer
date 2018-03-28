using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace Petzold.Phone.Xna
{
    public struct ArcSegment : IGeometrySegment
    {
        readonly double angle1, angle2;

        public ArcSegment(Vector2 center, float radius, 
                          Vector2 point1, Vector2 point2) : 
            this()
        {
            Center = center;
            Radius = radius;
            Point1 = point1;
            Point2 = point2;
            angle1 = Math.Atan2(point1.Y - center.Y, point1.X - center.X);
            angle2 = Math.Atan2(point2.Y - center.Y, point2.X - center.X);
        }

        public Vector2 Center { private set; get; }
        public float Radius { private set; get; }
        public Vector2 Point1 { private set; get; }
        public Vector2 Point2 { private set; get; }

        public void GetAllX(float y, IList<float> xCollection)
        {
            double sqrtArg = Radius * Radius - Math.Pow(y - Center.Y, 2);

            if (sqrtArg >= 0)
            {
                double sqrt = Math.Sqrt(sqrtArg);
                TryY(y, Center.X + sqrt, xCollection);
                TryY(y, Center.X - sqrt, xCollection);
            }
        }

        public void TryY(double y, double x, IList<float> xCollection)
        {
            double angle = Math.Atan2(y - Center.Y, x - Center.X);

            if ((angle1 < angle2 && (angle1 <= angle && angle < angle2)) ||
                (angle1 > angle2 && (angle1 <= angle || angle < angle2)))
            {
                xCollection.Add((float)x);
            }
        }
    }
}
