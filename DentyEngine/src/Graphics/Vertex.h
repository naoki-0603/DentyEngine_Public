#pragma once

#include "Mesh/MeshInfo.h"

#include "Graphics/Color.h"

namespace DentyEngine
{
	class Texture2D;

	struct Vertex
	{
		Vertex() : position() {}
		Vertex(const Vertex& source) :
		    position(source.position) {}

		Vector4 position;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				CEREAL_NVP(position)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				CEREAL_NVP(position)
			);
		}
	};

	struct ParticleVertex : Vertex
	{
	public:
		ParticleVertex() : Vertex(), texcoord() {}

		Vector2 texcoord;
	};

	namespace MeshResource
	{
		struct StaticMeshVertex : Vertex
		{
			StaticMeshVertex() :
				Vertex(), normal(), tangent(), binormal(), texcoord(), padding() {}

			Vector4 normal;
			Vector4 tangent;
			Vector4 binormal;

			Vector2 texcoord;
			Vector2 padding;
		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, const uint32_t version) const
			{
				if (version <= 1u)
				{
					archive(
						cereal::base_class<Vertex>(this),
						CEREAL_NVP(normal),
						CEREAL_NVP(tangent),
						CEREAL_NVP(binormal),
						CEREAL_NVP(texcoord)
					);    
				}
			}

			template <class Archive>
			void load(Archive& archive, const uint32_t version)
			{
				if (version <= 1u)
				{
					archive(
						cereal::base_class<Vertex>(this),
						CEREAL_NVP(normal),
						CEREAL_NVP(tangent),
						CEREAL_NVP(binormal),
						CEREAL_NVP(texcoord)
					);    
				}
			}
		};

		struct SkinnedMeshVertex : Vertex
		{
			SkinnedMeshVertex() :
				Vertex(), normal(), tangent(), binormal(), boneWeights({0, 0, 0, 0}), boneIndices({0, 0, 0, 0}), texcoord() {}

			Vector4 normal;
			Vector4 tangent;
			Vector4 binormal;

			// Weight of the bone applied to the vertex.
			std::array<float, MAX_BONE_INFLUENCES> boneWeights;

			// Index number of the bone added to the vertex.
			std::array<uint32_t, MAX_BONE_INFLUENCES> boneIndices;

			Vector2 texcoord;
		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, const uint32_t version) const
			{
				if (version <= 1u)
				{
				   archive(
						cereal::base_class<Vertex>(this),
						CEREAL_NVP(normal),
						CEREAL_NVP(tangent),
						CEREAL_NVP(binormal),
						CEREAL_NVP(boneWeights),
						CEREAL_NVP(boneIndices),
						CEREAL_NVP(texcoord)
				    ); 
				}
			}

			template <class Archive>
			void load(Archive& archive, const uint32_t version)
			{
				if (version <= 1u)
				{
				   archive(
						cereal::base_class<Vertex>(this),
						CEREAL_NVP(normal),
						CEREAL_NVP(tangent),
						CEREAL_NVP(binormal),
						CEREAL_NVP(boneWeights),
						CEREAL_NVP(boneIndices),
						CEREAL_NVP(texcoord)
				    ); 
				}
			}
		};

		struct CollisionMeshVertex : Vertex
		{
			CollisionMeshVertex() : Vertex() {}

		private:
			friend class cereal::access;

			template <class Archive>
			void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
			{
				archive(
					cereal::base_class<Vertex>(this)
				);
			}

			template <class Archive>
			void load(Archive& archive, [[maybe_unused]] const uint32_t version)
			{
				archive(
					cereal::base_class<Vertex>(this)
				);
			}
		};
	}

	struct SpriteVertex : Vertex
	{
		SpriteVertex() : Vertex(), color(), texcoord() {}
		SpriteVertex(const SpriteVertex& source) :
			Vertex(source), color(Color::WHITE), texcoord(source.texcoord) {}

		Color color;
		Vector2 texcoord;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
			    archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(color),
				CEREAL_NVP(texcoord)
			);
			}
			
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
			    archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(color),
				CEREAL_NVP(texcoord)
			);
			}
		}
	};

	struct LineVertex : Vertex
	{
		LineVertex() = default;
		LineVertex(const Vector4& position, const Color& color) :
			Vertex(), color(color)
		{
			this->position = position;
		}

		LineVertex(const LineVertex& source) :
			Vertex(source), color(source.color) {}

		Color color;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, [[maybe_unused]] const uint32_t version) const
		{
			archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(color)
			);
		}

		template <class Archive>
		void load(Archive& archive, [[maybe_unused]] const uint32_t version)
		{
			archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(color)
			);
		}
	};

	//
	// Unused struct. (2024/01/09)
	//
     struct MeshVertex : Vertex
	{
		MeshVertex() : Vertex(),
			normal(), padding1(), texcoord(), padding2(),
			tangent(), boneWeights({ 1, 1, 1, 1 }), boneIndices({ 0, 1, 2, 3 }) {}

		MeshVertex(const MeshVertex& source) :
		    Vertex(source), normal(source.normal),
		    padding1(source.padding1), texcoord(source.texcoord),
		    padding2(source.padding2),
		    tangent(source.tangent), boneWeights(source.boneWeights), boneIndices(source.boneIndices) {}

		Vector3 normal;
		float padding1;

		Vector2 texcoord;
		Vector2 padding2;

		Vector4 tangent;

		// Weight of the bone applied to the vertex.
		std::array<float, MAX_BONE_INFLUENCES> boneWeights;

		// Index number of the bone added to the vertex.
		std::array<uint32_t, MAX_BONE_INFLUENCES> boneIndices;
	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive) const
		{
			archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(normal),
				CEREAL_NVP(texcoord),
				CEREAL_NVP(tangent),
				CEREAL_NVP(boneWeights),
				CEREAL_NVP(boneIndices)
			);
		}

		template <class Archive>
		void load(Archive& archive)
		{
			archive(
				cereal::base_class<Vertex>(this),
				CEREAL_NVP(normal),
				CEREAL_NVP(texcoord),
				CEREAL_NVP(tangent),
				CEREAL_NVP(boneWeights),
				CEREAL_NVP(boneIndices)
			);
		}
	};
};

CEREAL_CLASS_VERSION(DentyEngine::Vertex, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::StaticMeshVertex, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::SkinnedMeshVertex, 1u)
CEREAL_CLASS_VERSION(DentyEngine::MeshResource::CollisionMeshVertex, 1u)
CEREAL_CLASS_VERSION(DentyEngine::SpriteVertex, 1u)