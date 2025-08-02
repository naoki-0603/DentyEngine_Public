#pragma once

#include "IProfiler.h"

namespace DentyEngine
{
	struct FrameStateSpecification
	{
		FrameStateSpecification(std::string_view name) :
			startPoint(), name(name) {}

		LARGE_INTEGER startPoint;

		std::string name;
	};

	class FrameState final : public IProfiler
	{
	public:
		explicit FrameState(const FrameStateSpecification&);
		~FrameState() override;

		DELETE_COPY_MOVE_CONSTRUCTORS(FrameState)

		void OnGui() override;
	private:
		void Begin() override;
		void End() override;
	private:
		FrameStateSpecification _specification;
	};
}
