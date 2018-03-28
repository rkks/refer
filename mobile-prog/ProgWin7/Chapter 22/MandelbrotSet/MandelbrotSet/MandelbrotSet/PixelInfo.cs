using System;
using Microsoft.Xna.Framework;

namespace MandelbrotSet
{
    public struct PixelInfo
    {
        public static int pixelWidth;
        public static int pixelHeight;
        public static double xPixelCoordAtComplexOrigin;
        public static double yPixelCoordAtComplexOrigin;
        public static double unitsPerPixel;

        public static bool hasNewColors;
        public static int firstNewIndex;
        public static int lastNewIndex;

        public double cReal;
        public double cImag;
        public double zReal;
        public double zImag;
        public int iteration;
        public bool finished;
        public uint packedColor;

        public PixelInfo(int pixelIndex, uint[] pixels)
        {
            int x = pixelIndex % pixelWidth;
            int y = pixelIndex / pixelWidth;
            cReal = (x - xPixelCoordAtComplexOrigin) * unitsPerPixel;
            cImag = (yPixelCoordAtComplexOrigin - y) * unitsPerPixel;
            zReal = 0;
            zImag = 0;
            iteration = 0;
            finished = false;
            packedColor = pixels != null ? pixels[pixelIndex] : Color.Black.PackedValue;
        }

        public bool Iterate()
        {
            double zImagSquared = zImag * zImag;
            zImag = 2 * zReal * zImag + cImag;
            zReal = zReal * zReal - zImagSquared + cReal;

            if (zReal * zReal + zImag * zImag >= 4.0)
            {
                finished = true;
                return true;
            }
            iteration++;
            return false;
        }
    }
}
