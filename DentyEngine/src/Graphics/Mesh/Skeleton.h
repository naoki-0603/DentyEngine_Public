#pragma once

#include "Bone.h"

#include "System/Resource/Asset.h"

#include "System/Core/Object.h"

namespace DentyEngine
{
    class MeshImporter;

    namespace MeshResource
    {
        struct SkeletonResource
        {
            SkeletonResource() : bones(), initialBones(), orientation() {}

            std::vector<Bone> bones;

            // Initial pose bones.
            std::vector<Bone> initialBones;

            Matrix orientation;
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, const uint32_t version) const
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(bones),
                        CEREAL_NVP(initialBones),
                        CEREAL_NVP(orientation)
                    );    
                }
            }

            template <class Archive>
            void load(Archive& archive, const uint32_t version)
            {
                if (version <= 1u)
                {
                    archive(
                        CEREAL_NVP(bones),
                        CEREAL_NVP(initialBones),
                        CEREAL_NVP(orientation)
                    );    
                }
            }
        };

        class Skeleton final : public Object, public Asset, public std::enable_shared_from_this<Skeleton>
        {
            friend class DentyEngine::MeshImporter;
        public:
            // For serialize.
            Skeleton() noexcept;
            Skeleton(const SkeletonResource&) noexcept;
            ~Skeleton() override = default;

            void Save(bool isUpdate = false) const override;
            void Load(const std::filesystem::path& filePath) override;

            void Rename(std::string_view newName, bool deleteOldFile) override;
            void OnReload() override;
            void Clone(const Ref<Asset>& asset) override;

            void OnSerialized() const override;
            void OnDeserialized() override;

            void OnEvent(Event* e) override;
            void OnDestroy() override;

            void Destroy() override;

            void AddBone(const Bone& bone);
            void UpdateBone(std::string_view boneName, const Matrix& localToGlobal, const Matrix& orientation);

            // Set to initial orientation.
            void ResetOrientation() { _resource.bones = _resource.initialBones; }

            //
            // Find functions.
            //
            [[nodiscard]]
            const Bone* FindBoneByIndex(uint32_t index) const;

            [[nodiscard]]
            const Bone* FindBoneByName(std::string_view name) const;

            //
            // Getter
            //
            [[nodiscard]]
            AssetType GetAssetType() const override { return AssetType::Avatar; }

            [[nodiscard]]
            bool HasBone() const { return (not _resource.bones.empty()); }

            [[nodiscard]]
            const SkeletonResource& GetResource() const { return _resource; }

            //
            // Static functions.
            //
            static void LoadAssetTexture();

            [[nodiscard]]
            static Ref<Texture2D> GetAssetTexture() { return _avatarAssetTexture; }
        public:
            static const String ADD_FILENAME_AT_END;
        private:
            [[nodiscard]]
            Bone* FindBoneByNameRef(std::string_view name);

            [[nodiscard]]
            Bone* FindInitialBoneByNameRef(std::string_view name);
        private:
            friend class cereal::access;

            template <class Archive>
            void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(_resource)
                );

                OnSerialized();
            }

            template <class Archive>
            void load(Archive& archive, [[maybe_unused]] const uint32_t version)
            {
                archive(
                    cereal::base_class<Object>(this),
                    cereal::base_class<Asset>(this),
                    CEREAL_NVP(_resource)
                );

                OnDeserialized();
            }
        private:
            SkeletonResource _resource;

            static Ref<Texture2D> _avatarAssetTexture;
        };
    }
}

CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SkeletonResource, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::Skeleton, 1u)