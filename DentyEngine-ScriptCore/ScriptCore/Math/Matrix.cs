using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public sealed class Matrix
    {
        public Matrix() 
        {
            _11 = Identity._11;
            _12 = Identity._12;
            _13 = Identity._13;
            _14 = Identity._14;

            _21 = Identity._21;
            _22 = Identity._22;
            _23 = Identity._23;
            _24 = Identity._24;

            _31 = Identity._31;
            _32 = Identity._32;
            _33 = Identity._33;
            _34 = Identity._34;

            _41 = Identity._41;
            _42 = Identity._42;
            _43 = Identity._43;
            _44 = Identity._44;
        }

        public Matrix(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44
            )
        {
            this._11 = _11; this._12 = _12; this._31 = _13; this._41 = _14;
            this._21 = _21; this._22 = _22; this._32 = _23; this._42 = _24;
            this._31 = _31; this._32 = _32; this._33 = _33; this._43 = _34;
            this._41 = _41; this._42 = _42; this._34 = _43; this._44 = _44;
        }

        public Matrix(Vector3 right, Vector3 up, Vector3 front, Vector3 offset)
        {
            _11 = right.x; _12 = right.y; _13 = right.z; _14 = 0.0f;
            _21 = up.x; _22 = up.y; _23 = right.z; _24 = 0.0f;
            _31 = front.x; _32 = front.y; _33 = front.z; _34 = 0.0f;
            _41 = offset.x; _42 = offset.y; _43 = offset.z; _44 = 1.0f;
        }

        // Copy constructor
        public Matrix(Matrix source)
        {
            _11 = source._11;
            _12 = source._12;
            _13 = source._13;
            _14 = source._14;

            _21 = source._21;
            _22 = source._22;
            _23 = source._23;
            _24 = source._24;
            
            _31 = source._31;
            _32 = source._32;
            _33 = source._33;
            _34 = source._34;
            
            _41 = source._41;
            _42 = source._42;
            _43 = source._43;
            _44 = source._44;
        }

        public Matrix Transpose()
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_Transpose(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                ref result._11, ref result._12, ref result._13, ref result._14,
                ref result._21, ref result._22, ref result._23, ref result._24,
                ref result._31, ref result._32, ref result._33, ref result._34,
                ref result._41, ref result._42, ref result._43, ref result._44
            );

            return result;
        }

        public Matrix Inverse()
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_Inverse(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                ref result._11, ref result._12, ref result._13, ref result._14,
                ref result._21, ref result._22, ref result._23, ref result._24,
                ref result._31, ref result._32, ref result._33, ref result._34,
                ref result._41, ref result._42, ref result._43, ref result._44
            );

            return result;
        }

        public Vector3 TransformCoord(Vector3 vec)
        {
            Vector3 result = new Vector3();
            InternalCalls.Matrix_TransformCoord(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                vec.x, vec.y, vec.z,
                out result.x, out result.y, out result.z
            );

            return result;
        }

        // Ignore offset components.
        public Vector3 TransformNormal(Vector3 vec)
        {
            Vector3 result = new Vector3();
            InternalCalls.Matrix_TransformNormal(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                vec.x, vec.y, vec.z,
                out result.x, out result.y, out result.z
            );

            return result;
        }

        public Vector3 ObtainEulerAngles()
        {
            Vector3 eulerAngles = new Vector3();

            // Unit is radian.
            InternalCalls.Matrix_ObtainEulerAngles(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                out eulerAngles.x, out eulerAngles.y, out eulerAngles.z
            );

            return eulerAngles;
        }

        public Vector3 ObtainScale()
        {
            Vector3 scale = new Vector3();

            InternalCalls.Matrix_ObtainScale(
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44,

                out scale.x, out scale.y, out scale.z
            );

            return scale;
        }
        
        public Vector3 Right
        {
            get
            {
                return new Vector3(_11, _12, _13);
            }

            set
            {
                _11 = value.x; _12 = value.y; _13 = value.z;
            }
        }

        public Vector3 Up
        {
            get
            {
                return new Vector3(_21, _22, _23);
            }

            set
            {
                _21 = value.x; _22 = value.y; _23 = value.z;
            }
        }

        public Vector3 Front
        {
            get
            {
                return new Vector3(_31, _32, _33);
            }

            set
            {
                _31 = value.x; _32 = value.y; _33 = value.z;
            }
        }

        public Vector3 Offset
        {
            get
            {
                return new Vector3(_41, _42, _43);
            }

            set
            {
                _41 = value.x; _42 = value.y; _43 = value.z;
            }
        }

        public float[] GetAsArray()
        {
            return new float[16]
            {
                _11, _12, _13, _14,
                _21, _22, _23, _24,
                _31, _32, _33, _34,
                _41, _42, _43, _44
            };
        }
       
        // Operators.
        public static Matrix operator*(Matrix m1, Matrix m2)
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_Multiply(
                m1._11, m1._12, m1._13, m1._14,
                m1._21, m1._22, m1._23, m1._24,
                m1._31, m1._32, m1._33, m1._34,
                m1._41, m1._42, m1._43, m1._44,

                m2._11, m2._12, m2._13, m2._14,
                m2._21, m2._22, m2._23, m2._24,
                m2._31, m2._32, m2._33, m2._34,
                m2._41, m2._42, m2._43, m2._44,

                ref result._11, ref result._12, ref result._13, ref result._14,
                ref result._21, ref result._22, ref result._23, ref result._24,
                ref result._31, ref result._32, ref result._33, ref result._34,
                ref result._41, ref result._42, ref result._43, ref result._44
            );

            return result;
        }

        public static Matrix RotationAxis(Vector3 axis, float radian)
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_RotationAxis(
                out result._11, out result._12, out result._13, out result._14,
                out result._21, out result._22, out result._23, out result._24,
                out result._31, out result._32, out result._33, out result._34,
                out result._41, out result._42, out result._43, out result._44,

                axis.x, axis.y, axis.z, radian
            );

            return result;
        }

        public static Matrix RotationQuaternion(Quaternion quaternion)
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_RotationQuaternion(
                out result._11, out result._12, out result._13, out result._14,
                out result._21, out result._22, out result._23, out result._24,
                out result._31, out result._32, out result._33, out result._34,
                out result._41, out result._42, out result._43, out result._44,

                quaternion.x, quaternion.y, quaternion.z, quaternion.w
            );

            return result;
        }

        public static Matrix RotationRollPitchYaw(float roll, float pitch, float yaw)
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_RotationRollPitchYaw(
                out result._11, out result._12, out result._13, out result._14,
                out result._21, out result._22, out result._23, out result._24,
                out result._31, out result._32, out result._33, out result._34,
                out result._41, out result._42, out result._43, out result._44,

                roll, pitch, yaw
            );

            return result;
        }

        public static Matrix Scaling(float x, float y, float z)
        {
            Matrix result = new Matrix();

            InternalCalls.Matrix_Scaling(
                out result._11, out result._12, out result._13, out result._14,
                out result._21, out result._22, out result._23, out result._24,
                out result._31, out result._32, out result._33, out result._34,
                out result._41, out result._42, out result._43, out result._44,

                x, y, z
            );

            return result;
        }

        public static Matrix Translation(float x, float y, float z)
        {
            Matrix translation = new Matrix();

            translation._41 = x;
            translation._42 = y;
            translation._43 = z;

            return translation;
        }

        public static Matrix ComputeOrientationFromFront(Vector3 front)
        {
            Matrix result = new Matrix();

            front.Normalized();
            
            Vector3 tempRight = front.Cross(Vector3.Up);
            tempRight.Normalized();

            Vector3 up = tempRight.Cross(front);
            up.Normalized();

            Vector3 right = up.Cross(front);
            right.Normalized();

            result.Right = right;
            result.Up = up;
            result.Front = front;

            return result;
        }

        public static Matrix Identity => new Matrix(Vector3.Right, Vector3.Up, Vector3.Front, Vector3.Zero);

        // Member values
        public float
            _11, _12, _13, _14,
            _21, _22, _23, _24,
            _31, _32, _33, _34,
            _41, _42, _43, _44;
    }
}