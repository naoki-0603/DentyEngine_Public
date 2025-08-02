#pragma once

namespace DentyEngine
{
	class Vector2I final
	{
	public:
		constexpr Vector2I() : x(), y() {}
		constexpr Vector2I(int32_t x, int32_t y) : x(x), y(y) {}

		[[nodiscard]]
		float Length() const
		{
			const float length = std::sqrtf(
				static_cast<float>(this->x) * static_cast<float>(this->x) + 
				   static_cast<float>(this->y) * static_cast<float>(this->y)
			);

			return length;
		}

		//
		// operators.
		//
		Vector2I& operator-=(const Vector2I& r)
		{
			this->x -= r.x;
			this->y -= r.y;

			return *this;
		}
	public:
		int32_t x;
		int32_t y;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y)
			);
		}
	};

	class Vector3I final
	{
	public:
		constexpr Vector3I() : x(), y(), z() {}
		constexpr Vector3I(int32_t x, int32_t y, int32_t z) :
			x(x), y(y), z(z) {}

	public:
		int32_t x, y, z;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(z)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(z)
			);
		}
	};

	class Vector4I final
	{
	public:
		constexpr Vector4I() : x(), y(), z(), w() {}
		constexpr Vector4I(int32_t x, int32_t y, int32_t z, int32_t w) :
			x(x), y(y), z(z), w(w) {}

	public:
		int32_t x, y, z, w;

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(z),
				CEREAL_NVP(w)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(x),
				CEREAL_NVP(y),
				CEREAL_NVP(z),
				CEREAL_NVP(w)
			);
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Vector2I, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Vector3I, 1u)
CEREAL_CLASS_VERSION(DentyEngine::Vector4I, 1u)
