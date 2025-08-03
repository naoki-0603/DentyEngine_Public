#pragma once

#include "System/Core/Object.h"
#include "System/Core/Tag.h"

#include "Component/Transform.h"

#include "System/Resource/Prefab.h"

namespace DentyEngine
{
	class Component;

	class PrefabPreviewObject final : public Object, public std::enable_shared_from_this<PrefabPreviewObject>
	{
		friend class PrefabEditor;
		friend class Prefab;
	public:
		PrefabPreviewObject();
		~PrefabPreviewObject() override = default;

		void Create(std::string_view name, const Tag& tag, const std::vector<Ref<Component>>& components, bool enabled, bool isStatic);

		void ShowGui();

		void Copy(PrefabGameObject& gameObject) const;

		[[nodiscard]]
		bool HasComponent(std::string_view componentName) const;
	private:
		void AddBuildInComponent(std::string_view componentName);

	private:
		std::vector<Ref<Component>> _components;

		Ref<Component> _reservedRemoveComponent;

		Transform _transform;

		Tag _tag;

		bool _enabled;
		bool _static;

		bool _pressedAddComponentButton;
	};
}
