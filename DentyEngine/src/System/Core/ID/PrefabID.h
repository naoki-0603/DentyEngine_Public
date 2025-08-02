#pragma once

namespace DentyEngine
{
	class PrefabID final
	{
	public:
		PrefabID();
		explicit PrefabID(const uint64_t prefabID);
		PrefabID(const PrefabID& prefabID) = default;
		~PrefabID() = default;

		void ReCalculate();

		operator uint64_t() const { return _prefabID; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_prefabID)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_prefabID)
			);
		}
	private:
		uint64_t _prefabID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template <>
	struct hash<DentyEngine::PrefabID>
	{
		std::size_t operator()(const DentyEngine::PrefabID& uuid) const
		{
			return static_cast<size_t>(uuid);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::PrefabID, 1u)