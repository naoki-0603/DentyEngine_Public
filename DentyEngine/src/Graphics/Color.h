#pragma once

namespace DentyEngine
{
	// https://docs.unity3d.com/ja/2018.4/ScriptReference/Color.html
	class Color final
	{
	public:
		Color();
		Color(float r, float g, float b, float a);
		Color(const Vector4& color);
		~Color() = default;

		[[nodiscard]]
		bool IsApproxEqual(const Color& target) const noexcept;

	public:
		[[nodiscard]]
		static Color Lerp(const Color& a, const Color& b, float t);
	public:
		Vector4 color;

		// C++17
		// Color constants.
		static constexpr Vector4 BLACK = Vector4(0, 0, 0, 1);
		static constexpr Vector4 BLUE = Vector4(0, 0, 1, 1);
		static constexpr Vector4 CYAN = Vector4(0, 1, 1, 1);
		static constexpr Vector4 GRAY = Vector4(0.5f, 0.5f, 0.5f, 1);
		static constexpr Vector4 GREEN = Vector4(0, 1, 0, 1);
		static constexpr Vector4 MAGENTA = Vector4(1, 0, 1, 1);
		static constexpr Vector4 RED = Vector4(1, 0, 0, 1);
		static constexpr Vector4 WHITE = Vector4(1, 1, 1, 1);
		static constexpr Vector4 YELLOW = Vector4(1, 0.92f, 0.016f, 1);
		static constexpr Vector4 CLEAR = Vector4(0, 0, 0, 0);
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(color)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(color)
			);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::Color, 1u)