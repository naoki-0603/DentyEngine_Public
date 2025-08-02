#pragma once

namespace DentyEngine
{
	// Provides quaternion
	class Quaternion final : public DirectX::XMFLOAT4
	{
	public:
		constexpr Quaternion() : DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) {}
		constexpr Quaternion(const float x, const float y, const float z, const float w) : DirectX::XMFLOAT4(x, y, z, w) {}

		constexpr explicit Quaternion(const Vector4& v) : DirectX::XMFLOAT4(v.x, v.y, v.z, v.w) {}

		Quaternion(const DirectX::XMVECTOR& V) : DirectX::XMFLOAT4(0, 0, 0, 1) { XMStoreFloat4(this, V); }
		
		[[nodiscard]]
		Matrix ConvertToMatrix() const
		{
			const DirectX::XMVECTOR Q = DirectX::XMLoadFloat4(this);
			const DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(Q);

			Matrix r = {};
			DirectX::XMStoreFloat4x4(&r, R);

			return r;
		}

		[[nodiscard]]
		Quaternion RotationAxis(const Vector3& axis, const float angle) const
		{
			const DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationAxis(
				DirectX::XMLoadFloat3(&axis), angle
			);

			Quaternion result = { };
			DirectX::XMStoreFloat4(&result, Q);

			return result;
		}

		[[nodiscard]]
		Quaternion Multiply(const Quaternion& q) const
		{
			DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(this);
			DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q);

			DirectX::XMVECTOR Q = DirectX::XMQuaternionMultiply(Q0, Q1);

			Quaternion result = { };
			DirectX::XMStoreFloat4(&result, Q);

			return result;
		}

		[[nodiscard]]
		Quaternion Slerp(const Quaternion& q, float factor) const
		{
			DirectX::XMVECTOR Q0 = DirectX::XMLoadFloat4(this);
			DirectX::XMVECTOR Q1 = DirectX::XMLoadFloat4(&q);

			DirectX::XMVECTOR Q = DirectX::XMQuaternionSlerp(Q0, Q1, factor);

			Quaternion result = { };
			DirectX::XMStoreFloat4(&result, Q);

			return result;
		}

		// https://www.geometrictools.com/Documentation/EulerAngles.pdf
		[[nodiscard]]
		Vector3 ObtainEulerAngles() const
		{
			const Matrix rotationMatrix = ConvertToMatrix();

			return rotationMatrix.ObtainEulerAngles();
		}

		[[nodiscard]]
		DirectX::XMVECTOR GetXM() const
		{
			Quaternion q = { x, y, z, w };
			DirectX::XMVECTOR Q = { };
			Q = XMLoadFloat4(&q);

			return Q;
		}

		[[nodiscard]]
		Quaternion operator*(const Quaternion& rhs)
		{
			using namespace DirectX;

			Quaternion result = { };
			XMStoreFloat4(&result, XMQuaternionMultiply(this->GetXM(), rhs.GetXM()));

			return result;
		}

		static Quaternion Identity() 
		{
			return IDENTITY;
		}

		[[nodiscard]]
		static Quaternion RotationRollPitchYaw(float roll, float pitch, float yaw)
		{
			const DirectX::XMVECTOR Q = DirectX::XMQuaternionRotationRollPitchYaw(
				roll, pitch, yaw
			);

			Quaternion result = { };
			DirectX::XMStoreFloat4(&result, Q);

			return result;
		}

		//
		// Converter
		//
#if DENTY_USE_BULLET_PHYSICS
		// Left-handed
		[[nodiscard]]
		static Quaternion ToQuaternion(const btQuaternion& quaternion)
		{
			const auto q = Quaternion(
				-quaternion.getX(), quaternion.getY(), quaternion.getZ(), -quaternion.getW()
			);

			return q;
		}
#endif

#if DENTY_USE_ASSIMP
		[[nodiscard]]
		 aiQuaternion ToAiQuaternion() const
		{
			const auto q = aiQuaternion(w, x, y, z);

			return q;
		}

		[[nodiscard]]
		static Quaternion ToQuaternion(const aiQuaternion& aiQuaternion)
		{
			const auto q = Quaternion(
				aiQuaternion.x, aiQuaternion.y, aiQuaternion.z, aiQuaternion.w
			);

			return q;
		}
#endif

	private:
		friend class cereal::access;

		// Serialize
		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4>(this)
			);
		}

	private:
		static const Quaternion IDENTITY;
	};
};

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(DentyEngine::Quaternion, cereal::specialization::member_load_save)
