using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DentyEngine
{
    public class Perspective
    {
        public Perspective()
        {
            fovAngleY = 0.0f;
            aspectRatio = 0.0f;
            nearZ = 0.0f;
            farZ = 0.0f;
            distance = 0.0f;

            up = Vector3.Zero;
        }

        public float fovAngleY;
        public float aspectRatio;
        public float nearZ, farZ;
        public float distance;

        public Vector3 up;
    }

    public class Orthographic
    {
        public Orthographic()
        {
            viewSize = Vector2.Zero;
            nearZ = 0.0f;
            farZ = 0.0f;
            distance = 0.0f;
            aspectRatio = 0.0f;

            up = Vector3.Zero;
        }

        public Vector2 viewSize;
        public float nearZ, farZ;
        public float distance;
        public float aspectRatio;

        public Vector3 up;
    }

    public class CameraConstants
    {
        public CameraConstants()
        {
            viewProjection = Matrix.Identity;
            view = Matrix.Identity;
            projection = Matrix.Identity;
            inverseView = view.Inverse();
            inverseProjection = projection.Inverse();

            position = Vector4.Zero;
        }

        public Matrix viewProjection;
        public Matrix view;
        public Matrix projection;
        public Matrix inverseView;
        public Matrix inverseProjection;

        public Vector4 position;
    }
}
