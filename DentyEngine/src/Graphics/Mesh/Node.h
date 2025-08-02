#pragma once

namespace DentyEngine
{
    namespace MeshResource
    {
        enum class NodeType
        {
	        None = -1,
            Node = 0,
            Mesh,
            Skeleton,
            Bone,

            Max
        };

        struct NodeResource
        {
            NodeResource() : name(), nodeIndex(), parentNodeIndex(), childNodeIndexes(), transform(), root(), hasBone(), hasSkeleton() {}

            std::string name;

            uint32_t nodeIndex;

            // -1 means there is no parent.
            int32_t parentNodeIndex;

            std::vector<uint32_t> childNodeIndexes;

            Matrix transform;

            bool root;

            // Doesn't serialize.
            bool hasBone;
            bool hasSkeleton;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(name),
                        CEREAL_NVP(nodeIndex),
                        CEREAL_NVP(parentNodeIndex),
                        CEREAL_NVP(childNodeIndexes),
                        CEREAL_NVP(transform),
                        CEREAL_NVP(root)
                    );    
                }
            }

            template <class Archive>
            void load(Archive& archive, const uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(name),
                        CEREAL_NVP(nodeIndex),
                        CEREAL_NVP(parentNodeIndex),
                        CEREAL_NVP(childNodeIndexes),
                        CEREAL_NVP(transform),
                        CEREAL_NVP(root)
                    );    
                }
            }
        };

        class Node final
        {
        public:
            // For serialize.
            Node() noexcept;
            Node(const NodeResource&) noexcept;
            Node(const Node&) = default;

        	~Node() = default;

            //
            // Operators.
            //
            Node& operator=(const Node& other)
            {
                _resource = other._resource;

                return *this;
            }

            //
            // Getter
            //
            [[nodiscard]]
            const NodeResource& GetResource() const { return _resource; }
        private:
            NodeResource _resource;

        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
            {
                archive(
                    CEREAL_NVP(_resource)
                );
            }

            template <class Archive>
            void load(Archive& archive, [[maybe_unused]] const uint32_t version) 
            {
                archive(
                    CEREAL_NVP(_resource)
                );
            }
        };
    }
}

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::NodeResource, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::Node, 1u)