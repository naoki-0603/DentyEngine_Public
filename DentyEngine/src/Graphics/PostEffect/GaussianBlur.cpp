#include "stdafx.h"

#include "System/Core/Object.h"

#include "GaussianBlur.h"

#include "FullScreenQuad.h"

#include "Graphics/Color.h"
#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"
#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/Renderer/Shader.h"

#include "System/Graphics/GraphicsManager.h"

#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	void GaussianBlur::HorizontalDownSampling::Create(const Vector2& screenSize)
	{
		// Find shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			shader = shaderLibrary->Find(DENTY_FILE_PATH(ShaderLibrary::GAUSSIAN_BLUR_HORIZONTAL_PATH)).value();
		}

		// Create buffer.
		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const Vector2 size = { screenSize.x * 0.5f, screenSize.y };
			buffer = std::make_unique<FrameBuffer>();
			buffer->Create(device, size);
		}
	}

	void GaussianBlur::VerticalDownSampling::Create(const Vector2& screenSize)
	{
		// Find shader.
		{
			const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

			shader = shaderLibrary->Find(DENTY_FILE_PATH(ShaderLibrary::GAUSSIAN_BLUR_VERTICAL_PATH)).value();
		}

		// Create buffer.
		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			const Vector2 size = { screenSize.x, screenSize.y * 0.5f };
			buffer = std::make_unique<FrameBuffer>();
			buffer->Create(device, size);
		}
	}

	GaussianBlur::GaussianBlur() :
		PostEffect(), 
		_horizontalDownSampling(), _verticalDownSampling(), _constants(), _bokehRate(8.0f)
	{
		_name = "GaussianBlur";
	}

	void GaussianBlur::Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode, [[maybe_unused]] bool useDefaultBlendMode)
	{
		PostEffect::Create(sceneBuffer, screenSize, blendMode);

		// Create down sampling buffers. 
		{
			_horizontalDownSampling.Create(screenSize);
			_verticalDownSampling.Create(screenSize);
		}

		// Calculate weights.
		CalculateWeightsTableFromGaussian(_constants.weights, _bokehRate);

		_hasCreated = true;
	}

	void GaussianBlur::Process()
	{
		ID3D11DeviceContext* const context = GraphicsManager::GetInstance().GetContext();

		// Horizontal down sampling.
		{
			_horizontalDownSampling.buffer->Clear(context, Color::GRAY);
			_horizontalDownSampling.buffer->Activate(context);

			// Srv
			FullScreenQuad::SrvInfo srvInfo = { };
			srvInfo.startSlot = 0u;
			srvInfo.viewsNum = 1u;
			srvInfo.shaderResourceViews = _sceneBuffer->GetColorBuffer()->GetResource();

			// Constants.
			FullScreenQuad::ConstantInfo constantInfo = { };
			constantInfo.startSlot = 0u;
			constantInfo.constants = &_constants;

			_fullScreenQuad->Blit(_horizontalDownSampling.shader, srvInfo, constantInfo);

			_horizontalDownSampling.buffer->DeActivate(context);
		}

		// Vertical down sampling.
		{
			_verticalDownSampling.buffer->Clear(context, Color::GRAY);
			_verticalDownSampling.buffer->Activate(context);

			// Srv
			FullScreenQuad::SrvInfo srvInfo = { };
			srvInfo.startSlot = 0u;
			srvInfo.viewsNum = 1u;
			srvInfo.shaderResourceViews = _horizontalDownSampling.buffer->GetColorBuffer()->GetResource();

			// Constants.
			FullScreenQuad::ConstantInfo constantInfo = { };
			constantInfo.startSlot = 0u;
			constantInfo.constants = &_constants;

			_fullScreenQuad->Blit(_verticalDownSampling.shader, srvInfo, constantInfo);

			_verticalDownSampling.buffer->DeActivate(context);
		}

		// 
		{
			_resultBuffer->Clear(context, Color::GRAY);
			_resultBuffer->Activate(context);

			// Srv
			FullScreenQuad::SrvInfo srvInfo = { };
			srvInfo.startSlot = 0u;
			srvInfo.viewsNum = 1u;
			srvInfo.shaderResourceViews = _verticalDownSampling.buffer->GetColorBuffer()->GetResource();

			// Constants.
			FullScreenQuad::ConstantInfo constantInfo = { };
			constantInfo.startSlot = 0u;
			constantInfo.constants = nullptr;

			_fullScreenQuad->Blit(_fullScreenQuadShader, srvInfo, constantInfo);

			_resultBuffer->DeActivate(context);
		}
	}

	void GaussianBlur::OnGui()
	{
		// Parameter
		if (ImGui::TreeNodeEx("Parameter##GaussianBlur"))
		{
			ImGui::Indent();

			if (ImGui::DragFloat("BokehRate##Parameter", &_bokehRate, 0.01f, 1.0f, 32.0f))
			{
				// Calculate weights.
				CalculateWeightsTableFromGaussian(_constants.weights, _bokehRate);
			}

			ImGui::Unindent();

			ImGui::TreePop();
		}

		// Preview.
		if (_hasCreated)
		{
			if (ImGui::TreeNodeEx("Process##GaussianBlur"))
			{
				ImGui::Indent();

				// Horizontal
				{
					ImGui::TextWrapped("Horizontal");

					const ImTextureID texture = *_horizontalDownSampling.buffer->GetColorBuffer()->GetResource();
					ImGui::Image(texture, PROCESS_IMAGE_SIZE.ToImVec2());
				}

				// Vertical
				{
					ImGui::TextWrapped("Horizontal + Vertical");

					const ImTextureID texture = *_verticalDownSampling.buffer->GetColorBuffer()->GetResource();
					ImGui::Image(texture, PROCESS_IMAGE_SIZE.ToImVec2());
				}

				// Result
				{
					ImGui::TextWrapped("Result");

					const ImTextureID texture = *_resultBuffer->GetColorBuffer()->GetResource();
					ImGui::Image(texture, PROCESS_IMAGE_SIZE.ToImVec2());
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}
		}
	}

	void GaussianBlur::CalculateWeightsTableFromGaussian(std::array<float, 8>& weights, float bokehRate)
	{
		float weightTotal = 0.0f;

		// X -> distance from base texel.
		for (int x = 0; x < weights.size(); ++x)
		{
			weights.at(x) = std::expf(-0.5f * static_cast<float>(x * x) / bokehRate);
			weightTotal += 2.0f * weights.at(x);
		}

		// Normalized.
		for (int index = 0; index < weights.size(); ++index)
		{
			weights.at(index) /= weightTotal;
		}
	}
}
