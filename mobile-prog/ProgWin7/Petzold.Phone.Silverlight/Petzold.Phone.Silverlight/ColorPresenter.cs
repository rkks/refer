using System;
using System.Text;
using System.Windows.Media;

namespace Petzold.Phone.Silverlight
{
    public class ColorPresenter
    {
        static string[] colorNames = 
        {
            "AliceBlue", "AntiqueWhite", "Aqua", "Aquamarine", "Azure", 
            "Beige", "Bisque", "Black", "BlanchedAlmond", "Blue", "BlueViolet",
            "Brown", "BurlyWood", "CadetBlue", "Chartreuse", "Chocolate", 
            "Coral", "CornflowerBlue", "Cornsilk", "Crimson", "Cyan", 
            "DarkBlue", "DarkCyan", "DarkGoldenrod", "DarkGray", "DarkGreen", 
            "DarkKhaki", "DarkMagenta", "DarkOliveGreen", "DarkOrange", 
            "DarkOrchid", "DarkRed", "DarkSalmon", "DarkSeaGreen", 
            "DarkSlateBlue", "DarkSlateGray", "DarkTurquoise", "DarkViolet", 
            "DeepPink", "DeepSkyBlue", "DimGray", "DodgerBlue", "Firebrick", 
            "FloralWhite", "ForestGreen", "Fuchsia", "Gainsboro", "GhostWhite", 
            "Gold", "Goldenrod", "Gray", "Green", "GreenYellow", "Honeydew", 
            "HotPink", "IndianRed", "Indigo", "Ivory", "Khaki", "Lavender", 
            "LavenderBlush", "LawnGreen", "LemonChiffon", "LightBlue", 
            "LightCoral", "LightCyan", "LightGoldenrodYellow", "LightGray", 
            "LightGreen", "LightPink", "LightSalmon", "LightSeaGreen", 
            "LightSkyBlue", "LightSlateGray", "LightSteelBlue", "LightYellow", 
            "Lime", "LimeGreen", "Linen", "Magenta", "Maroon", 
            "MediumAquamarine", "MediumBlue", "MediumOrchid", "MediumPurple", 
            "MediumSeaGreen", "MediumSlateBlue", "MediumSpringGreen", 
            "MediumTurquoise", "MediumVioletRed", "MidnightBlue", "MintCream", 
            "MistyRose", "Moccasin", "NavajoWhite", "Navy", "OldLace", "Olive", 
            "OliveDrab", "Orange", "OrangeRed", "Orchid", "PaleGoldenrod", 
            "PaleGreen", "PaleTurquoise", "PaleVioletRed", "PapayaWhip", 
            "PeachPuff", "Peru", "Pink", "Plum", "PowderBlue", "Purple", "Red", 
            "RosyBrown", "RoyalBlue", "SaddleBrown", "Salmon", "SandyBrown", 
            "SeaGreen", "SeaShell", "Sienna", "Silver", "SkyBlue", "SlateBlue", 
            "SlateGray", "Snow", "SpringGreen", "SteelBlue", "Tan", "Teal", 
            "Thistle", "Tomato", "Turquoise", "Violet", "Wheat", "White", 
            "WhiteSmoke", "Yellow", "YellowGreen"
        };

        static uint[] uintColors = 
        {
            0xFFF0F8FF, 0xFFFAEBD7, 0xFF00FFFF, 0xFF7FFFD4, 0xFFF0FFFF, 
            0xFFF5F5DC, 0xFFFFE4C4, 0xFF000000, 0xFFFFEBCD, 0xFF0000FF, 
            0xFF8A2BE2, 0xFFA52A2A, 0xFFDEB887, 0xFF5F9EA0, 0xFF7FFF00, 
            0xFFD2691E, 0xFFFF7F50, 0xFF6495ED, 0xFFFFF8DC, 0xFFDC143C,
            0xFF00FFFF, 0xFF00008B, 0xFF008B8B, 0xFFB8860B, 0xFFA9A9A9, 
            0xFF006400, 0xFFBDB76B, 0xFF8B008B, 0xFF556B2F, 0xFFFF8C00, 
            0xFF9932CC, 0xFF8B0000, 0xFFE9967A, 0xFF8FBC8F, 0xFF483D8B, 
            0xFF2F4F4F, 0xFF00CED1, 0xFF9400D3, 0xFFFF1493, 0xFF00BFFF,
            0xFF696969, 0xFF1E90FF, 0xFFB22222, 0xFFFFFAF0, 0xFF228B22, 
            0xFFFF00FF, 0xFFDCDCDC, 0xFFF8F8FF, 0xFFFFD700, 0xFFDAA520, 
            0xFF808080, 0xFF008000, 0xFFADFF2F, 0xFFF0FFF0, 0xFFFF69B4, 
            0xFFCD5C5C, 0xFF4B0082, 0xFFFFFFF0, 0xFFF0E68C, 0xFFE6E6FA,
            0xFFFFF0F5, 0xFF7CFC00, 0xFFFFFACD, 0xFFADD8E6, 0xFFF08080, 
            0xFFE0FFFF, 0xFFFAFAD2, 0xFFD3D3D3, 0xFF90EE90, 0xFFFFB6C1, 
            0xFFFFA07A, 0xFF20B2AA, 0xFF87CEFA, 0xFF778899, 0xFFB0C4DE, 
            0xFFFFFFE0, 0xFF00FF00, 0xFF32CD32, 0xFFFAF0E6, 0xFFFF00FF,
            0xFF800000, 0xFF66CDAA, 0xFF0000CD, 0xFFBA55D3, 0xFF9370DB, 
            0xFF3CB371, 0xFF7B68EE, 0xFF00FA9A, 0xFF48D1CC, 0xFFC71585, 
            0xFF191970, 0xFFF5FFFA, 0xFFFFE4E1, 0xFFFFE4B5, 0xFFFFDEAD, 
            0xFF000080, 0xFFFDF5E6, 0xFF808000, 0xFF6B8E23, 0xFFFFA500,
            0xFFFF4500, 0xFFDA70D6, 0xFFEEE8AA, 0xFF98FB98, 0xFFAFEEEE, 
            0xFFDB7093, 0xFFFFEFD5, 0xFFFFDAB9, 0xFFCD853F, 0xFFFFC0CB, 
            0xFFDDA0DD, 0xFFB0E0E6, 0xFF800080, 0xFFFF0000, 0xFFBC8F8F, 
            0xFF4169E1, 0xFF8B4513, 0xFFFA8072, 0xFFF4A460, 0xFF2E8B57,
            0xFFFFF5EE, 0xFFA0522D, 0xFFC0C0C0, 0xFF87CEEB, 0xFF6A5ACD, 
            0xFF708090, 0xFFFFFAFA, 0xFF00FF7F, 0xFF4682B4, 0xFFD2B48C, 
            0xFF008080, 0xFFD8BFD8, 0xFFFF6347, 0xFF40E0D0, 0xFFEE82EE, 
            0xFFF5DEB3, 0xFFFFFFFF, 0xFFF5F5F5, 0xFFFFFF00, 0xFF9ACD32
        };

        // Static constructor
        static ColorPresenter()
        {
            Colors = new ColorPresenter[140];

            for (int i = 0; i < 140; i++)
            {
                // Break down the color into components
                byte A = (byte)((uintColors[i] & 0xFF000000) >> 24);
                byte R = (byte)((uintColors[i] & 0x00FF0000) >> 16);
                byte G = (byte)((uintColors[i] & 0x0000FF00) >> 8);
                byte B = (byte)((uintColors[i] & 0x000000FF) >> 0);

                // Create a display name for the color
                StringBuilder builder = new StringBuilder();

                foreach (char ch in colorNames[i])
                {
                    if (builder.Length == 0 || Char.IsLower(ch))
                        builder.Append(ch);
                    else
                    {
                        builder.Append(' ');
                        builder.Append(ch);
                    }
                }

                // Create a ColorPresenter for each color
                ColorPresenter clrPresenter = new ColorPresenter();
                clrPresenter.Color = Color.FromArgb(A, R, G, B);
                clrPresenter.Name = colorNames[i];
                clrPresenter.DisplayName = builder.ToString();
                clrPresenter.Brush = new SolidColorBrush(clrPresenter.Color);

                // Add it to the static array
                Colors[i] = clrPresenter;
            }
        }

        public static ColorPresenter[] Colors { protected set; get; }

        public Color Color { protected set; get; }

        public string Name { protected set; get; }

        public string DisplayName { protected set; get; }

        public Brush Brush { protected set; get; }

        public override string ToString()
        {
            return Name;
        }
    }
}
