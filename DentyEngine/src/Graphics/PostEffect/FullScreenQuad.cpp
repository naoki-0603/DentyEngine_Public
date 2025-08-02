#include "stdafx.h"

#include "FullScreenQuad.h"

#include "Graphics/Buffer/ConstantBuffer.h"
#include "Graphics/Renderer/Shader.h"

#include "System/Graphics/GraphicsManager.h"

namespace DentyEngine
{
	FullScreenQuad::FullScreenQuad()
	{
	}

	void FullScreenQuad::Blit(const Ref<Shader>& shader, const SrvInfo& srvInfo, const ConstantInfo& constantInfo)
	{
		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
		context->IASetInputLayout(nullptr);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		context->PSSetShaderResources(
			srvInfo.startSlot, srvInfo.viewsNum, srvInfo.shaderResourceViews
		);

		if (constantInfo.constants)
		{
			shader->constantBuffer->UpdateSubresource(context, constantInfo.constants);
			shader->constantBuffer->BindAll(context, constantInfo.startSlot);
		}

		shader->BindVertexShader(context);
		shader->BindPixelShader(context);

		context->Draw(4, 0);
	}
}
