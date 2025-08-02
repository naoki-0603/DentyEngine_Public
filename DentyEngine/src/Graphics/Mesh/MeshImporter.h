#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "Skeleton.h"
#include "Node.h"
#include "SceneTree.h"

#include "Graphics/Vertex.h"

#include "System/Resource/Animations/AnimationClip.h"

// Ref
// https://www.technicalife.net/loading-pmx-with-assimp/
// https://assimp-docs.readthedocs.io/en/latest/usage/

namespace DentyEngine
{
	namespace Utils
	{
		[[nodiscard]]
		std::string AiStringToString(const aiString& aiString);
	}

	class Mesh;
	class StaticMesh;
	class SkinnedMesh;
	class CollisionMesh;

	struct AssimpScene
	{
		struct Vertex
		{
			Vertex() : position(), normal(), tangent(), binormal(), texcoord(), color(1, 1, 1, 1) {}

			aiVector3D position;
			aiVector3D normal;
			aiVector3D tangent;
			aiVector3D binormal;

			aiVector3D texcoord;
			aiColor4D color;
		};

		struct Node
		{
			Node() : name(), nodeIndex(), numMeshes(), meshIndexes(), parent(), numChildren(), children(), transform(), rootNode(), hasBone(), skeleton() {}

			std::string name;

			uint32_t nodeIndex;

			uint32_t numMeshes;
			uint32_t* meshIndexes;

			// this type is aiNode*.
			aiNode* parent;

			uint32_t numChildren;
			aiNode** children;

			aiMatrix4x4 transform;

			bool rootNode;
			bool hasBone;
			bool skeleton;
		};

		struct Skeleton
		{
			struct Bone
			{
				struct VertexWeight
				{
					VertexWeight() : vertexID(), weight() {}

					uint32_t vertexID;
					float weight;
				};

				struct NecessityNode
				{
					using Map = std::vector<std::pair<std::string, Node>>;
				public:
					NecessityNode() : map() {}

					void Fetch(const AssimpScene& scene, const Bone& bone, const std::vector<Node>& nodes);

					void Process(const AssimpScene& scene);
					
				public:
					Map map;

				private:
					[[nodiscard]]
					Node* FindNodeByNameRef(std::string_view name)
					{
						for (auto&& node : map)
						{
							if (node.first != name)
								continue;

							return &node.second;
						}

						return nullptr;
					}

					void RecursivelyRise(const AssimpScene& scene, const AssimpScene::Node& node);
				};
			public:
				Bone() : name(), parentBoneName(), nodeIndex(), necessityNode() {}

				std::string name;
				std::string parentBoneName;

				uint32_t nodeIndex;

				NecessityNode necessityNode;
			};
		public:
			Skeleton() : bones() {}

			[[nodiscard]]
			const Bone* FindBoneByName(std::string_view name) const
			{
				for (const auto& bone : bones)
				{
					if (bone.name != name)
						continue;

					return &bone;
				}

				return nullptr;
			}

			[[nodiscard]]
			Bone* FindBoneByNameRef(std::string_view name)
			{
				for (auto&& bone : bones)
				{
					if (bone.name != name)
						continue;

					return &bone;
				}

				return nullptr;
			}

			[[nodiscard]]
			bool HasBone() const { return (not bones.empty()); }

			std::vector<Bone> bones;
		};

		struct Material
		{
			struct Texture
			{
				Texture() : filePath() {}

				std::string filePath;
			};
		public:
			Material() noexcept;
			~Material() noexcept = default;

		public:
			std::string name;
			std::string filePath;
			std::string convertedFilePath;

			aiColor3D diffuse;
			aiColor3D emissive;

			std::vector<Texture> textures;
		};

		struct Mesh
		{
			struct BoneInfo
			{
				BoneInfo() : name(), nodeIndex(), offsetMatrix(), weights() {}

				std::string name;
				uint32_t nodeIndex;

				Matrix offsetMatrix;
				std::vector<Skeleton::Bone::VertexWeight> weights;
			};
		public:
			Mesh() : name(), nodeIndex(), materialName(), materialIndex(), vertices(), indices(), boneInfo() {}

			[[nodiscard]]
			bool HasBone() const { return (not boneInfo.empty()); }

			std::string name;

			uint32_t nodeIndex;

			std::string materialName;
			uint32_t materialIndex;

			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			std::vector<BoneInfo> boneInfo;

		public:
			[[nodiscard]]
			std::optional<BoneInfo> FindBoneInfoByName(std::string_view paramName) const
			{
				for (const auto& localBoneInfo : boneInfo)
				{
					if (localBoneInfo.name != paramName)
						continue;

					return localBoneInfo;
				}

				return std::nullopt;
			}
		};

		struct BatchedMesh
		{
		public:
			BatchedMesh() : materialName(), materialIndex(), vertices(), indices() {}

			std::string materialName;

			uint32_t materialIndex;

			std::vector<MeshResource::StaticMeshVertex> vertices;
			std::vector<uint32_t> indices;
		};

		struct Animation
		{
			struct Node
			{
				struct VectorKey
				{
					VectorKey() : time(), value() {}

					// The time of this key.
					double time;

					// The value of this key.
					aiVector3D value;

					bool operator==(const VectorKey& rhs) const
					{
					    return (time == rhs.time && value == rhs.value);
					}
				};

				struct QuaternionKey
				{
					QuaternionKey() : time(), value() {}

					// The time of this key.
					double time;

					// The value of this key.
					aiQuaternion value;

					bool operator==(const QuaternionKey& rhs) const
					{
					    return (time == rhs.time && value == rhs.value);
					}
				};
			public:
				Node() : name(), positionKeys(), rotationKeys(), scalingKeys() {}

				std::string name;

				std::vector<VectorKey> positionKeys;
				std::vector<QuaternionKey> rotationKeys;
				std::vector<VectorKey> scalingKeys;

			public:
				[[nodiscard]]
				std::optional<VectorKey> FindPositionKeyByTime(float time) const;

				[[nodiscard]]
				std::optional<QuaternionKey> FindRotationKeyByTime(float time) const;

				[[nodiscard]]
				std::optional<VectorKey> FindScalingKeyByTime(float time) const;

				[[nodiscard]]
				std::optional<VectorKey> FindPositionKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const;

				[[nodiscard]]
				std::optional<QuaternionKey> FindRotationKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const;

				[[nodiscard]]
				std::optional<VectorKey> FindScalingKeyBetweenBeginTimeAndEndTime(float beginTime, float endTime) const;
			};
		public:
			Animation() : name(), ticksPerSecond(), duration(), ticksPerDuration(), ticksPerFrame(), nodes() {}

			std::string name;

			double ticksPerSecond;

			// Duration of the animation in ticks.
			double duration;

			float ticksPerDuration;
			float ticksPerFrame;

			std::vector<Node> nodes;
		public:
			void Convert();
		};
	public:
		AssimpScene() : filePath(), nodes(), meshes(), batchedMeshes(), materials(), animations(), skeleton() {}

		[[nodiscard]]
		bool HasAnimations() const { return (not animations.empty()); }

		[[nodiscard]]
		const Node* FindNodeByName(std::string_view name) const
		{
		    for (const auto& node : nodes)
		    {
		        if (node.name != name)
					continue;

				return &node;
		    }

			return nullptr;
		}

		[[nodiscard]]
		const Node* FindNodeByIndex(int index) const
		{
			DENTY_ASSERT((0 <= index) && (index < nodes.size()), "Index overflow!");

			const auto& node = nodes.at(index);

			return &node;
		}

		[[nodiscard]]
		Node* FindNodeByNameRef(std::string_view name)
		{
			for (auto& node : nodes)
			{
				if (node.name != name)
					continue;

				return &node;
			}

			return nullptr;
		}

		[[nodiscard]]
		std::vector<uint32_t> FindMeshIndexesByMaterialIndex(uint32_t materialIndex) const
		{
			std::vector<uint32_t> indexes = { };
			for (uint32_t index = 0u; index < meshes.size(); ++index)
			{
				const auto& mesh = meshes.at(index);

				if (mesh.materialIndex != materialIndex)
					continue;

				indexes.emplace_back(index);
			}

			return indexes;
		}

		void Reset()
		{
			filePath = FilePath();
			nodes.clear();
			meshes.clear();
			batchedMeshes.clear();
			materials.clear();
			animations.clear();

			skeleton.bones.clear();
		}

	public:
		FilePath filePath;

		std::vector<Node> nodes;
		std::vector<Mesh> meshes;
		std::vector<BatchedMesh> batchedMeshes;
		std::vector<Material> materials;
		std::vector<Animation> animations;

	    Skeleton skeleton;
	};

	class MeshImporter final
	{
	public:
		enum class ImportType
		{
			ImportAsMesh,
			ImportAsCollisionMesh,

			Max
		};
	public:
		MeshImporter() noexcept;
		~MeshImporter() = default;

		DELETE_COPY_MOVE_CONSTRUCTORS(MeshImporter)

		[[nodiscard]]
		Ref<Mesh> Import(const char* filePath, ImportType importType = ImportType::ImportAsMesh, bool fixName = true);

		// Note: should be call after mesh serialized.
		// Call on after MeshImporter::Import function.
		void BindSceneTreeMeshes();

		[[nodiscard]]
		std::optional<MeshResource::Node> FindNodeByName(std::string_view) const;

		[[nodiscard]]
		bool IsSkinned() const { return _isSkinned; }
	public:
		static const uint32_t AI_POST_PROCESS_STEPS_STATIC_MESH;
		static const uint32_t AI_POST_PROCESS_STEPS_SKINNED_MESH;
		static const uint32_t AI_POST_PROCESS_STEPS_COLLISION_MESH;

		static constexpr float COMPARE_ANIMATION_TIME_EPSILON = 1.192093E-05f;
	private:
		[[nodiscard]]
		Ref<Mesh> ReImportAsStatic(const char* filePath, ImportType importType = ImportType::ImportAsMesh, bool fixName = true);

		[[nodiscard]]
		Ref<Mesh> ImportAsCollision(const char* filePath, ImportType importType = ImportType::ImportAsMesh, bool fixName = true);

		void Reset();

		void TraverseNode(const aiScene* aiScene, aiNode* aiNode);

		//
		// Fetch functions.
		//
		void FetchMeshes(const aiScene* aiScene);
		void FetchBones(AssimpScene::Mesh& mesh, const aiMesh* aiMesh);
		void FetchSkeletonNecessityMap(const aiScene* aiScene);
		void FetchMaterials(const aiScene* aiScene);
		void FetchAnimations(const aiScene* aiScene);

		//
		// As static.
		//
		void FetchMeshesAsStatic(const aiScene* aiScene);

		void GenerateBatchedMeshes(const aiScene* aiScene);

		//
		// Create functions.
		//
		void CreateSkeleton();
		void CreateMaterials();
		void CreateNodes();
		void CreateSceneTree();
		void CreateMeshes(bool hasSkeleton, bool fixName);

		void CreateCollisionMeshes(bool fixName = true);

		void CreateStaticMesh();
		void CreateSkinnedMesh();

		void CreateStaticMesh(const Ref<StaticMesh>& staticMesh, const Matrix& scaling, const AssimpScene::BatchedMesh& mesh);
		void CreateSkinnedMesh(const Ref<SkinnedMesh>& skinnedMesh, const AssimpScene::Mesh& mesh);
		void CreateCollisionMesh(const Ref<CollisionMesh>& collisionMesh, const Matrix& scaling, const AssimpScene::Mesh& mesh);

		void CreateMaterial(const Ref<StaticMesh>& staticMesh, const AssimpScene::BatchedMesh& mesh);
		void CreateMaterial(const Ref<SkinnedMesh>& skinnedMesh, const AssimpScene::Mesh& mesh);

		void CreateAnimation(const AssimpScene::Animation& animation);

		void CreateBoundingBox(const Ref<Mesh>& mesh, const Matrix& scaling);

		void CreateVertexBuffer(const Ref<StaticMesh>& staticMesh);
		void CreateVertexBuffer(const Ref<CollisionMesh>& collisionMesh);
		void CreateVertexAndIndexBuffer(const Ref<SkinnedMesh>& skinnedMesh);

		void SetSystemUnit(const Ref<SkinnedMesh>& skinnedMesh, bool mainNode);
		void SetSystemUnit(const Ref<StaticMesh>& staticMesh, bool mainNode);
		void SetSystemUnit(const Ref<CollisionMesh>& collisionMesh, bool mainNode);

		void SetNodeBoneInfos(const aiScene* aiScene);

	    void SetBoneParentNames();

		[[nodiscard]]
		std::string ObtainBoneParentName(int32_t parentIndex) const;

		void SetSkeletonInfo();
		void SetMeshInfos(bool isCollisionMesh = false);

		void ComputeBoneGlobalTransforms();

		[[nodiscard]]
		Matrix ComputeBoneGlobalTransformAtKeyframe(const MeshResource::Keyframe& keyframe, Matrix& orientation, std::string_view parentBoneName);

		[[nodiscard]]
		FilePath ConvertMaterialPath(const String&) const;

		[[nodiscard]]
		FilePath ConvertTexturePath(const String&) const;

		[[nodiscard]]
		std::vector<MeshResource::SceneTreeResource> FindNodeByNodeType(MeshResource::NodeType nodeType) const;
	private:
		AssimpScene _scene;

        Ref<MeshResource::Skeleton> _skeleton;
		Ref<MeshResource::SceneTree> _sceneTree;

		std::vector<MeshResource::Node> _nodes;

		int32_t _nodeIndexCounter;

		// Store created mesh.
		Ref<Mesh> _mesh;

		uint32_t _scaleFactor;
		bool _isSkinned;
	};
}
