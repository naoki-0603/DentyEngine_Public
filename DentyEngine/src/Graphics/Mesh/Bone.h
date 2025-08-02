#pragma once

namespace DentyEngine
{
    class MeshImporter;

    namespace MeshResource
    {
        class Skeleton;

        struct BoneResource
        {
        public:
            BoneResource() : name(), parentBoneName(), offsetTransforms(), localTransform(), localToGlobal() {}

            std::string name;

            // If doesn't exists, set "NoParent".
            std::string parentBoneName;

            // Each meshes.
            std::vector<Matrix> offsetTransforms;

            // Local transform.
        	Matrix localTransform;

            // Convert to local space to global space.
            Matrix localToGlobal;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(name),
                        CEREAL_NVP(parentBoneName),
                        CEREAL_NVP(offsetTransforms),
                        CEREAL_NVP(localTransform),
                        CEREAL_NVP(localToGlobal)
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
                        CEREAL_NVP(parentBoneName),
                        CEREAL_NVP(offsetTransforms),
                        CEREAL_NVP(localTransform),
                        CEREAL_NVP(localToGlobal)
                    );    
                }
            }
        };

        class Bone final
        {
            friend class MeshImporter;
            friend class Skeleton;
        public:
            // For serialize.
            Bone() noexcept;
            explicit Bone(const BoneResource&) noexcept;

            //
            // Getter
            //
        	[[nodiscard]]
            const BoneResource& GetResource() const { return _resource; }
        private:
            BoneResource _resource;

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

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::BoneResource, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::Bone, 1u)
