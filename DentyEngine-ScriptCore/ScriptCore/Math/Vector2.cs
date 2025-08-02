using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector2
    {
        // Member functions.
        public Vector2() { x = 0; y = 0; }
        
        public Vector2(float x, float y)
        {
            this.x = x; 
            this.y = y;
        }

        public Vector2(float scalar)
        {
            this.x = scalar;
            this.y = scalar;
        }

        public float LengthSquared()
        {
            return x * x + y * y;
        }

        public float Length()
        {
            return (float)Math.Sqrt(LengthSquared());
        }

        public new string ToString()
        {
            string text = "";
            text += x.ToString();
            text += ", ";
            text += y.ToString();

            return text;
        }

        // Member operators.
        public static Vector2 operator +(Vector2 v1, Vector2 v2)
        {
            return new Vector2(v1.x + v2.x, v1.y + v2.y);
        }

        public static Vector2 operator -(Vector2 v1, Vector2 v2)
        {
            return new Vector2(v1.x - v2.x, v1.y - v2.y);
        }

        public static Vector2 operator *(Vector2 v, float scalar)
        {
            return new Vector2(v.x * scalar, v.y * scalar);
        }

        // Member static functions.
        public static Vector2 Zero => new Vector2(0.0f, 0.0f);

        // Member variables.
        public float x, y;
    }
}
