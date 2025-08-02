#pragma once

#include "Texture.h"

#include "Graphics/Color.h"
#include "Graphics/Buffer/ColorBuffer.h"
#include "Graphics/Buffer/FrameBuffer.h"

namespace DentyEngine
{
	class FullScreenQuad;
	class Shader;

	struct ExpandedImageConstants
	{
		ExpandedImageConstants() :
			redElementEnabled(), greenElementEnabled(), blueElementEnabled(), alphaElementEnabled() {}

		int32_t redElementEnabled;
		int32_t greenElementEnabled;
		int32_t blueElementEnabled;
		int32_t alphaElementEnabled;
	};

	struct ExpandedImageCallbackInfo
	{
		ExpandedImageCallbackInfo() :  context(), samplerState(), constants(), shader() {}

		ID3D11DeviceContext* context;
		ID3D11SamplerState* samplerState;
		ExpandedImageConstants constants;

		Ref<Shader> shader;
	};

	class FrameBuffer;

	class Texture2D final : public Texture, public std::enable_shared_from_this<Texture2D>
	{
	public:
		Texture2D();
		Texture2D(const Texture2D&) = default;

		Texture2D& operator=(const Texture2D& source);

		void Load(ID3D11Device* device, const std::filesystem::path& filePath, bool serialize = true);
		void Load(ID3D11Device* device, const std::filesystem::path& filePath,
			const Vector2& clipPosition, const Vector2& clipSize, const Color& color, bool serialize = true);

		void CreateDummyTexture(ID3D11Device* device, const UINT dimension);

		void Save(bool isUpdate = false) const override;
		void Load(const std::filesystem::path& filePath) override;

		void Rename(std::string_view newName, bool deleteOldFile = true) override;

		//void UpdateAssetFile(EventAssetUpdatedOperationType type) override;
		void Clone(const Ref<Asset>& asset) override;

		void OnEvent(Event* e) override;
		void OnDestroy() override;

		void OnSerialized() const override;
		void OnDeserialized() override;

		// Expanded ImGui::Image function.
		void ExpandedImageGui(Vector2 size);

		//
		// Setter
		//
		void SetShaderResourceView(ComRef<ID3D11ShaderResourceView> shaderResourceView);

		//
		// Getter
		//
		[[nodiscard]]
		ID3D11ShaderResourceView* const* GetThumbnail() const { return _downSamplingTexture->GetColorBuffer()->GetResource(); }

		[[nodiscard]]
		const Vector2& GetClipPosition() const { return _clipPosition; }

		[[nodiscard]]
		const Vector2& GetClipSize() const { return _clipSize; }

		[[nodiscard]]
		const Color& GetColor() const { return _color; }
	private:
		void GenerateDownSamplingTexture();

		void ColorFilterGui();

	private:
		friend class cereal::access;

		template <class Archive>
		void save(Archive& archive, const uint32_t version) const
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Texture>(this),
					CEREAL_NVP(_clipPosition),
					CEREAL_NVP(_clipSize),
					CEREAL_NVP(_color)
				);    
			}

			OnSerialized();
		}

		template <class Archive>
		void load(Archive& archive, const uint32_t version)
		{
			if (version <= 1u)
			{
				archive(
					cereal::base_class<Texture>(this),
					CEREAL_NVP(_clipPosition),
					CEREAL_NVP(_clipSize),
					CEREAL_NVP(_color)
				);    
			}

			OnDeserialized();
		}

	private:
		// Doesn't serialize
		ExpandedImageCallbackInfo _callbackInfo;

		// Doesn't serialize, maybe serialize as thumbnail in the future.
		Ref<FrameBuffer> _downSamplingTexture;
		Ref<FullScreenQuad> _fullScreenQuad;

		Vector2 _clipPosition;
		Vector2 _clipSize;
		Color _color;

		// Doesn't serialize
		uint8_t _colorFiltering;
	private:
		//
		// Static variables.
		//
		static constexpr uint8_t RED = BIT(1);
		static constexpr uint8_t GREEN = BIT(2);
		static constexpr uint8_t BLUE = BIT(3);
		static constexpr uint8_t ALPHA = BIT(4);
	};
};

CEREAL_CLASS_VERSION(DentyEngine::Texture2D, 1u)

CEREAL_REGISTER_TYPE(DentyEngine::Texture2D)