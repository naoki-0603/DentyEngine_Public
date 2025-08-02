#pragma once

namespace DentyEngine
{
	class TextureID final
	{
	public:
		TextureID();
		explicit TextureID(const uint64_t textureID);
		TextureID(const TextureID& textureID) = default;
		~TextureID() = default;

		void ReCalculate();

		operator uint64_t() const { return _textureID; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_textureID)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_textureID)
			);
		}
	private:
		uint64_t _textureID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template <>
	struct hash<DentyEngine::TextureID>
	{
		std::size_t operator()(const DentyEngine::TextureID& uuid) const
		{
			return static_cast<size_t>(uuid);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::TextureID, 1u)