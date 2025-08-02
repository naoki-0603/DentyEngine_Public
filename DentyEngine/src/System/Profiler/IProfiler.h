#pragma once

namespace DentyEngine
{
	class IProfiler
	{
	public:
		IProfiler() = default;
		virtual ~IProfiler() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(IProfiler)
		
		virtual void OnGui() = 0;
	protected:
		virtual void Begin() = 0;
		virtual void End() = 0;
	};
}