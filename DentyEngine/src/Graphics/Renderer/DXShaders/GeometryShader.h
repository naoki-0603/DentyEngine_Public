#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class GeometryShader final : public IDXShader
	{
	public:
		GeometryShader();
		GeometryShader(const GeometryShader& source);
		~GeometryShader() override = default;

		// Operators.
		GeometryShader& operator=(const GeometryShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11GeometryShader> _geometryShader;
	};
};