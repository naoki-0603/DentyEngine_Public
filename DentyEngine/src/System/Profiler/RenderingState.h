#pragma once

#include "IProfiler.h"

namespace DentyEngine
{
	struct RenderingStateSpecification
	{
		RenderingStateSpecification(
			std::string_view name
		) : startPoint(), meshName(name) {}

		LARGE_INTEGER startPoint;

		std::string meshName;
	};

	class RenderingState final : public IProfiler
	{
	public:
		explicit RenderingState(const RenderingStateSpecification&);
		~RenderingState() override;

		DELETE_COPY_MOVE_CONSTRUCTORS(RenderingState)
		
		void OnGui() override;
	private:
		void Begin() override;
		void End() override;
	private:
		RenderingStateSpecification _specification;
	};
}
