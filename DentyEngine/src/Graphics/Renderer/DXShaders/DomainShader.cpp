#include "stdafx.h"

#include "DomainShader.h"

namespace DentyEngine
{
	DomainShader::DomainShader() :
		IDXShader()
	{
	}

	DomainShader::DomainShader(const DomainShader& source)
	{
		source._domainShader.CopyTo(_domainShader.ReleaseAndGetAddressOf());
	}

	DomainShader& DomainShader::operator=(const DomainShader& source)
	{
		source._domainShader.CopyTo(_domainShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void DomainShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreateDomainShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_domainShader.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void DomainShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void DomainShader::Bind(ID3D11DeviceContext* context)
	{
		context->DSSetShader(_domainShader.Get(), nullptr, 0);
	}
};