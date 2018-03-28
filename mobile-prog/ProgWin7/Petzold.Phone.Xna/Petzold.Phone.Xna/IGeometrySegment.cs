using System.Collections.Generic;

namespace Petzold.Phone.Xna
{
    public interface IGeometrySegment
    {
        void GetAllX(float y, IList<float> xCollection);
    }
}
