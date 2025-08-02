#include "stdafx.h"

#include "VertexShader.h"

namespace DentyEngine
{
	VertexShader::VertexShader() :
		IDXShader()
	{
	}

	VertexShader::VertexShader(const VertexShader& source)
	{
		source._vertexShader.CopyTo(_vertexShader.ReleaseAndGetAddressOf());
	}

	VertexShader& VertexShader::operator=(const VertexShader& source)
	{
		source._vertexShader.CopyTo(_vertexShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void VertexShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreateVertexShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_vertexShader.ReleaseAndGetAddressOf()
		);

		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void VertexShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void VertexShader::Bind(ID3D11DeviceContext* context)
	{
		context->VSSetShader(_vertexShader.Get(), nullptr, 0);
	}
};