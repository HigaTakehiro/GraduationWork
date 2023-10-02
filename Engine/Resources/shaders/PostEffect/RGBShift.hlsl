#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float2 uvPoint = input.uv;
    float4 texcolor = tex0.Sample(smp, input.uv);
    uvPoint.x += 0.01;
    texcolor.r = tex0.Sample(smp, uvPoint).r;

    
    return float4(texcolor.rgb, 1);
}