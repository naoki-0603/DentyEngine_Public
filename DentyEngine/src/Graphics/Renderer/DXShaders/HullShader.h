#pragma once

#include "IDXShader.h"

namespace DentyEngine
{
	class HullShader final : public IDXShader
	{
	public:
		HullShader();
		HullShader(const HullShader& source);
		~HullShader() override = default;

		HullShader& operator=(const HullShader& source);

		void Create(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void OnReCompile(ID3D11Device* device, const ComRef<ID3DBlob>& shaderCode) override;
		void Bind(ID3D11DeviceContext* context) override;
	private:
		ComRef<ID3D11HullShader> _hullShader;
	};
};