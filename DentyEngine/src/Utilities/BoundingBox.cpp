#include "stdafx.h"

#include "BoundingBox.h"

#include "Graphics/Mesh/Mesh.h"
#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Mesh/SkinnedMesh.h"
#include "Physics/CollisionMesh.h"

namespace DentyEngine
{
	BoundingBox::BoundingBox() :
		_min(+D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX, +D3D11_FLOAT32_MAX),
		_max(-D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX, -D3D11_FLOAT32_MAX),
		_center(), _halfSize()
	{
	}

    void BoundingBox::Generate(const Ref<Mesh>& mesh, const Matrix& scaling, bool parentNode)
    {
		if (mesh->GetMeshType() == std::string("SkinnedMesh"))
		{
		    GenerateFromSkinnedMesh(std::static_pointer_cast<SkinnedMesh>(mesh), scaling, parentNode);
		}
	    else if (mesh->GetMeshType() == std::string("StaticMesh"))
		{
		    GenerateFromStaticMesh(std::static_pointer_cast<StaticMesh>(mesh), parentNode);
		}
		else if (mesh->GetMeshType() == std::string("CollisionMesh"))
		{
			GenerateFromCollisionMesh(std::static_pointer_cast<CollisionMesh>(mesh));
		}
		else
		{
		    DENTY_ASSERT(false, "Unknown mesh type! (in BoundingBox::Generate)");
		}

		{
		    const float width = fabsf(_max.x - _min.x);
		    const float height = fabsf(_max.y - _min.y);
		    const float depth = fabsf(_max.z - _min.z);

			_halfSize = { width * 0.5f, height * 0.5f, depth * 0.5f };
		    _center = { width * 0.5f, height * 0.5f, depth * 0.5f };
		}
    }

    void BoundingBox::OnGizmoRender()
    {

    }

    void BoundingBox::GenerateFromStaticMesh(const Ref<StaticMesh>& staticMesh, bool parentNode)
    {
		std::vector<Vector3> positions = { };

		if (parentNode)
		{
		    for (const auto& subMesh : staticMesh->GetSubMeshes())
		    {
		        const auto& subMeshSkinned = std::static_pointer_cast<StaticMesh>(subMesh);

				for (const auto& vertex : subMeshSkinned->GetVertices())
				{
				    positions.emplace_back(vertex.position.GetAsVector3());
				}
		    }
		}
		else
		{
		    for (const auto& vertex : staticMesh->GetVertices())
			{
			    positions.emplace_back(vertex.position.GetAsVector3());
			}
		}

		CalculateMinAndMax(positions);
    }

    void BoundingBox::GenerateFromSkinnedMesh(const Ref<SkinnedMesh>& skinnedMesh, const Matrix& scaling, bool parentNode)
    {
		std::vector<Vector3> positions = { };

		if (parentNode)
		{
		    for (const auto& subMesh : skinnedMesh->GetSubMeshes())
		    {
		        const auto& subMeshSkinned = std::static_pointer_cast<SkinnedMesh>(subMesh);

				for (const auto& vertex : subMeshSkinned->GetVertices())
				{
				    positions.emplace_back(scaling.TransformCoord(vertex.position.GetAsVector3()));
				}
		    }
		}
		else
		{
		    for (const auto& vertex : skinnedMesh->GetVertices())
			{
			    positions.emplace_back(scaling.TransformCoord(vertex.position.GetAsVector3()));
			}
		}

		CalculateMinAndMax(positions);
    }

    void BoundingBox::GenerateFromCollisionMesh(const Ref<CollisionMesh>& collisionMesh)
    {
		std::vector<Vector3> positions = {};
		for (const auto& vertex : collisionMesh->GetVertices())
		{
			positions.emplace_back(vertex.position.GetAsVector3());
		}

		CalculateMinAndMax(positions);
    }

    void BoundingBox::CalculateMinAndMax(const std::vector<Vector3>& positions)
    {
		for (const auto& position : positions)
		{
		    _min.x = std::min<float>(_min.x, position.x);
		    _min.y = std::min<float>(_min.y, position.y);
		    _min.z = std::min<float>(_min.z, position.z);

		    _max.x = std::max<float>(_max.x, position.x);
		    _max.y = std::max<float>(_max.y, position.y);
		    _max.z = std::max<float>(_max.z, position.z);
		}
    }
}
