#pragma once

namespace DentyEngine
{
	struct FrameDataSpecification
	{
		FrameDataSpecification(std::string_view name, float milliseconds, float fps) :
			name(name), milliseconds(milliseconds), fps(fps) {}

		std::string name;

		float milliseconds;
		float fps;
	};

	struct FunctionDataSpecification
	{
		FunctionDataSpecification(std::string_view name, float milliseconds) :
			name(name), milliseconds(milliseconds) {}

		std::string name;

		float milliseconds;
	};

	struct RenderingDataSpecification
	{
		RenderingDataSpecification(std::string_view name, float milliseconds) :
			name(name), milliseconds(milliseconds) {}

		std::string name;

		float milliseconds;
	};

	//
	// This measured data use in EditorProfilerWindow class.
	//
	class MeasuredData final
	{
		using FrameStates = std::unordered_map<std::string, FrameDataSpecification>;
		using FunctionStates = std::unordered_map<std::string, FunctionDataSpecification>;
		using RenderingState = std::unordered_map<std::string, RenderingDataSpecification>;
	public:
		DEFINE_SINGLETON_CLASS(MeasuredData)

		void AddFrameState(const FrameDataSpecification&);
		void AddFunctionState(const FunctionDataSpecification&);
		void AddRenderingState(const RenderingDataSpecification&);

		// Call after Timer::OnUpdate()
		void Clear();

		//
		// Getter
		//
		[[nodiscard]]
		const FrameStates& GetFrameStates() const { return _frameStates; }

		[[nodiscard]]
		const FunctionStates& GetFunctionStates() const { return _functionStates; }

		[[nodiscard]]
		const RenderingState& GetRenderingState() const { return _renderingState; }
	private:
		FrameStates _frameStates;
		FunctionStates _functionStates;
		RenderingState _renderingState;
	};
}