#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class ComputeShader final : public IDXShader
	{
	public:
		ComputeShader();
		ComputeShader(const ComputeShader& source);
		~ComputeShader() override = default;

		// Operators.
		ComputeShader& operator=(const ComputeShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11ComputeShader> _computeShader;
	};
};