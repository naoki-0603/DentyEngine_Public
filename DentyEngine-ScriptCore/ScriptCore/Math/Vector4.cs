using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector4
    {
        // Member functions.
        public Vector4() { x = 0; y = 0; z = 0; w = 0; }

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x; 
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public Vector4(float scalar)
        {
            this.x = scalar;
            this.y = scalar;
            this.z = scalar;
            this.w = scalar;
        }

        public float LengthSquared()
        {
            return x * x + y * y + z * z + w * w;
        }

        public float Length() 
        {
            return (float)System.Math.Sqrt(LengthSquared());
        }

        public void Normalized()
        {
            float length = Length();

            x /= length;
            y /= length;
            z /= length;
        }

        public new string ToString()
        {
            string text = "";
            text += x.ToString();
            text += ", ";
            text += y.ToString();
            text += ", ";
            text += z.ToString();
            text += ", ";
            text += w.ToString();

            return text;
        }

        // Member operators.
        public static Vector4 operator +(Vector4 v1, Vector4 v2)
        {
            return new Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
        }

        public static Vector4 operator -(Vector4 v1, Vector4 v2)
        {
            return new Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
        }

        // Member static functions.
        public static Vector4 Zero => new Vector4(0.0f, 0.0f, 0.0f, 0.0f);

        // Member variables.
        public float x, y, z, w;
    }
}
