#pragma once

namespace DentyEngine
{
	class SceneID final
	{
	public:
		SceneID();
		explicit SceneID(const uint64_t sceneID);
		SceneID(const SceneID& sceneID) = default;
		~SceneID() = default;

		void ReCalculate();

		operator uint64_t() const { return _sceneID; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] uint32_t version) const
		{
			archive(
				CEREAL_NVP(_sceneID)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] uint32_t version)
		{
			archive(
				CEREAL_NVP(_sceneID)
			);
		}
	private:
		uint64_t _sceneID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template <>
	struct hash<DentyEngine::SceneID>
	{
		std::size_t operator()(const DentyEngine::SceneID& uuid) const
		{
			return static_cast<size_t>(uuid);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::SceneID, 1u)