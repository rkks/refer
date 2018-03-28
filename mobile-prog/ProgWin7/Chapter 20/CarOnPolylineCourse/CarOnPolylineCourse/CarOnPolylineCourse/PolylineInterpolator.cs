using System;
using System.Collections.Generic;
using Microsoft.Xna.Framework;

namespace CarOnPolylineCourse
{
    public class PolylineInterpolator
    {
        public PolylineInterpolator()
        {
            Vertices = new List<Vector2>();
        }

        public List<Vector2> Vertices { protected set; get; }

        public float TotalLength()
        {
            float totalLength = 0;

            // Notice looping begins at index 1
            for (int i = 1; i < Vertices.Count; i++)
            {
                totalLength += (Vertices[i] - Vertices[i - 1]).Length();
            }
            return totalLength;
        }

        public Vector2 GetValue(float t, bool smooth, out float angle)
        {
            if (Vertices.Count == 0)
            {
                return GetValue(Vector2.Zero, Vector2.Zero, t, smooth, out angle);
            }

            else if (Vertices.Count == 1)
            {
                return GetValue(Vertices[0], Vertices[0], t, smooth, out angle);
            }

            if (Vertices.Count == 2)
            {
                return GetValue(Vertices[0], Vertices[1], t, smooth, out angle);
            }

            // Calculate total length
            float totalLength = TotalLength();
            float accumLength = 0;

            // Notice looping begins at index 1
            for (int i = 1; i < Vertices.Count; i++)
            {
                float prevLength = accumLength;
                accumLength += (Vertices[i] - Vertices[i - 1]).Length();

                if (t >= prevLength / totalLength && t <= accumLength / totalLength)
                {
                    float tPrev = prevLength / totalLength;
                    float tThis = accumLength / totalLength;
                    float tNew = (t - tPrev) / (tThis - tPrev);

                    return GetValue(Vertices[i - 1], Vertices[i], 
                                    tNew, smooth, out angle);
                }
            }

            return GetValue(Vector2.Zero, Vector2.Zero, t, smooth, out angle);
        }

        Vector2 GetValue(Vector2 vertex1, Vector2 vertex2, float t, bool smooth, out float angle)
        {
            angle = (float)Math.Atan2(vertex2.Y - vertex1.Y, vertex2.X - vertex1.X);

            return smooth ? Vector2.SmoothStep(vertex1, vertex2, t) : 
                            Vector2.Lerp(vertex1, vertex2, t);
        }
    }
}