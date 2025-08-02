#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class PixelShader final : public IDXShader
	{
	public:
		PixelShader();
		PixelShader(const PixelShader& source);
		~PixelShader() override = default;

		PixelShader& operator=(const PixelShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11PixelShader> _pixelShader;
	};
};