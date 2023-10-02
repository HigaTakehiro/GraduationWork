cbuffer cbuff0 : register(b0)
{
    //時間
    float time;
    //最大時間
    float maxTime;
    //ブラーが適用されない範囲
    float mask;
    //ブラーの中心位置(x座標)
    float blurCenterX;
    //ブラーの中心位置(y座標)
    float blurCenterY;
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
    float4 svpos : SV_POSITION; // システム用頂点座標
    float2 uv  :TEXCOORD; // uv値
};
