#pragma once

#include <DirectXMath.h>

#include "VectorI.h"

// Reference https://qiita.com/Reputeless/items/96226cfe1282a014b147

namespace DentyEngine
{
	class Matrix;

	namespace Utils
	{
		[[nodiscard]]
		constexpr extern float ClampAngle(float angle)
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

		[[nodiscard]]
		constexpr bool ApproxEqual(float lhs, float rhs, const float epsilon = FLT_EPSILON) noexcept
		{
			return (lhs >= rhs - epsilon && lhs <= rhs + epsilon);
		}

		template <class T>
		[[nodiscard]]
		constexpr T RoundOffFractions(T value, uint32_t digit)
		{
			value *= static_cast<T>(digit);
			value = std::floor(value) / static_cast<T>(digit);

			return value;
		}

		template <class T>
		[[nodiscard]]
		constexpr T Round(T value, uint32_t digit)
		{
			value *= static_cast<T>(digit);
			value = std::roundf(value) / static_cast<T>(digit);

			return value;
		}
	}

	// Provides Vector (x, y)
	class Vector2 final : public DirectX::XMFLOAT2
	{
	public:
		constexpr Vector2() : XMFLOAT2(0, 0) {}
		constexpr Vector2(float x, float y) : XMFLOAT2(x, y) {}

		//
		// Member functions.
		//

		[[nodiscard]]
		float Length() const
		{
			const float length = std::sqrt(this->x * this->x + this->y * this->y);
			return length;
		}

		[[nodiscard]]
		constexpr float LengthSq() const
		{
			const float lengthSq = Dot(*this);
			return lengthSq;
		}

		[[nodiscard]]
		Vector2 Normalize() const
		{
			const float length = Length();
			if (length <= 0.0f) 
				DENTY_ASSERT(length, "length value is zero.");

			Vector2 v = { x, y };

			v.x /= length;
			v.y /= length;

			return v;
		}

		[[nodiscard]]
		constexpr Vector2 Inverse() const
		{
			return Vector2(this->x * -1.0f, this->y * -1.0f);
		}

		[[nodiscard]]
		constexpr float Dot(const Vector2& rhs) const
		{
			return (this->x * rhs.x + this->y * rhs.y);
		}

		// TODO Make constexpr.
		[[nodiscard]]
		Vector2 Cross(const Vector2& rhs) const
		{
			const DirectX::XMVECTOR Lhs = DirectX::XMLoadFloat2(this);
			const DirectX::XMVECTOR Rhs = DirectX::XMLoadFloat2(&rhs);
			const DirectX::XMVECTOR Cross = DirectX::XMVector2Cross(Lhs, Rhs);

			Vector2 cross = {};
			DirectX::XMStoreFloat2(&cross, Cross);

			return cross;
		}

		[[nodiscard]]
		constexpr Vector2 Absolute() const
		{
			return Vector2(fabsf(x), fabsf(y));
		}

		[[nodiscard]]
		constexpr Vector2I ToInt() const
		{
			return Vector2I(static_cast<int32_t>(x), static_cast<int32_t>(y));
		}

		[[nodiscard]]
		constexpr bool IsZero() const
		{
			return (this->x == 0.0f && this->y == 0.0f);
		}

		[[nodiscard]]
		constexpr bool IsContainMinus() const
		{
			return (this->x < 0.0f || this->y < 0.0f);
		}

		[[nodiscard]]
		float DistanceFrom(const Vector2& target) const
		{
			return (target - *this).Length();
		}

		[[nodiscard]]
		DirectX::XMVECTOR GetXM() const
		{
			return XMLoadFloat2(this);
		}

		// Operators
		[[nodiscard]]
		constexpr Vector2 operator+(const Vector2& r) const
		{
			return Vector2(x + r.x, y + r.y);
		}

		[[nodiscard]]
		constexpr Vector2 operator-(const Vector2& r) const
		{
			return Vector2(x - r.x, y - r.y);
		}

		[[nodiscard]]
		constexpr Vector2 operator*(const Vector2& r) const
		{
			return Vector2(x * r.x, y * r.y);
		}

		[[nodiscard]]
		constexpr Vector2 operator/(const Vector2& r) const
		{
			return Vector2(x / r.x, y / r.y);
		}

		[[nodiscard]]
		constexpr Vector2 operator+(const float r) const
		{
			return Vector2(x + r, y + r);
		}

		[[nodiscard]]
		constexpr Vector2 operator-(const float r) const
		{
			return Vector2(x - r, y - r);
		}

		[[nodiscard]]
		constexpr Vector2 operator*(const float r) const
		{
			return Vector2(x * r, y * r);
		}

		[[nodiscard]]
		constexpr Vector2 operator/(const float r) const
		{
			return Vector2(x / r, y / r);
		}

		Vector2& operator+=(const Vector2& r)
		{
			this->x += r.x;
			this->y += r.y;

			return *this;
		}

		Vector2& operator-=(const Vector2& r)
		{
			this->x -= r.x;
			this->y -= r.y;

			return *this;
		}

		Vector2& operator*=(const Vector2& r)
		{
			this->x *= r.x;
			this->y *= r.y;

			return *this;
		}

		Vector2& operator/=(const Vector2& r)
		{
			this->x /= r.x;
			this->y /= r.y;

			return *this;
		}

		Vector2& operator+=(const float r)
		{
			this->x += r;
			this->y += r;

			return *this;
		}

		Vector2& operator-=(const float r)
		{
			this->x -= r;
			this->y -= r;

			return *this;
		}

		Vector2& operator*=(const float r)
		{
			this->x *= r;
			this->y *= r;

			return *this;
		}

		Vector2& operator/=(const float r)
		{
			this->x /= r;
			this->y /= r;

			return *this;
		}

		[[nodiscard]]
		bool operator!=(const Vector2& r) const
		{
			int rx = static_cast<int>(r.x);
			int ry = static_cast<int>(r.y);

			return (static_cast<int32_t>(x) != rx or static_cast<int32_t>(y) != ry);
		}

		//
		// Converter
		//
#if DENTY_USE_IMGUI
		[[nodiscard]]
		constexpr ImVec2 ToImVec2() const
		{
			const auto v = ImVec2(x, y);

			return v;
		}

		[[nodiscard]]
		static Vector2 ToVector2(const ImVec2& imVec2)
		{
			const auto v = Vector2(imVec2.x, imVec2.y);

			return v;
		}
#endif
#if DENTY_USE_FBX_SDK
		[[nodiscard]]
		static Vector2 ToVector2(const FbxVector2& fbxVector2)
		{
			const auto v = Vector2(static_cast<float>(fbxVector2[0]), static_cast<float>(fbxVector2[1]));

			return v;
		}
#endif
#if DENTY_USE_ASSIMP
		[[nodiscard]]
		 aiVector2D ToAiVector2() const
		{
			const auto v = aiVector2D(x, y);

			return v;
		}

		[[nodiscard]]
		static Vector2 ToVector2(const aiVector2D& aiVector2D)
		{
			const auto v = Vector2(aiVector2D.x, aiVector2D.y);

			return v;
		}
#endif
	private:
		friend class cereal::access;

		// Serialize
		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT2>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT2>(this)
			);
		}
	};

	/// Provides Vector (x, y, z)
	class Vector3 final : public DirectX::XMFLOAT3
	{
	public:
		constexpr Vector3() : DirectX::XMFLOAT3() {}
		constexpr Vector3(const Vector2I& xy, float z) : DirectX::XMFLOAT3(static_cast<float>(xy.x), static_cast<float>(xy.y), z) {}
		constexpr Vector3(float x, float y, float z) : DirectX::XMFLOAT3(x, y, z) {}

		explicit constexpr Vector3(const Vector2& v, float z) : DirectX::XMFLOAT3(v.x, v.y, z) {}

		Vector3(const DirectX::XMVECTOR& V) : DirectX::XMFLOAT3() { DirectX::XMStoreFloat3(this, V); }

		//
		// Member functions.
		//

		[[nodiscard]]
		float Length() const
		{
			const float length = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
			return length;
		}

		[[nodiscard]]
		constexpr float LengthSq() const
		{
			const float lengthSq = Dot(*this);
			return lengthSq;
		}

		[[nodiscard]]
		Vector3 Normalize() const
		{
			const float length = Length();
			if (length <= 0.0f) 
				DENTY_ASSERT(length, "length value is zero.");

			Vector3 v = { x, y, z };

			v.x /= length;
			v.y /= length;
			v.z /= length;

			return v;
		}

		[[nodiscard]]
		constexpr Vector3 Inverse() const
		{
			return Vector3(this->x * -1.0f, this->y * -1.0f, this->z * -1.0f);
		}

		[[nodiscard]]
		constexpr float Dot(const Vector3& rhs) const
		{
			return (this->x * rhs.x + this->y * rhs.y + this->z * rhs.z);
		}

		// TODO Make constexpr.
		[[nodiscard]]
		Vector3 Cross(const Vector3& rhs) const
		{
			const DirectX::XMVECTOR Lhs = DirectX::XMLoadFloat3(this);
			const DirectX::XMVECTOR Rhs = DirectX::XMLoadFloat3(&rhs);
			const DirectX::XMVECTOR Cross = DirectX::XMVector3Cross(Lhs, Rhs);

			Vector3 cross = {};
			DirectX::XMStoreFloat3(&cross, Cross);

			return cross;
		}

		[[nodiscard]]
		constexpr Vector3 ToRadian() const
		{
			Vector3 radians = { };

			radians.x = DirectX::XMConvertToRadians(x);
			radians.y = DirectX::XMConvertToRadians(y);
			radians.z = DirectX::XMConvertToRadians(z);

			return radians;
		}

		[[nodiscard]]
		constexpr Vector3 ToDegree() const
		{
			Vector3 degrees = {};

			degrees.x = DirectX::XMConvertToDegrees(x);
			degrees.y = DirectX::XMConvertToDegrees(y);
			degrees.z = DirectX::XMConvertToDegrees(z);

			return degrees;
		}

		void ClampAngles()
		{
			x = Utils::ClampAngle(x);
			y = Utils::ClampAngle(y);
			z = Utils::ClampAngle(z);
		}

		void SetZeroIsAlmostZero()
		{
			if (IsAlmostZero())
			{
				x = y = z = 0.0f;
			}
		}

		[[nodiscard]]
		constexpr bool IsZero() const
		{
			return (this->x == 0.0f && this->y == 0.0f && this->z == 0.0f);
		}

		[[nodiscard]]
		constexpr bool IsAlmostZero() const
		{
			if (!Utils::ApproxEqual(fabsf(x), 0.0f))
			{
				return false;
			}

			if (!Utils::ApproxEqual(fabsf(y), 0.0f))
			{
				return false;
			}

			if (!Utils::ApproxEqual(fabsf(z), 0.0f))
			{
				return false;
			}

			return true;
		}

		[[nodiscard]]
		float DistanceFrom(const Vector3& target) const
		{
			return (target - *this).Length();
		}

		[[nodiscard]]
		DirectX::XMVECTOR GetXM() const
		{
			return DirectX::XMLoadFloat3(this);
		}

		// Operators
		[[nodiscard]]
		constexpr Vector3 operator+(const Vector3& r) const
		{
			return Vector3(x + r.x, y + r.y, z + r.z);
		}

		[[nodiscard]]
		constexpr Vector3 operator-(const Vector3& r) const
		{
			return Vector3(x - r.x, y - r.y, z - r.z);
		}

		[[nodiscard]]
		constexpr Vector3 operator*(const Vector3& r) const
		{
			return Vector3(x * r.x, y * r.y, z * r.z);
		}

		[[nodiscard]]
		constexpr Vector3 operator/(const Vector3& r) const
		{
			return Vector3(x / r.x, y / r.y, z / r.z);
		}

		[[nodiscard]]
		constexpr Vector3 operator+(const float r) const
		{
			return Vector3(x + r, y + r, z + r);
		}

		[[nodiscard]]
		constexpr Vector3 operator-(const float r) const
		{
			return Vector3(x - r, y - r, z - r);
		}

		[[nodiscard]]
		constexpr Vector3 operator*(const float r) const
		{
			return Vector3(x * r, y * r, z * r);
		}

		[[nodiscard]]
		constexpr Vector3 operator/(const float r) const
		{
			return Vector3(x / r, y / r, z / r);
		}

		[[nodiscard]]
		constexpr bool operator!=(const Vector3& r) const
		{
			return ((x != r.x) || (y != r.y) || (z != r.z));
		}

		Vector3& operator+=(const Vector3& r)
		{
			this->x += r.x;
			this->y += r.y;
			this->z += r.z;

			return *this;
		}

		Vector3& operator-=(const Vector3& r)
		{
			this->x -= r.x;
			this->y -= r.y;
			this->z -= r.z;

			return *this;
		}

		Vector3& operator*=(const Vector3& r)
		{
			this->x *= r.x;
			this->y *= r.y;
			this->z *= r.z;

			return *this;
		}

		Vector3& operator/=(const Vector3& r)
		{
			this->x /= r.x;
			this->y /= r.y;
			this->z /= r.z;

			return *this;
		}

		Vector3& operator+=(const float r)
		{
			this->x += r;
			this->y += r;
			this->z += r;

			return *this;
		}

		Vector3& operator-=(const float r)
		{
			this->x -= r;
			this->y -= r;
			this->z -= r;

			return *this;
		}

		Vector3& operator*=(const float r)
		{
			this->x *= r;
			this->y *= r;
			this->z *= r;

			return *this;
		}

		Vector3& operator/=(const float r)
		{
			this->x /= r;
			this->y /= r;
			this->z /= r;

			return *this;
		}

		//
		// Converter
		//
#if DENTY_USE_BULLET_PHYSICS
		[[nodiscard]]
		btVector3 ToBtVector3() const
		{
			const auto v = btVector3(x, y, z);

			return v;
		}

		[[nodiscard]]
		static Vector3 ToVector3(const btVector3& btVector3)
		{
			const auto v = Vector3( btVector3.x(), btVector3.y(), btVector3.z() );

			return v;
		}
#endif
#if DENTY_USE_FBX_SDK
		[[nodiscard]]
		static Vector3 ToVector3(const FbxDouble3& fbxDouble3)
		{
			const auto v = Vector3(fbxDouble3[0], fbxDouble3[1], fbxDouble3[2]);

			return v;
		}
#endif
#if DENTY_USE_ASSIMP
		[[nodiscard]]
		 aiVector3D ToAiVector3() const
		{
			const auto v = aiVector3D(x, y, z);

			return v;
		}

		[[nodiscard]]
		static Vector3 ToVector3(const aiVector3D& aiVector3D)
		{
			const auto v = Vector3(aiVector3D.x, aiVector3D.y, aiVector3D.z);

			return v;
		}

		[[nodiscard]]
		static Vector3 ToVector3(const aiColor3D& aiColor3D)
		{
		    const auto v = Vector3(aiColor3D.r, aiColor3D.g, aiColor3D.b);

			return v;
		}
#endif

		[[nodiscard]]
		static Vector3 WorldToScreen(const Vector3& worldPosition,
			float viewportX, float viewportY,
			float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
			const Matrix* view, const Matrix* projection);

		[[nodiscard]]
		static Vector3 ScreenToWorld(const Vector3& screenPosition,
			float viewportX, float viewportY,
			float viewportWidth, float viewportHeight, float viewportMinZ, float viewportMaxZ,
			const Matrix* view, const Matrix* projection
		);

		[[nodiscard]]
		static constexpr Vector3 Zero() { return Vector3(0, 0, 0); }

		[[nodiscard]]
		static constexpr Vector3 Right() { return Vector3(1, 0, 0); }

		[[nodiscard]]
		static constexpr Vector3 Up() { return Vector3(0, 1, 0); }

		[[nodiscard]]
		static constexpr Vector3 Down() { return Vector3(0, -1, 0); }

		[[nodiscard]]
		static constexpr Vector3 Front() { return Vector3(0, 0, 1); }
	private:
		friend class cereal::access;

		// Serialize
		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT3>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT3>(this)
			);
		}
	};

	/// Provides Vector (x, y, z, w)
	class Vector4 final : public DirectX::XMFLOAT4
	{
	public:
		constexpr Vector4() : DirectX::XMFLOAT4() {}
		constexpr Vector4(float x, float y, float z, float w) : DirectX::XMFLOAT4(x, y, z, w) {}
		constexpr explicit Vector4(const Vector3& xyz, const float w) : Vector4(xyz.x, xyz.y, xyz.z, w) {}

		//
		// Member functions.
		//

		[[nodiscard]]
		float Length() const
		{
			const float length = std::sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w);
			return length;
		}

		[[nodiscard]]
		constexpr float LengthSq() const
		{
			const float lengthSq = Dot(*this);
			return lengthSq;
		}

		[[nodiscard]]
		Vector4 Normalize() const
		{
			const float length = Length();
			if (length <= 0.0f) 
				DENTY_ASSERT(length, "length value is zero.");

			Vector4 v = { x, y, z, w };

			v.x /= length;
			v.y /= length;
			v.z /= length;
			v.w /= length;

			return v;
		}

		[[nodiscard]]
		constexpr Vector4 Inverse() const
		{
			return Vector4(this->x * -1.0f, this->y * -1.0f, this->z * -1.0f, this->w * -1.0f);
		}

		[[nodiscard]]
		constexpr float Dot(const Vector4& rhs) const
		{
			return (this->x * rhs.x + this->y * rhs.y + this->z * rhs.z + this->w * rhs.w);
		}

		[[nodiscard]]
		constexpr bool IsZero() const
		{
			return (this->x == 0.0f && this->y == 0.0f && this->z == 0.0f);
		}

		[[nodiscard]]
		bool IsApproxEqual(const Vector4& target) const noexcept
		{
			if (!Utils::ApproxEqual(x, target.x))
				return false;

			if (!Utils::ApproxEqual(y, target.y))
				return false;

			if (!Utils::ApproxEqual(z, target.z))
				return false;

			if (!Utils::ApproxEqual(w, target.w))
				return false;

			return true;
		}

		[[nodiscard]]
		float DistanceFrom(const Vector4& target) const
		{
			return (target - *this).Length();
		}

		[[nodiscard]]
		DirectX::XMVECTOR GetXM() const
		{
			return DirectX::XMLoadFloat4(this);
		}

		// Operators

		[[nodiscard]]
		constexpr Vector4 operator+(const Vector4& r) const
		{
			return Vector4(x + r.x, y + r.y, z + r.z, w + r.w);
		}

		[[nodiscard]]
		constexpr Vector4 operator-(const Vector4& r) const
		{
			return Vector4(x - r.x, y - r.y, z - r.z, w - r.w);
		}

		[[nodiscard]]
		constexpr Vector4 operator*(const Vector4& r) const
		{
			return Vector4(x * r.x, y * r.y, z * r.z, w * r.w);
		}

		[[nodiscard]]
		constexpr Vector4 operator/(const Vector4& r) const
		{
			return Vector4(x / r.x, y / r.y, z / r.z, w / r.w);
		}

		[[nodiscard]]
		constexpr Vector4 operator+(const float r) const
		{
			return Vector4(x + r, y + r, z + r, w + r);
		}

		[[nodiscard]]
		constexpr Vector4 operator-(const float r) const
		{
			return Vector4(x - r, y - r, z - r, w - r);
		}

		[[nodiscard]]
		constexpr Vector4 operator*(const float r) const
		{
			return Vector4(x * r, y * r, z * r, w * r);
		}

		[[nodiscard]]
		constexpr Vector4 operator/(const float r) const
		{
			return Vector4(x / r, y / r, z / r, w / r);
		}

		Vector4& operator+=(const Vector4& r)
		{
			this->x += r.x;
			this->y += r.y;
			this->z += r.z;
			this->w += r.w;

			return *this;
		}

		Vector4& operator-=(const Vector4& r)
		{
			this->x -= r.x;
			this->y -= r.y;
			this->z -= r.z;
			this->w -= r.w;

			return *this;
		}

		Vector4& operator*=(const Vector4& r)
		{
			this->x *= r.x;
			this->y *= r.y;
			this->z *= r.z;
			this->w *= r.w;

			return *this;
		}

		Vector4& operator/=(const Vector4& r)
		{
			this->x /= r.x;
			this->y /= r.y;
			this->z /= r.z;
			this->w /= r.w;

			return *this;
		}

		Vector4& operator+=(const float r)
		{
			this->x += r;
			this->y += r;
			this->z += r;
			this->w += r;

			return *this;
		}

		Vector4& operator-=(const float r)
		{
			this->x -= r;
			this->y -= r;
			this->z -= r;
			this->w -= r;

			return *this;
		}

		Vector4& operator*=(const float r)
		{
			this->x *= r;
			this->y *= r;
			this->z *= r;
			this->w *= r;

			return *this;
		}

		Vector4& operator/=(const float r)
		{
			this->x /= r;
			this->y /= r;
			this->z /= r;
			this->w /= r;

			return *this;
		}

		// Return x, y, z components.
		[[nodiscard]]
		constexpr Vector3 GetAsVector3() const
		{
			return Vector3(x, y, z);
		}

		//
		// Converter
		//
#if DENTY_USE_FBX_SDK
		[[nodiscard]]
		static Vector4 ToVector4(const FbxVector4& fbxVector4)
		{
			const auto v = Vector4(static_cast<float>(fbxVector4[0]), static_cast<float>(fbxVector4[1]), 
				static_cast<float>(fbxVector4[2]), static_cast<float>(fbxVector4[3])
			);

			return v;
		}
#endif
#if DENTY_USE_IMGUI
		[[nodiscard]]
		constexpr ImVec4 ToImVec4() const
		{
			const auto v = ImVec4(x, y, z, w);

			return v;
		}
#endif

#if DENTY_USE_ASSIMP
		[[nodiscard]]
		 aiColor4D ToAiVector4() const
		{
			const auto v = aiColor4D(x, y, z, w);

			return v;
		}

		[[nodiscard]]
		static Vector4 ToVector4(const aiColor4D& aiColor4D)
		{
			const auto v = Vector4(
				aiColor4D.r, aiColor4D.g, aiColor4D.b, aiColor4D.a
			);

			return v;
		}
#endif

	private:
		friend class cereal::access;

		// Serialize
		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4>(this)
			);
		}
	};

	// Vector2 Define out-of-class operators.
	inline Vector2 operator*(const float x, const Vector2& r)
	{
		return Vector2(x * r.x, x * r.y);
	}

	inline Vector2 operator/(const float x, const Vector2& r)
	{
		return Vector2(x / r.x, x / r.y);
	}

	template <class Char>
	inline std::basic_ofstream<Char>& operator<<(std::basic_ofstream<Char>& os, const Vector2& r)
	{
		return os << Char('(') << r.x << Char(',') << r.y << Char(')');
	}

	// Vector3 Define out-of-class operators.
	inline Vector3 operator*(const float x, const Vector3& r)
	{
		return Vector3(x * r.x, x * r.y, x * r.z);
	}

	inline Vector3 operator/(const float x, const Vector3& r)
	{
		return Vector3(x / r.x, x / r.y, x / r.z);
	}

	template <class Char>
	inline std::basic_ofstream<Char>& operator<<(std::basic_ofstream<Char>& os, const Vector3& r)
	{
		return os << Char('(') << r.x << Char(',') << r.y << Char(',') << r.z << Char(')');
	}

	// Vector4 Define out-of-class operators.
	inline Vector4 operator*(const float x, const Vector4& r)
	{
		return Vector4(x * r.x, x * r.y, x * r.z, x * r.w);
	}

	inline Vector4 operator/(const float x, const Vector4& r)
	{
		return Vector4(x / r.x, x / r.y, x / r.z, x / r.w);
	}

	template <class Char>
	inline std::basic_ofstream<Char>& operator<<(std::basic_ofstream<Char>& os, const Vector4& r)
	{
		return os << Char('(') << r.x << Char(',') << r.y << Char(',') << r.z << Char(',') << r.w << Char(')');
	}
};

// Serialize
namespace DirectX
{
    template <class Archive>
	void serialize(Archive& archive, XMFLOAT2& xmfloat2, [[maybe_unused]] const uint32_t version)
    {
        archive(
		    CEREAL_NVP(xmfloat2.x),
			CEREAL_NVP(xmfloat2.y)
		);
    }

	template <class Archive>
	void serialize(Archive& archive, XMFLOAT3& xmfloat3, [[maybe_unused]] const uint32_t version)
    {
        archive(
		    CEREAL_NVP(xmfloat3.x),
			CEREAL_NVP(xmfloat3.y),
			CEREAL_NVP(xmfloat3.z)
		);
    }

	template <class Archive>
	void serialize(Archive& archive, XMFLOAT4& xmfloat4, [[maybe_unused]] const uint32_t version)
    {
        archive(
		    CEREAL_NVP(xmfloat4.x),
			CEREAL_NVP(xmfloat4.y),
			CEREAL_NVP(xmfloat4.z),
			CEREAL_NVP(xmfloat4.w)
		);
    }
};

CEREAL_CLASS_VERSION(DentyEngine::Vector2, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Vector3, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Vector4, 1u)

CEREAL_CLASS_VERSION(DirectX::XMFLOAT2, 1u)
CEREAL_CLASS_VERSION(DirectX::XMFLOAT3, 1u)
CEREAL_CLASS_VERSION(DirectX::XMFLOAT4, 1u)

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(DentyEngine::Vector2, cereal::specialization::member_load_save)
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(DentyEngine::Vector3, cereal::specialization::member_load_save)
CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(DentyEngine::Vector4, cereal::specialization::member_load_save)