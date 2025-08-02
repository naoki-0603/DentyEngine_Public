#include "stdafx.h"

#include "MeshUtilities.h"

#include "Graphics/Mesh/Mesh.h"
//#include "Graphics/Mesh/DynamicMesh.h"
//#include "Graphics/Mesh/StaticBatchingMesh.h"

namespace DentyEngine
{
    namespace MeshUtils
    {
        template <>
        void CreateVertexBuffer(
            ID3D11Device* device,
            const std::vector<Vertex>& vertices,
            ID3D11Buffer** vertexBuffer
        )
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA subresourceData = {};
            subresourceData.pSysMem = vertices.data();

            const HRESULT hr = device->CreateBuffer(
                &bufferDesc,
                &subresourceData,
                vertexBuffer
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        template <>
        void CreateVertexBuffer(
            ID3D11Device* device,
            const std::vector<MeshVertex>& vertices,
            ID3D11Buffer** vertexBuffer
        )
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.ByteWidth = static_cast<UINT>(sizeof(MeshVertex) * vertices.size());
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA subresourceData = {};
            subresourceData.pSysMem = vertices.data();

            const HRESULT hr = device->CreateBuffer(
                &bufferDesc,
                &subresourceData,
                vertexBuffer
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        void CreateIndexBuffer(
            ID3D11Device* device,
            const std::vector<uint32_t>& indices,
            ID3D11Buffer** indexBuffer
        )
        {
            D3D11_BUFFER_DESC bufferDesc = {};
            bufferDesc.ByteWidth = static_cast<UINT>(sizeof(uint32_t) * indices.size());
            bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = 0;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;

            D3D11_SUBRESOURCE_DATA subresourceData = {};
            subresourceData.pSysMem = indices.data();

            const HRESULT hr = device->CreateBuffer(
                &bufferDesc,
                &subresourceData,
                indexBuffer
            );

            DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
        }

        void CalculateSubsetsIndexCount(
            const std::vector<uint32_t>& indices, std::vector<Subset>& subsets
        )
        {
            if (subsets.empty()) return;

            auto rit = subsets.rbegin();
            rit->indexCount = static_cast<uint32_t>(indices.size() - rit->indexStart);

            for (rit = subsets.rbegin() + 1; rit != subsets.rend(); ++rit)
            {
                rit->indexCount = (rit - 1)->indexStart - rit->indexStart;
            }
        }

        void CalculateBoxMesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
        {
            vertices.resize(8);

            // Set vertex position
            {
                vertices[0].position = { -1.0f, +1.0f, -1.0f, +1.0f };
                vertices[1].position = { -1.0f, -1.0f, -1.0f, +1.0f };
                vertices[2].position = { +1.0f, -1.0f, -1.0f, +1.0f };
                vertices[3].position = { +1.0f, +1.0f, -1.0f, +1.0f };

                vertices[4].position = { -1.0f, +1.0f, +1.0f, +1.0f };
                vertices[5].position = { -1.0f, -1.0f, +1.0f, +1.0f };
                vertices[6].position = { +1.0f, -1.0f, +1.0f, +1.0f };
                vertices[7].position = { +1.0f, +1.0f, +1.0f, +1.0f };
            }

            indices.resize(36);

            // Set indices
            {
                // front
                indices[0] = 1;
                indices[1] = 0;
                indices[2] = 3;

                indices[3] = 1;
                indices[4] = 3;
                indices[5] = 2;

                // back 
                indices[6] = 5;
                indices[7] = 7;
                indices[8] = 4;

                indices[9] = 5;
                indices[10] = 6;
                indices[11] = 7;

                // right
                indices[12] = 2;
                indices[13] = 3;
                indices[14] = 7;

                indices[15] = 2;
                indices[16] = 7;
                indices[17] = 6;

                // left
                indices[18] = 5;
                indices[19] = 4;
                indices[20] = 0;

                indices[21] = 5;
                indices[22] = 0;
                indices[23] = 1;

                // top
                indices[24] = 0;
                indices[25] = 4;
                indices[26] = 7;

                indices[27] = 0;
                indices[28] = 7;
                indices[29] = 3;

                // bottom
                indices[30] = 6;
                indices[31] = 5;
                indices[32] = 1;

                indices[33] = 2;
                indices[34] = 6;
                indices[35] = 1;
            }
        }

        std::pair<Ref<btTriangleMesh>, Ref<btConvexTriangleMeshShape>> GenerateShapeFromMeshes([[maybe_unused]] const Ref<Mesh>& mesh)
        {
            //const auto triangleMesh = std::make_shared<btTriangleMesh>();

            //switch (mesh->GetMeshType())
            //{
            //case MeshType::Dynamic:
	           // {
            //		const auto dynamic = static_cast<const DynamicMesh*>(mesh.get());

            //        // Vertices.
            //        for (int vertexIndex = 0; (vertexIndex + 3) < dynamic->GetVertices().size(); vertexIndex += 3)
            //        {
            //            const auto vertex0 = dynamic->GetVertices().at(vertexIndex).position.GetAsVector3();
            //            const auto vertex1 = dynamic->GetVertices().at(vertexIndex + 1).position.GetAsVector3();
            //            const auto vertex2 = dynamic->GetVertices().at(vertexIndex + 2).position.GetAsVector3();

            //            triangleMesh->addTriangle(
            //                MathConverter::Vector3ToBtVector3(vertex0),
            //                MathConverter::Vector3ToBtVector3(vertex1),
            //                MathConverter::Vector3ToBtVector3(vertex2)
            //            );
            //        }

            //        // Indices.
            //        for (int indicesIndex = 0; (indicesIndex + 3) < dynamic->GetIndices().size(); indicesIndex += 3)
            //        {
            //            const auto index0 = dynamic->GetIndices().at(indicesIndex);
            //            const auto index1 = dynamic->GetIndices().at(indicesIndex + 1);
            //            const auto index2 = dynamic->GetIndices().at(indicesIndex + 2);

            //            triangleMesh->addTriangleIndices(
            //                index0, index1, index2
            //            );
            //        }

            //        // Sub meshes.
            //		{
            //			for (int subMeshIndex = 0; subMeshIndex < dynamic->GetSubMeshCount(); ++subMeshIndex)
            //			{
            //                const auto subMesh = static_cast<const DynamicMesh*>(dynamic->FindSubMeshAt(subMeshIndex).get());

            //                // Vertices.
            //                for (int vertexIndex = 0; (vertexIndex + 3) < subMesh->GetVertices().size(); vertexIndex += 3)
            //                {
            //                    const auto vertex0 = subMesh->GetVertices().at(vertexIndex).position.GetAsVector3();
            //                    const auto vertex1 = subMesh->GetVertices().at(vertexIndex + 1).position.GetAsVector3();
            //                    const auto vertex2 = subMesh->GetVertices().at(vertexIndex + 2).position.GetAsVector3();

            //                    triangleMesh->addTriangle(
            //                        MathConverter::Vector3ToBtVector3(vertex0),
            //                        MathConverter::Vector3ToBtVector3(vertex1),
            //                        MathConverter::Vector3ToBtVector3(vertex2)
            //                    );
            //                }

            //                // Indices.
            //                for (int indicesIndex = 0; (indicesIndex + 3) < subMesh->GetIndices().size(); indicesIndex += 3)
            //                {
            //                    const auto index0 = subMesh->GetIndices().at(indicesIndex);
            //                    const auto index1 = subMesh->GetIndices().at(indicesIndex + 1);
            //                    const auto index2 = subMesh->GetIndices().at(indicesIndex + 2);

            //                    triangleMesh->addTriangleIndices(
            //                        index0, index1, index2
            //                    );
            //                }
            //			}
            //		}
            //        
            //        const auto convexTriangleMeshShape = std::make_shared<btConvexTriangleMeshShape>(triangleMesh.get());

            //        return std::make_pair(triangleMesh, convexTriangleMeshShape);
	           // }
            //    break;
            //case MeshType::StaticBatching:
            //    
            //    break;
            //}

            //DENTY_ASSERT(false, "Only supported dynamic mesh!. (in GenerateShapeFromMeshes)");

            return std::make_pair(nullptr, nullptr);
        }
    }
};