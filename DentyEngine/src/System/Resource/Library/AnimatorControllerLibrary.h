#pragma once

#include "System/Core/DFilePath.h"
#include "System/Core/DString.h"

#include "System/Core/ID/AnimatorControllerID.h"

#include "System/Resource/ILibrary.h"

namespace DentyEngine
{
	class AnimatorController;

	class AnimatorControllerLibrary final : public ILibrary
	{
	public:
		AnimatorControllerLibrary();
		~AnimatorControllerLibrary() override = default;

		void Initialize() override;
		void Finalize() override;
		void Update() override;

		void OnGui() override;
		void OnEvent(Event* e) override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		void Add(const Ref<AnimatorController>& controller);

		void Remove(const FilePath& filePath);

		[[nodiscard]]
		std::optional<Ref<AnimatorController>> Find(const FilePath& filePath) const;

		[[nodiscard]]
		bool Reload(const FilePath& path) override;
	private:
		void Reload(AnimatorControllerID id);

		[[nodiscard]]
		bool IsAnimatorControllerFile(const String& path) const;
	private:
		using AnimatorControllers = std::unordered_map<FilePath, Ref<AnimatorController>, FilePath::Hash>;

		AnimatorControllers _caches;
	};
}
