#include "PostEffect.hlsli"

Texture2D<float4> tex0 : register(t0); // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0); // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
    float2 uvPoint = input.uv;
    float4 texcolor = tex0.Sample(smp, input.uv);
    
    float sinv = sin(input.uv.y * 16 + time * -0.1);
    float steped = step(0.99, sinv * sinv);
    texcolor.rgb -= (1 - steped) * abs(sin(input.uv.y * 50.0 + time * 1.0)) * 0.001;
    texcolor.rgb -= (1 - steped) * abs(sin(input.uv.y * 100.0 - time * 2.0)) * 0.002;
    texcolor.rgb += steped * 0.01;
    
    float vignette = length(float2(0.5, 0.5) - input.uv);
    vignette = clamp(vignette - 0.45, 0, 1);
    texcolor.rgb -= vignette;
    
    return float4(texcolor.rgb, 1);
}