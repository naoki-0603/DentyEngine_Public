#pragma once

#include "System/Core/Object.h"
#include "Asset.h"

#include "System/Scripting/ScriptField.h"

namespace DentyEngine
{
    class ScriptClass;
    class ScriptObject;

    class Script final : public Object, public Asset, public std::enable_shared_from_this<Script>
	{
	public:
		Script();
		Script(const Script& source);
		~Script() override = default;

		void Create(const FilePath& filePath);

		void OnGui();
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void OnDestroy() override;

		void OnReload() override;

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile) override;

		void Clone(const Ref<Asset>& asset) override;

		void OnScriptAssemblyReloaded(const std::map<std::string, ScriptField>& newScriptFields);

		//
		// Getter
		//
		[[nodiscard]]
		const std::map<std::string, ScriptField>& GetFields() const { return _fields; }

		[[nodiscard]]
		AssetType GetAssetType() const override { return AssetType::Script; }
	public:
		static void LoadAssetTexture();

		[[nodiscard]]
		static Ref<Texture2D> GetAssetTexture() { return _assetTexture; }
	private:
		void GenerateCSharpScript();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_fields)
			    );
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
					cereal::base_class<Object>(this),
					cereal::base_class<Asset>(this),
					CEREAL_NVP(_fields)
			    );
			}

			OnDeserialized();
		}
	private:
		std::map<std::string, ScriptField> _fields;

	private:
		static Ref<Texture2D> _assetTexture;
	};
}

CEREAL_CLASS_VERSION(DentyEngine::Script, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Script)