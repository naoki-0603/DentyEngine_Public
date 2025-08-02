#include "Particle.hlsli"

VSOut main(const uint vertexId : SV_VertexId)
{
    VSOut vout;
	vout.vertexId = vertexId;

	return vout;
}