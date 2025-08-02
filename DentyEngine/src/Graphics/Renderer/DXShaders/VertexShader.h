#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class VertexShader final : public IDXShader
	{
	public:
		VertexShader();
		VertexShader(const VertexShader& source);
		~VertexShader() override = default;

		// Operators
		VertexShader& operator=(const VertexShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11VertexShader> _vertexShader;
	};
};