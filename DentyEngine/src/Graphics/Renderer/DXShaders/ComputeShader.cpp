#include "stdafx.h"

#include "ComputeShader.h"

namespace DentyEngine
{
	ComputeShader::ComputeShader() :
		IDXShader()
	{
	}

	ComputeShader::ComputeShader(const ComputeShader& source)
	{
		source._computeShader.CopyTo(_computeShader.ReleaseAndGetAddressOf());
	}

	ComputeShader& ComputeShader::operator=(const ComputeShader& source)
	{
		source._computeShader.CopyTo(_computeShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void ComputeShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreateComputeShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_computeShader.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void ComputeShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void ComputeShader::Bind(ID3D11DeviceContext* context)
	{
		context->CSSetShader(_computeShader.Get(), nullptr, 0);
	}
};