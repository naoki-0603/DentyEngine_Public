#pragma once

#include "System/Core/DString.h"

#include "System/Resource/ILibrary.h"
#include "System/Scripting/ScriptField.h"

namespace DentyEngine
{
    class ScriptObject;
    class ScriptClass;
    class Script;

	class ScriptLibrary final : public ILibrary
	{
	public:
		ScriptLibrary();
		~ScriptLibrary() override = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ScriptLibrary)

		void Initialize() override;
		void Update() override;
		void OnGui() override;
		void OnEvent(Event* e) override;
		void Finalize() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void ReloadAt(
			const FilePath& targetScriptFilePathToSerialized, 
			const std::map<std::string, ScriptField>& newScriptFields
		);

		void Add(Ref<Script> script);
		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<Script>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;

		[[nodiscard]]
		bool IsScriptFile(const String& path) const;

		[[nodiscard]]
		bool Exists(const FilePath& filePath);
	private:
		std::unordered_map<FilePath, Ref<Script>, FilePath::Hash> _caches;
	};
}
