#pragma once

#include <DirectXMath.h>

namespace DentyEngine
{
	class Quaternion;

	// Provides matrix (4x4)
	class Matrix final : public DirectX::XMFLOAT4X4
	{
	public:
		constexpr Matrix() : DirectX::XMFLOAT4X4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		) {}

		constexpr Matrix(
			float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44
		) : DirectX::XMFLOAT4X4(
			_11, _12, _13, _14,
			_21, _22, _23, _24,
			_31, _32, _33, _34,
			_41, _42, _43, _44
		) {}

		constexpr explicit Matrix(float m[4][4]) : DirectX::XMFLOAT4X4(
			m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
		) {}

		constexpr explicit Matrix(float m[16]) : DirectX::XMFLOAT4X4(
			m[0], m[1], m[2], m[3],
			m[4], m[5], m[6], m[7],
			m[8], m[9], m[10], m[11],
			m[12], m[13], m[14], m[15]
		) {}

		constexpr explicit Matrix(const DirectX::XMFLOAT4X4& m) : DirectX::XMFLOAT4X4(
			m
		) {}

		explicit Matrix(const std::array<float, 16>& array) : DirectX::XMFLOAT4X4(
			array.data()
		) {}

		explicit Matrix(const Vector3& right, const Vector3& up, const Vector3& front) : DirectX::XMFLOAT4X4(
			right.x, right.y, right.z, 0.0f,
			up.x, up.y, up.z, 0.0f,
			front.x, front.y, front.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		) {}

		Matrix(const Vector3& right, const Vector3& up, const Vector3& front, const Vector3& offset) : DirectX::XMFLOAT4X4(
			right.x, right.y, right.z, 0.0f,
			up.x, up.y, up.z, 0.0f,
			front.x, front.y, front.z, 0.0f,
			offset.x, offset.y, offset.z, 1.0f
		) {}

		Matrix(const DirectX::XMMATRIX& M)
		{
			DirectX::XMFLOAT4X4 matrix = {};
			DirectX::XMStoreFloat4x4(this, M);
		}

		//
		// Operators.
		//
		[[nodiscard]]
		Matrix operator*(const Matrix& r) const
		{
			DirectX::XMMATRIX R = DirectX::XMMatrixMultiply(this->GetXM(), r.GetXM());
			DirectX::XMFLOAT4X4 result = {};
			DirectX::XMStoreFloat4x4(&result, R);

			return Matrix(result);
		}

		[[nodiscard]]
		Matrix Inverse() const
		{
			const DirectX::XMMATRIX M = this->GetXM();
			const DirectX::XMMATRIX I = DirectX::XMMatrixInverse(nullptr, M);

			Matrix inverse = {};
			DirectX::XMStoreFloat4x4(&inverse, I);

			return inverse;
		}

		[[nodiscard]]
		Matrix Transpose() const
		{
			const Matrix transpose = DirectX::XMMatrixTranspose(this->GetXM());
			return transpose;
		}

		/**
		 * @fn Identity
		 * @brief 単位行列を取得する
		 * @return Matrix
		 */
		[[nodiscard]]
		constexpr static Matrix Identity()
		{
			return IDENTITY;
		}

		void SetRightVec3(const Vector3& right)
		{
			_11 = right.x;
			_12 = right.y;
			_13 = right.z;
		}

		void SetUpVec3(const Vector3& up)
		{
			_21 = up.x;
			_22 = up.y;
			_23 = up.z;
		}

		void SetFrontVec3(const Vector3& front)
		{
			_31 = front.x;
			_32 = front.y;
			_33 = front.z;
		}

		void SetOffsetVec3(const Vector3& offset)
		{
			_41 = offset.x;
			_42 = offset.y;
			_43 = offset.z;
		}

		/**
		 * @fn GetXM
		 * @brief Convert to XMMATRIX
		 * @return DirectX::XMMATRIX
		 */
		[[nodiscard]]
		DirectX::XMMATRIX GetXM() const
		{
			DirectX::XMMATRIX M = DirectX::XMLoadFloat4x4(this);
			return M;
		}

		[[nodiscard]]
		constexpr Vector3 GetRightVec3() const
		{
			Vector3 right = { _11, _12, _13 };
			return right;
		}

		[[nodiscard]]
		constexpr Vector3 GetUpVec3() const
		{
			Vector3 up = { _21, _22, _23 };
			return up;
		}

		[[nodiscard]]
		constexpr Vector3 GetFrontVec3() const
		{
			Vector3 front = { _31, _32, _33 };
			return front;
		}

		[[nodiscard]]
		constexpr Vector3 GetOffsetVec3() const
		{
			Vector3 offset = { _41, _42, _43 };
			return offset;
		}

		/**
		* @fn GetRight
		* @brief 一行目を取得する
		* @return Vector4
		*/
		[[nodiscard]]
		constexpr Vector4 GetRight() const
		{
			Vector4 right = { _11, _12, _13, _14 };
			return right;
		}

		/**
		 * @fn GetUp
		 * @brief 二行目を取得する
		 * @return Vector4
		 */
		[[nodiscard]]
		constexpr Vector4 GetUp() const
		{
			Vector4 up = { _21, _22, _23, _24 };
			return up;
		}

		/**
		* @fn GetFront
		* @brief 三行目を取得する
		* @return Vector4
		*/
		[[nodiscard]]
		constexpr Vector4 GetFront() const
		{
			Vector4 front = { _31, _32, _33, _34 };

			return front;
		}

		/**
		 * @fn GetOffset
		 * @brief 四行目を取得する
		 * @return Vector4
		 */
		[[nodiscard]]
		constexpr Vector4 GetOffset() const
		{
			Vector4 offset = { _41, _42, _43, _44 };

			return offset;
		}

		/**
		 * @fn GetRightXM
		 * @brief 一行目を取得する
		 * @return XMVECTOR
		 */
		[[nodiscard]]
		DirectX::XMVECTOR GetRightXM() const
		{
			Vector4 right = { _11, _12, _13, _14 };
			DirectX::XMVECTOR Right = DirectX::XMLoadFloat4(&right);

			return Right;
		}

		/**
		 * @fn GetUpXM
		 * @brief 二行目を取得する
		 * @return XMVECTOR
		 */
		[[nodiscard]]
		DirectX::XMVECTOR GetUpXM() const
		{
			Vector4 up = { _21, _22, _23, _24 };
			DirectX::XMVECTOR Up = DirectX::XMLoadFloat4(&up);

			return Up;
		}

		/**
		 * @fn GetFrontXM
		 * @brief 三行目を取得する
		 * @return XMVECTOR
		 */
		[[nodiscard]]
		DirectX::XMVECTOR GetFrontXM() const
		{
			Vector4 front = { _31, _32, _33, _34 };
			DirectX::XMVECTOR Front = DirectX::XMLoadFloat4(&front);

			return Front;
		}

		/**
		 * @fn GetOffsetXM
		 * @brief 四行目を取得する
		 * @return XMVECTOR
		 */
		[[nodiscard]]
		DirectX::XMVECTOR GetOffsetXM() const
		{
			Vector4 offset = { _41, _42, _43, _44 };
			DirectX::XMVECTOR Offset = DirectX::XMLoadFloat4(&offset);

			return Offset;
		}

		[[nodiscard]]
		constexpr float* GetAsFloatPointer() const
		{
			std::array<float, 16> matrix = {};
			matrix.at(0) = _11;
			matrix.at(1) = _12;
			matrix.at(2) = _13;
			matrix.at(3) = _14;

			matrix.at(4) = _21;
			matrix.at(5) = _22;
			matrix.at(6) = _23;
			matrix.at(7) = _24;
			
			matrix.at(8) = _31;
			matrix.at(9) = _32;
			matrix.at(10) = _33;
			matrix.at(11) = _34;

			matrix.at(12) = _41;
			matrix.at(13) = _42;
			matrix.at(14) = _43;
			matrix.at(15) = _44;

			return matrix.data();
		}

		// http://marupeke296.sakura.ne.jp/DXG_No39_WorldMatrixInformation.html
		[[nodiscard]]
		Vector3 ObtainScale() const
		{
			float x = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(_11, _12, _13, 0.0f)));
			float y = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(_21, _22, _23, 0.0f)));
			float z = DirectX::XMVectorGetX(DirectX::XMVector3Length(DirectX::XMVectorSet(_31, _32, _33, 0.0f)));

			return Vector3(x, y, z);
		}

		// https://www.geometrictools.com/Documentation/EulerAngles.pdf
		[[nodiscard]]
		Vector3 ObtainEulerAngles() const
		{
			// 11, 12, 13, 14 (r00, r01, r02, r03)
			// 21, 22, 23, 24 (r10, r11, r12, r13)
			// 31, 32, 33, 34 (r20, r21, r22, r23)
			// 41, 42, 43, 44 (r30, r31, r32, r33)

			const float r21 = _32;

			float thetaX = 0.0f;
			float thetaZ = 0.0f;
			float thetaY = 0.0f;

			if (r21 < +1.0f)
			{
				if (r21 > -1.0f)
				{
					thetaX = std::asinf(r21);
					thetaZ = std::atan2f(-_12, _22);
					thetaY = std::atan2f(-_31, _33);
				}
				else
				{
					thetaX = -DirectX::XM_PIDIV2;
					thetaZ = -std::atan2f(_13, _11);
					thetaY = 0.0f;
				}
			}
			else
			{
				thetaX = DirectX::XM_PIDIV2;
				thetaZ = std::atan2f(_13, _11);
				thetaY = 0.0f;
			}

			return Vector3(thetaX, thetaY, thetaZ);
		}

		[[nodiscard]]
		Vector3 TransformCoord(const Vector3& value) const
		{
			return DirectX::XMVector3TransformCoord(value.GetXM(), this->GetXM());
		}

		[[nodiscard]]
		Matrix ObtainWithoutScaling() const
		{
			Vector3 scale = ObtainScale();

			Matrix W =
			{
				_11 / scale.x, _12 / scale.x, _13 / scale.x, 0.0f,
				_21 / scale.y, _22 / scale.y, _23 / scale.y, 0.0f,
				_31 / scale.z, _32 / scale.z, _33 / scale.z, 0.0f,
				_41, _42, _43, 1.0f
			};

			return W;
		}

		[[nodiscard]]
		Matrix ObtainWithoutRotation() const
		{
			Vector3 scale = ObtainScale();

			Matrix S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
			Matrix T = DirectX::XMMatrixTranslation(_41, _42, _43);

			return S * T;
		}

		[[nodiscard]]
		Matrix ObtainWithoutTranslation() const
		{
			Matrix world = *this;
			world.SetOffsetVec3(Vector3::Zero());

			return world;
		}

		//
		// Converter
		//

#if DENTY_USE_BULLET_PHYSICS
		// Right-handed
		[[nodiscard]]
		btTransform ToBtTransform() const
		{
			btTransform btTransform = {};
			btTransform.setIdentity();

			const btMatrix3x3 basis = 
			{
				GetRightVec3().ToBtVector3(),
				GetUpVec3().ToBtVector3(),
				GetFrontVec3().ToBtVector3()
			};
			const btVector3 origin = GetOffsetVec3().ToBtVector3();

			btTransform.setBasis(basis);
			btTransform.setOrigin(origin);

			return btTransform;
		}

		// Right-handed
		[[nodiscard]]
		btTransform ToBtTransformWithoutScaling() const
		{
			btTransform btTransform = {};
			btTransform.setIdentity();

			Matrix withoutScaling = ObtainWithoutScaling().Transpose();

			const btMatrix3x3 basis =
			{
				withoutScaling.GetRightVec3().ToBtVector3(),
				withoutScaling.GetUpVec3().ToBtVector3(),
				withoutScaling.GetFrontVec3().ToBtVector3()
			};
			const btVector3 origin = GetOffsetVec3().ToBtVector3();

			btTransform.setBasis(basis);
			btTransform.setOrigin(origin);

			return btTransform;
		}

		// Left-handed
		[[nodiscard]]
		static Matrix ToMatrix(const btMatrix3x3& btMatrix3X3)
		{
			const btMatrix3x3 transposedMatrix = btMatrix3X3.transpose();
			
			const auto matrix = Matrix(
				Vector3::ToVector3(transposedMatrix.getRow(0)),
				Vector3::ToVector3(transposedMatrix.getRow(1)),
				Vector3::ToVector3(transposedMatrix.getRow(2))
			);

			return matrix;
		}

		// Left-handed
		[[nodiscard]]
		static Matrix ToMatrix(const btTransform& btTransform)
		{
			const btMatrix3x3 basis = btTransform.getBasis();
			const auto origin = Vector3::ToVector3(btTransform.getOrigin());

			const Matrix transposedBasis = ToMatrix(basis);
			const auto transform = Matrix(
				transposedBasis.GetRightVec3(),
				transposedBasis.GetUpVec3(),
				transposedBasis.GetFrontVec3(),
				origin
			);

			return transform;
		}

		[[nodiscard]]
		static Matrix ComputeOrientationFromFront(Vector3 front)
		{
			Matrix result = {};

			front = front.Normalize();

			Vector3 tempRight = front.Cross(Vector3::Up());
			tempRight = tempRight.Normalize();

			Vector3 up = tempRight.Cross(front);
			up = up.Normalize();

			Vector3 right = up.Cross(front);
			right = right.Normalize();

			result.SetRightVec3(right);
			result.SetUpVec3(up);
			result.SetFrontVec3(front);

			return result;
		}
#endif
#if DENTY_USE_FBX_SDK
		[[nodiscard]]
		static Matrix ToMatrix(const FbxMatrix& fbxMatrix)
		{
			Matrix matrix = { };
			for (int row = 0; row < 4; ++row)
			{
				for (int column = 0; column < 4; ++column)
				{
					matrix.m[row][column] = static_cast<float>(fbxMatrix[row][column]);
				}
			}

			return matrix;
		}
#endif
#if DENTY_USE_ASSIMP
		// return transposed matrix. 
		[[nodiscard]]
		 aiMatrix4x4 ToMatrix4X4() const
		{
			const Matrix transposed = this->Transpose();

			aiMatrix4x4 aiMatrix4X4 = { };
			for (int row = 0; row < 4; ++row)
			{
				for (int column = 0; column < 4; ++column)
				{
					aiMatrix4X4[row][column] = static_cast<float>(transposed.m[row][column]);
				}
			}

			return aiMatrix4X4;
		}

		// aiMatrix4X4 should be transposed.
		[[nodiscard]]
		static Matrix ToMatrix(const aiMatrix4x4& aiMatrix4X4)
		{
			Matrix matrix = { };
			for (int row = 0; row < 4; ++row)
			{
				for (int column = 0; column < 4; ++column)
				{
					matrix.m[row][column] = static_cast<float>(aiMatrix4X4[row][column]);
				}
			}

			return matrix;
		}
#endif

	private:
		friend class cereal::access;

		// Serialize
		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4X4>(this)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<DirectX::XMFLOAT4X4>(this)
			);
		}

	private:
		static const Matrix IDENTITY;
	};
};

namespace DirectX
{
    template <class Archive>
	void serialize(Archive& archive, XMFLOAT4X4& xmfloat4X4, [[maybe_unused]] const uint32_t version)
    {
        archive(
		    CEREAL_NVP(xmfloat4X4._11),
		    CEREAL_NVP(xmfloat4X4._12),
		    CEREAL_NVP(xmfloat4X4._13),
		    CEREAL_NVP(xmfloat4X4._14),

			CEREAL_NVP(xmfloat4X4._21),
		    CEREAL_NVP(xmfloat4X4._22),
		    CEREAL_NVP(xmfloat4X4._23),
		    CEREAL_NVP(xmfloat4X4._24),

			CEREAL_NVP(xmfloat4X4._31),
		    CEREAL_NVP(xmfloat4X4._32),
		    CEREAL_NVP(xmfloat4X4._33),
		    CEREAL_NVP(xmfloat4X4._34),

			CEREAL_NVP(xmfloat4X4._41),
		    CEREAL_NVP(xmfloat4X4._42),
		    CEREAL_NVP(xmfloat4X4._43),
		    CEREAL_NVP(xmfloat4X4._44)
		);
    }
};

CEREAL_CLASS_VERSION(DirectX::XMFLOAT4X4, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Matrix, 1u)

CEREAL_SPECIALIZE_FOR_ALL_ARCHIVES(DentyEngine::Matrix, cereal::specialization::member_load_save)