#pragma once

#include "IProfiler.h"

namespace DentyEngine
{
	struct FunctionStateSpecification
	{
		FunctionStateSpecification(std::string_view functionName) :
			startPoint(), name(functionName) {}

		LARGE_INTEGER startPoint;

		std::string name;
	};

	class FunctionState final : public IProfiler
	{
	public:
		explicit FunctionState(const FunctionStateSpecification&);
		~FunctionState() override;

		DELETE_COPY_MOVE_CONSTRUCTORS(FunctionState)

		void OnGui() override;
	private:
		void Begin() override;
		void End() override;

	private:
		FunctionStateSpecification _specification;
	};
}
