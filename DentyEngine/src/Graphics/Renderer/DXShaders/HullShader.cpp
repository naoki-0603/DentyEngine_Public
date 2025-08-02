#include "stdafx.h"

#include "HullShader.h"

namespace DentyEngine
{
	HullShader::HullShader() :
		IDXShader()
	{
	}

	HullShader::HullShader(const HullShader& source)
	{
		source._hullShader.CopyTo(_hullShader.ReleaseAndGetAddressOf());
	}

	HullShader& HullShader::operator=(const HullShader& source)
	{
		source._hullShader.CopyTo(_hullShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void HullShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreateHullShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_hullShader.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void HullShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void HullShader::Bind(ID3D11DeviceContext* context)
	{
		context->HSSetShader(_hullShader.Get(), nullptr, 0);
	}
};