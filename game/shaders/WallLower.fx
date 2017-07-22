#include <define>
#include <transform>
#include <sun>
#include <lights>
#include <fog>
#include <normal>

Texture mtlSkin1;
float4 vecTime;

sampler sTexture = sampler_state { Texture = <mtlSkin1>; MipFilter = Linear; };

struct out_ps // Output to the pixelshader fragment
{
	float4 Pos		: POSITION;
	float3 WorldPos : TEXCOORD1;
	float2 uv       : TEXCOORD0;
};

out_ps vs(
	float4 inPos : POSITION,
	float2 inTexCoord0 : TEXCOORD0)
{
	out_ps Out;
	Out.Pos = DoTransform(inPos);
	Out.WorldPos = mul(matWorld, float4(inPos.xyz, 1.0));
	Out.uv = inTexCoord0;
	return Out;
}

float4 ps(out_ps In): COLOR
{
	float height = -In.WorldPos.y*0.003333;
	float3 color = lerp(float3(0.0, 0.1, 0.0), float3(0.3, 0.1, 0.0), tex2D(sTexture, float2(In.uv.x, height)).r);
	color += tex2D(sTexture, float2(In.uv.x, height)).a * float3(0.7, 0.4, 0.0) * 3.0;
	color += max(height - 0.3, 0) * float3(1.0, 0.0, 0.0);
	return float4(color, 1.0);
}


technique object
{
	pass one
	{
		VertexShader = compile vs_2_0 vs();
		PixelShader = compile ps_2_0 ps();
	}
}

// fallback if nothing works
technique fallback { pass one { } }
