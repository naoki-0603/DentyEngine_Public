using System;
using System.CodeDom;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Quaternion
    {
        public Quaternion() 
        { 
            x = 0.0f;
            y = 0.0f;
            z = 0.0f;
            w = 1.0f;
        }
        
        public Quaternion(Vector4 vec)
        {
            x = vec.x;
            y = vec.y;
            z = vec.z;
            w = vec.w;
        }

        public Matrix ToMatrix()
        {
            Matrix result = new Matrix();

            InternalCalls.Quaternion_ToMatrix(
                x, y, z, w,

                out result._11, out result._12, out result._13, out result._14,
                out result._21, out result._22, out result._23, out result._24,
                out result._31, out result._32, out result._33, out result._34,
                out result._41, out result._42, out result._43, out result._44
            );

            return result;
        }

        public static Quaternion Slerp(Quaternion q1, Quaternion q2, float t)
        {
            Quaternion result = new Quaternion();

            InternalCalls.Quaternion_Slerp(
                q1.x, q1.y, q1.z, q1.w,
                q2.x, q2.y, q2.z, q2.w, t,

                out result.x, out result.y, out result.z, out result.w
            );

            return result;
        }

        public static Quaternion RotationAxis(Vector3 axis, float angle)
        {
            Quaternion result = new Quaternion();

            InternalCalls.Quaternion_RotationAxis(
                axis.x, axis.y, axis.z, angle,
                out result.x, out result.y, out result.z, out result.w
            );

            return result;
        }

        public static Quaternion RotationRollPitchYaw(Vector3 angle)
        {
            Quaternion result = new Quaternion();

            InternalCalls.Quaternion_RollPitchYaw(
                angle.x, angle.y, angle.z,
                out result.x, out result.y, out result.z, out result.w
            );

            return result;
        }

        public static Quaternion operator*(Quaternion q1, Quaternion q2)
        {
            Quaternion result = new Quaternion();

            InternalCalls.Quaternion_Multiply(
                q1.x, q1.y, q1.z, q1.w,
                q2.x, q2.y, q2.z, q2.w,

                out result.x, out result.y, out result.z, out result.w
            );

            return result;
        }

        // Member values.
        public float x, y, z, w;
    }
}