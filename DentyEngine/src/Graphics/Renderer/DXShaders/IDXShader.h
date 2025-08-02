#pragma once

namespace DentyEngine
{
	class IDXShader
	{
	public:
		IDXShader() = default;
		virtual ~IDXShader() = default;

		virtual void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) = 0;
		virtual void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) = 0;
		virtual void Bind(ID3D11DeviceContext* context) = 0;
	};
};