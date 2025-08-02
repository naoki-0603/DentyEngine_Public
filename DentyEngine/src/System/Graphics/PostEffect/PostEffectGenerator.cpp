#include "stdafx.h"

#include "PostEffectGenerator.h"

#include "Graphics/PostEffect/PostEffect.h"
#include "Graphics/PostEffect/GaussianBlur.h"
#include "Graphics/PostEffect/FullScreenQuad.h"
#include "Graphics/PostEffect/Bloom/Bloom.h"

#include "Utilities/StringUtilities.h"

namespace DentyEngine
{
	PostEffectGenerator::PostEffects PostEffectGenerator::_postEffects = { };

	PostEffectGenerator::PostEffectGenerator()
	{
	}

	void PostEffectGenerator::Initialize()
	{
		using PostEffectType = PostEffect::Type;

		const auto gaussianBlur = std::make_shared<GaussianBlur>();
		const auto bloom = std::make_shared<Bloom>();

		// Add to post effects.
		_postEffects.emplace(gaussianBlur->GetName(), gaussianBlur);
		_postEffects.emplace(bloom->GetName(), bloom);
	}

	void PostEffectGenerator::ShowList(_Out_ std::string& postEffectName, _Out_ bool& pressedAddPostEffectButton)
	{
		bool open = true;
		if (ImGui::Begin("PostEffect##PostEffectList", &open, ImGuiWindowFlags_NoDocking))
		{
			constexpr ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow;

			if (ImGui::TreeNodeEx("PostEffect##PostEffectList", nodeFlags))
			{
				ImGui::Indent();

				for (const auto& key : _postEffects | std::views::keys)
				{
					if (ImGui::Selectable(key.c_str()))
					{
						postEffectName = key;

						pressedAddPostEffectButton = false;
					}
				}

				ImGui::Unindent();

				ImGui::TreePop();
			}

			if (ImGui::IsWindowHovered(ImGuiHoveredFlags_RootWindow) && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
			{
				pressedAddPostEffectButton = false;
			}
		}

		ImGui::End();
	}

	Ref<PostEffect> PostEffectGenerator::Generate(std::string_view name)
	{
		if (Exists(name))
		{
			const auto it = _postEffects.find(std::string(name));

			// TODO Improve.
			if (name == "GaussianBlur")
			{
				return std::make_shared<GaussianBlur>();
			}
			else if (name == "Bloom")
			{
				return std::make_shared<Bloom>();
			}
		}

		return nullptr;
	}

	bool PostEffectGenerator::Exists(std::string_view name)
	{
		const auto it = _postEffects.find(std::string(name));

		return (it != _postEffects.end());
	}
}
