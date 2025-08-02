#include "stdafx.h"

#include "MathUtilities.h"

namespace DentyEngine
{
    namespace MathUtils
    {
        bool ApproxEqual(float rhs, float lhs, float epsilon)
        {
            return (rhs < lhs + epsilon && rhs > lhs - epsilon);
        }

        float ToRadian(float degree)
        {
            return DirectX::XMConvertToRadians(degree);
        }

        float ToDegree(float radian)
        {
            return DirectX::XMConvertToDegrees(radian);
        }

        float ClampAngle(float angle)
        {
            if (angle <= -DirectX::XM_PI)
            {
                angle += +DirectX::XM_2PI;
            }
            else if (angle >= +DirectX::XM_PI)
            {
                angle += -DirectX::XM_2PI;
            }

            return angle;
        }

        Vector3 ClampAngles(const Vector3& eulerAngles)
        {
            const float angleX = ClampAngle(eulerAngles.x);
            const float angleY = ClampAngle(eulerAngles.y);
            const float angleZ = ClampAngle(eulerAngles.z);

            return Vector3(angleX, angleY, angleZ);
        }

        Vector3 WorldToScreen(const Vector3& worldPosition,
            float viewportX, float viewportY,
            float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
            const Matrix& view, const Matrix& projection)
        {
            return DirectX::XMVector3Project(
                worldPosition.GetXM(), viewportX, viewportY,
                viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ,
                projection.GetXM(), view.GetXM(), Matrix::Identity().GetXM()
            );
        }

        Vector3 ScreenToWorld(const Vector3& screenPosition,
            float viewportX, float viewportY,
            float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
            const Matrix& view, const Matrix& projection)
        {
            return DirectX::XMVector3Unproject(
                screenPosition.GetXM(), viewportX, viewportY,
                viewportWidth, viewportHeight, viewportMinZ, viewportMaxZ,
                projection.GetXM(), view.GetXM(), Matrix::Identity().GetXM()
            );
        }
    }

}
