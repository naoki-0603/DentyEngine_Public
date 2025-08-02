#include "stdafx.h"

#include "GeometryShader.h"

namespace DentyEngine
{
	GeometryShader::GeometryShader() :
		IDXShader()
	{
	}

	GeometryShader::GeometryShader(const GeometryShader& source)
	{
		source._geometryShader.CopyTo(_geometryShader.ReleaseAndGetAddressOf());
	}

	GeometryShader& GeometryShader::operator=(const GeometryShader& source)
	{
		source._geometryShader.CopyTo(_geometryShader.ReleaseAndGetAddressOf());

		return *this;
	}

	void GeometryShader::Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		const HRESULT hr = device->CreateGeometryShader(
			shaderCode->GetBufferPointer(),
			shaderCode->GetBufferSize(),
			nullptr,
			_geometryShader.ReleaseAndGetAddressOf()
		);
		DENTY_ASSERT(SUCCEEDED(hr), Utils::HRTrace(hr));
	}

	void GeometryShader::OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode)
	{
		Create(device, shaderCode);
	}

	void GeometryShader::Bind(ID3D11DeviceContext* context)
	{
		context->GSSetShader(_geometryShader.Get(), nullptr, 0);
	}
};