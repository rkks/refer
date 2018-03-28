using Microsoft.Xna.Framework.Input.Touch;

namespace Petzold.Phone.Xna
{
    public interface IProcessTouch
    {
        bool ProcessTouch(TouchLocation touch);
    }
}
