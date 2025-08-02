#include "stdafx.h"

#include "Node.h"

namespace DentyEngine
{
    namespace MeshResource
    {
        Node::Node() noexcept
        {
        }

        Node::Node(const NodeResource& resource) noexcept :
            _resource(resource)
        {
        }
    }
}
