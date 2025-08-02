#pragma once

namespace DentyEngine
{
	class Tag final
	{
	public:
		// For serialize
		Tag() = default;
		Tag(const std::string& tag);
		Tag(const Tag&) = default;
		~Tag() = default;

		const std::string& operator()() const
		{
			return tag;
		}

		[[nodiscard]]
		static std::optional<uint32_t> GetTagNumberFrom(std::string_view tag);
	public:
		std::string tag;

	    static const std::array<std::string, 12> TAGS;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] uint32_t version) const
		{
			archive(
				CEREAL_NVP(tag)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] uint32_t version)
		{
			archive(
				CEREAL_NVP(tag)
			);
		}
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Tag, 1u)