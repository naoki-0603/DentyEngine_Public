#pragma once

#include "AssetPhotoTaker.h"
#include "System/Core/Singleton.h"

namespace DentyEngine
{
	class Asset;
	class Texture2D;

	//
	// Called in Editor content browser.
	//
	class AssetPhotoStadium final : public DesignPattern::Singleton<AssetPhotoStadium>
	{
		friend class Singleton;
	public:
		~AssetPhotoStadium() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(AssetPhotoStadium)

		// Take photo.
		[[nodiscard]]
		Ref<Texture2D> TakePhoto(const Asset* asset) const;
	private:
		AssetPhotoStadium();
	};
}
