#include "stdafx.h"

#include "Bone.h"

namespace DentyEngine
{
	namespace MeshResource
	{
	    Bone::Bone() noexcept
        {
        }

        Bone::Bone(const BoneResource& resource) noexcept :
            _resource(resource)
        {
        } 
	}
}
