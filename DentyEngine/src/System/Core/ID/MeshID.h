#pragma once

namespace DentyEngine
{
	class MeshID final
	{
	public:
		MeshID();
		explicit MeshID(const uint64_t meshID);
		MeshID(const MeshID& meshID) = default;
		~MeshID() = default;

		void ReCalculate();

		operator uint64_t() const { return _meshID; }
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(_meshID)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(_meshID)
			);
		}
	private:
		uint64_t _meshID;
	};
}

namespace std
{
	template <typename T> struct hash;

	template <>
	struct hash<DentyEngine::MeshID>
	{
		std::size_t operator()(const DentyEngine::MeshID& uuid) const
		{
			return static_cast<size_t>(uuid);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::MeshID, 1u)