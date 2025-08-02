#include "stdafx.h"

#include "Bloom.h"

#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/SceneBuffer.h"

#include "Graphics/PostEffect/FullScreenQuad.h"
#include "Graphics/PostEffect/GaussianBlur.h"

#include "System/Graphics/GraphicsManager.h"
#include "System/Resource/Library/LibraryManager.h"
#include "System/Resource/Library/ShaderLibrary.h"

namespace DentyEngine
{
	Bloom::Bloom() :
		PostEffect(), _gaussianBlur(), _luminanceExtractionShader()
	{
		_name = "Bloom";
	}

	void Bloom::Create(Ref<SceneBuffer> sceneBuffer, const Vector2& screenSize, BlendState::Type blendMode, bool useDefaultBlendMode)
	{
		PostEffect::Create(sceneBuffer, screenSize, (useDefaultBlendMode ? BlendState::Type::Add : blendMode));

		// Shader library.
		const ShaderLibrary* shaderLibrary = LibraryManager::GetInstance().GetShaderLibrary();

		// Create buffer.
		{
			ID3D11Device* device = GraphicsManager::GetInstance().GetDevice();

			_extractionLuminanceBuffer = std::make_shared<SceneBuffer>();
			_extractionLuminanceBuffer->Create(device, screenSize);
		}

		// Gaussian blur
		{
			_gaussianBlur = std::make_shared<GaussianBlur>();
			_gaussianBlur->Create(sceneBuffer, screenSize);
		}

		// Bloom shader.
		{
			_luminanceExtractionShader = shaderLibrary->Find(ShaderLibrary::BLOOM_PATH).value();
		}

		_hasCreated = true;
	}

	void Bloom::Process()
	{
		PostEffect::Process();

		ID3D11DeviceContext* context = GraphicsManager::GetInstance().GetContext();

		// Step-1 Extract luminance.
		{
			_extractionLuminanceBuffer->Clear(context, Color::BLACK);
			_extractionLuminanceBuffer->Activate(context);

			FullScreenQuad::SrvInfo srvInfo = {};
			srvInfo.startSlot = 0u;
			srvInfo.viewsNum = 1u;
			srvInfo.shaderResourceViews = _sceneBuffer->GetColorBuffer()->GetResource();

			FullScreenQuad::ConstantInfo constantInfo = {};
			constantInfo.startSlot = 0u;
			constantInfo.constants = nullptr;

			_fullScreenQuad->Blit(_luminanceExtractionShader, srvInfo, constantInfo);

			_extractionLuminanceBuffer->DeActivate(context);
		}

		// Step-2 Obscure extracted luminance buffer.
		{
			_gaussianBlur->SetBaseBuffer(_extractionLuminanceBuffer);
			_gaussianBlur->Process();
		}

		// Step-3 compose bokeh luminance buffer and scene buffer.
		{
			_resultBuffer->Clear(context, Color::BLACK);
			_resultBuffer->Activate(context);

			// Scene buffer.
			{
				FullScreenQuad::SrvInfo srvInfo = {};
				srvInfo.startSlot = 0u;
				srvInfo.viewsNum = 1u;
				srvInfo.shaderResourceViews = _sceneBuffer->GetColorBuffer()->GetResource();

				FullScreenQuad::ConstantInfo constantInfo = {};
				constantInfo.startSlot = 0u;
				constantInfo.constants = nullptr;

				_fullScreenQuad->Blit(_fullScreenQuadShader, srvInfo, constantInfo);
			}

			// Bokeh buffer.
			{
				FullScreenQuad::SrvInfo srvInfo = {};
				srvInfo.startSlot = 0u;
				srvInfo.viewsNum = 1u;
				srvInfo.shaderResourceViews = _gaussianBlur->GetResultBuffer()->GetColorBuffer()->GetResource();

				FullScreenQuad::ConstantInfo constantInfo = {};
				constantInfo.startSlot = 0u;
				constantInfo.constants = nullptr;

				_fullScreenQuad->Blit(_fullScreenQuadShader, srvInfo, constantInfo);
			}

			_resultBuffer->DeActivate(context);
		}

	}

	void Bloom::OnGui()
	{
		PostEffect::OnGui();

		if (ImGui::TreeNodeEx("GaussianBlur##Bloom"))
		{
			_gaussianBlur->OnGui();

			ImGui::TreePop();
		}

		// Preview.
		if (_hasCreated)
		{
			if (ImGui::TreeNodeEx("Process##Bloom"))
			{
				ImGui::Indent();

				// Scene buffer.
				{
					ImGui::TextWrapped("SceneBuffer");

					const ImTextureID texture = *_sceneBuffer->GetColorBuffer()->GetResource();
					ImGui::Image(texture, PROCESS_IMAGE_SIZE.ToImVec2());
				}

				// ExtractionLuminance
				{
					ImGui::TextWrapped("ExtractionLuminanceBuffer");

					const ImTextureID texture = *_extractionLuminanceBuffer->GetColorBuffer()->GetResource();
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
}
