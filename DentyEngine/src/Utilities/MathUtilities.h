#pragma once

namespace DentyEngine
{
    namespace MathUtils
    {
        extern bool ApproxEqual(float rhs, float lhs, float epsilon = FLT_EPSILON);

        extern float ToRadian(float degree);
        extern float ToDegree(float radian);

        // Both functions unit is radian.
        extern float ClampAngle(float angle);
        extern Vector3 ClampAngles(const Vector3& eulerAngles);

        // Convert to coordinate system.
        extern Vector3 WorldToScreen(const Vector3& worldPosition,
            float viewportX, float viewportY,
            float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
            const Matrix& view, const Matrix& projection
        );
        extern Vector3 ScreenToWorld(const Vector3& screenPosition,
            float viewportX, float viewportY,
            float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
            const Matrix& view, const Matrix& projection
        );
    }
}