#pragma once

#include "System/Core/DFilePath.h"

namespace DentyEngine
{
	class Event;
	class EventOnSceneSaved;
	class EventOnScenePlayButtonPressed;

	class Asset;

	class DENTY_NOVTABLE ILibrary
	{
	public:
		ILibrary() = default;
		virtual ~ILibrary() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(ILibrary)

		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void OnGui() = 0;
		virtual void OnEvent(Event* e) = 0;
		virtual void Finalize() = 0;

		virtual void OnSerialized() const = 0;
		virtual void OnDeserialized() = 0;

		[[nodiscard]]
		virtual bool Reload(const FilePath& path) = 0;
	};
};