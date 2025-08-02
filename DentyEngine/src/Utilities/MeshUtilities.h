#pragma once

#include "Graphics/Vertex.h"

#include "Graphics/Mesh/MeshInfo.h"

namespace DentyEngine
{
    class Mesh;

    namespace MeshUtils
    {
        template <class VertexType>
        extern void CreateVertexBuffer(
            ID3D11Device* device,
            const std::vector<VertexType>& vertices,
            ID3D11Buffer** vertexBuffer
        );

        extern void CreateIndexBuffer(
            ID3D11Device* device,
            const std::vector<uint32_t>& indices,
            ID3D11Buffer** indexBuffer
        );

        extern void CalculateSubsetsIndexCount(
            const std::vector<uint32_t>& indices, std::vector<Subset>& subsets
        );

        extern void CalculateBoxMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);

        //extern std::pair<Ref<btTriangleMesh>, Ref<btConvexHullShape>> GenerateShapeFromMeshes(const Ref<Mesh>& mesh);
        extern std::pair<Ref<btTriangleMesh>, Ref<btConvexTriangleMeshShape>> GenerateShapeFromMeshes(const Ref<Mesh>& mesh);
    }
};