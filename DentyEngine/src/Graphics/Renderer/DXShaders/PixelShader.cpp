#include "stdafx.h"

#include "PixelShader.h"

namespace DentyEngine
{
	PixelShader::PixelShader() :
		IDXShader()
	{
	}

	PixelShader::PixelShader(const PixelShader& source)
	{
		source._pixelShader.CopyTo(_pixelShader.ReleaseAndGetAddressOf());
	}

	PixelShader& PixelShader::operator=(const PixelShader& source)
	{
		source._pixelShader.CopyTo(_pixelShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void PixelShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreatePixelShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_pixelShader.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void PixelShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void PixelShader::Bind(ID3D11DeviceContext* context)
	{
		context->PSSetShader(_pixelShader.Get(), nullptr, 0);
	}
};