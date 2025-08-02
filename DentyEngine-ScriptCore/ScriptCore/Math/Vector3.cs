using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Vector3
    {
        // Member functions.
        public Vector3() { x = 0; y = 0; z = 0; }

        public Vector3(float x, float y, float z)
        {
            this.x = x; 
            this.y = y;
            this.z = z;
        }

        public Vector3(float scalar)
        {
            this.x = scalar;
            this.y = scalar;
            this.z = scalar;
        }

        public float LengthSquared()
        {
            return x * x + y * y + z * z;
        }

        public float Length() 
        {
            return (float)Math.Sqrt(LengthSquared());
        }

        public float LengthConsiderationSign()
        {
            bool isMinus = (x < 0 || y < 0 || z < 0);

            return (float)Math.Sqrt(LengthSquared()) * (isMinus ? -1.0f : 1.0f);
        }

        public float Dot(Vector3 rhs)
        {
            float dot;
            InternalCalls.Vector3_Dot(
                x, y, z,
                rhs.x, rhs.y, rhs.z,
                out dot
            );

            return dot;
        }
        
        public float AngleBetweenNormals(Vector3 rhs)
        {
            float angle;
            InternalCalls.Vector3_AngleBetweenNormals(
                x, y, z,
                rhs.x, rhs.y, rhs.z,
                out angle
            );

            return angle;
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

            return text;
        }

        public Vector3 Cross(Vector3 rhs)
        {
            Vector3 result = new Vector3();

            InternalCalls.Vector3_Cross(
                x, y, z,
                rhs.x, rhs.y, rhs.z,
                out result.x, out result.y, out result.z
            );

            return result;
        }

        // Member operators.
        public static Vector3 operator+(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
        }

        public static Vector3 operator -(Vector3 v1, Vector3 v2)
        {
            return new Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
        }

        public static Vector3 operator *(Vector3 v, float scalar)
        {
            return new Vector3(v.x * scalar, v.y * scalar, v.z * scalar);
        }

        public static Vector3 operator/(Vector3 v, float scalar)
        {
            return new Vector3(v.x / scalar, v.y * scalar, v.z / scalar);
        }

        // Member static functions.
        public static Vector3 Zero => new Vector3(0.0f, 0.0f, 0.0f);

        public static Vector3 Right => new Vector3(1.0f, 0.0f, 0.0f);

        public static Vector3 Up => new Vector3(0.0f, 1.0f, 0.0f);
        
        public static Vector3 Front => new Vector3(0.0f, 0.0f, 1.0f);

        // Member variables.
        public float x, y, z;
    }
}
