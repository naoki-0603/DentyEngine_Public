#pragma once

#include "Behavior.h"

namespace DentyEngine
{
	class ComponentGenerator final
	{
	public:
		ComponentGenerator();
		~ComponentGenerator() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ComponentGenerator)

		static void AddBuildIn(const Ref<Component>& component);

		// Show gui component list.
		static void ShowComponentList(_Out_ std::string& componentName, _Out_ bool& isPressAddComponentButton, _Out_ bool& isAddBuildInComponent);

		//
		// Component generate function.
		//
		template <class ComponentType>
		static Ref<ComponentType> Generate()
		{
			const auto& component = std::make_shared<ComponentType>();

			return component;
		}

		template <class ComponentType>
		static void AddBuildInGenerator(std::string_view componentName)
		{
			_buildInComponentGenerators.emplace(componentName, Generate<ComponentType>);
		}

		static Ref<Component> FindBuildInComponent(std::string_view componentName);

		static const std::vector<std::string>& GetBuildInComponentNames() { return _buildInComponentNames; }
		static const std::vector<std::string>& GetMonoComponentNames() { return _monoComponentNames; }
	private:
		static std::vector<std::string> _buildInComponentNames;
		static std::vector<std::string> _monoComponentNames;

		static std::unordered_map<std::string, std::function<Ref<Component>()>> _buildInComponentGenerators;
		//static std::unordered_map<std::string, std::function<Ref<MonoBehaviour>()>> _monoBehaviourGenerators;
	};

#define DENTY_REGISTER_MONO_COMPONENT(MonoComponent) \
	static void RegisterMonoComponent() \
	{ \
		Ref<MonoComponent> component = std::make_shared<MonoComponent>(); \
		ComponentGenerator::AddMono(component); \
		ComponentGenerator::AddMonoBehaviourGenerator<MonoComponent>(component->GetName()); \
		DENTY_SET_INFO_COLOR \
		DENTY_INFO_LOG << "MonoComponent " << component->GetName().c_str() << " Added!"; \
	}

#define DENTY_REGISTER_BUILD_IN_COMPONENT(BuildInComponent) \
	static void RegisterBuildInComponent() \
	{ \
		Ref<BuildInComponent> component = std::make_shared<BuildInComponent>(); \
		ComponentGenerator::AddBuildIn(component); \
		ComponentGenerator::AddBuildInGenerator<BuildInComponent>(component->GetName()); \
		DENTY_SET_INFO_COLOR \
		DENTY_INFO_LOG << "BuildInComponent " << component->GetName().c_str() << " Added!"; \
	}
}
