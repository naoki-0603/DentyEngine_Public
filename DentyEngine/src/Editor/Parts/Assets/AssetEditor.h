#pragma once

namespace DentyEngine
{
	class AssetEditor
	{
	public:
		AssetEditor() : _open() {}
		virtual ~AssetEditor() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(AssetEditor)

		[[nodiscard]]
		virtual bool AssetExists() const = 0;

		//
		// Getter
		//
		bool IsOpen() const { return _open; }
	protected:

		bool _open;
	};
}