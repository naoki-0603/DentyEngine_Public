using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class Color
    {
        public Color()
        {
            color = new Vector4(1.0f, 1.0f, 1.0f, 1.0f);
        }

        public Color(float r, float g, float b, float a)
        {
            color = new Vector4(r, g, b, a);
        }

        public Color(Vector4 color)
        {
            this.color = color;
        }

        public Color(Color color, bool transparent = false)
        {
            this.color = color.color;

            if (transparent)
            {
                this.color.w = 0.0f;
            }
        }

        //
        // Static variables.
        //
        public static Color BLACK
        {
            get
            {
                return new Color(0, 0, 0, 1);
            }
        }

        public static Color WHITE
        {
            get
            {
                return new Color(1, 1, 1, 1);
            }
        }

        public static Color RED
        {
            get
            {
                return new Color(1, 0, 0, 1);
            }
        }

        public static Color GREEN
        {
            get
            {
                return new Color(0, 1, 0, 1);
            }
        }

        public static Color BLUE
        {
            get
            {
                return new Color(0, 0, 1, 1);
            }
        }

        public static Color YELLOW
        {
            get
            {
                return new Color(1, 0.92f, 0.016f, 1);
            }
        }

        public static Color ORANGE
        {
            get
            {
                return new Color(0.952f, 0.59f, 0.0f, 1.0f);
            }
        }

        public static Color PINK
        {
            get
            {
                return new Color(1.0f, 0.75f, 0.79f, 1.0f);
            }
        }

        //
        // Member variables.
        //
        public Vector4 color;
    }
}