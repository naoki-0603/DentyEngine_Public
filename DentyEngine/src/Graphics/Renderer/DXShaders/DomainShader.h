#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class DomainShader final : public IDXShader
	{
	public:
		DomainShader();
		DomainShader(const DomainShader& source);
		~DomainShader() override = default;

		// Operators.
		DomainShader& operator=(const DomainShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11DomainShader> _domainShader;
	};
};