#include "Lighting.hlsli"

//================================ 
// CBV
//================================
ConstantBuffer<Camera> gCamera : register(b0);
ConstantBuffer<DirectionalLightData> gDirectionalLight : register(b1);
ConstantBuffer<LightCameraData> gLightCamera : register(b2);
ConstantBuffer<InverseCamera> gInvCamera : register(b3);

//================================ 
// GBuffer
//================================
Texture2D<float4> gAlbedoTex : register(t0);
Texture2D<float4> gNormalTex : register(t1);

//================================ 
// DepthTex
//================================
Texture2D<float> gDepthTex : register(t2);

//================================ 
// ShadowMapTex
//================================
Texture2D<float> gShadowMap : register(t3);

//================================
// EnvironmentTex
//================================
TextureCube<float4> gEnvironmentTex : register(t4);


//================================
// Samplers
//================================
SamplerState gSampler : register(s0);
SamplerComparisonState gShadowSampler : register(s1);
SamplerState gSamplerPoint : register(s2);

//================================
// HelperFuncs
//================================

// 平行光源を計算
float3 ComputeDirectionalLight(float3 normal)
{
    const float3 directionalLightDirection = normalize(gDirectionalLight.direction);
    const float directionalLightIntensity = gDirectionalLight.intencity;
    const float3 directionalLightColor = gDirectionalLight.color;
    float3 L = -directionalLightDirection;
    float NdotL = saturate(dot(normal, L));
    
    return directionalLightColor * directionalLightIntensity * NdotL;
}

// 平行光源の影を計算
float ComputeDirectionalLightShadow(float3 worldPos, float3 worldNormal)
{
    // ライト空間投影
    float4 lsPos = mul(float4(worldPos, 1.0f), gLightCamera.viewProjection);
    lsPos /= lsPos.w;

    // UV 変換（NDC→[0,1]）
    float2 uv = lsPos.xy * float2(0.5f, -0.5f) + 0.5f;

    // 法線ベースの可変バイアス計算
    float3 N = normalize(worldNormal);
    float3 L = normalize(-gDirectionalLight.direction);
    float ndl = saturate(dot(N, L)); // [0,1]
    float normalBias = ConstantBias + (1.0f - ndl) * SlopeBiasScale;

    // 増減バイアスを反映した深度参照値
    float depthRef = lsPos.z - normalBias;

    // シャドウテスト
    float shadow = 1.0f;
    if (uv.x >= 0.0f && uv.x <= 1.0f &&
        uv.y >= 0.0f && uv.y <= 1.0f)
    {
        // SampleCmpLevelZero で比較
        shadow = gShadowMap.SampleCmpLevelZero(gShadowSampler, uv, depthRef);
    }
    return shadow;
}

// 環境マップ計算
float3 ComputeEnvironment(float3 position, float3 normal, float coefficient)
{
    float3 cameraToPosition = normalize(position - gCamera.worldPosition);
    float3 reflectedVector = reflect(cameraToPosition, normalize(normal));
    float3 environmentColor = gEnvironmentTex.Sample(gSampler, reflectedVector).rgb;
    
    return environmentColor * coefficient;
}

// uv、深度バッファ、カメラの逆行列からワールド座標を復元
float3 ReconstructWorldPos(float2 uv, float depth, float4x4 invProj, float4x4 invView)
{
    float3 ndc = float3(uv.x * 2.0f - 1.0f, 1.0f - uv.y * 2.0f, depth);
    float4 viewSpace = mul(float4(ndc, 1.0f), invProj);
    viewSpace.xyz *= rcp(viewSpace.w); // 同時座標系からデカルト座標系へ変換
    
    return mul(float4(viewSpace.xyz, 1.0f), invView).xyz; // ワールド座標に変換
}

//================================
// EntryPoint
//================================
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // GBufferから取得
    float4 albedo = gAlbedoTex.Sample(gSampler, input.texcoord);
    float4 normalRaw = gNormalTex.Sample(gSampler, input.texcoord);
    
    // 深度バッファから深度を取得
    float ndxDepth = gDepthTex.Sample(gSamplerPoint, input.texcoord).r;
    // ワールド座標を復元
    float3 worldPos = ReconstructWorldPos(input.texcoord, ndxDepth, gInvCamera.invProj, gInvCamera.invView);

    // Normalを[-1,1]空間に戻して正規化
    float3 normal = normalize(normalRaw.xyz * 2.0f - 1.0f);
    
    // 元の色とalphaを取得
    float3 baseColor = albedo.rgb;
    float alpha = albedo.a;
    
    // カラー加算用の値
    float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);
     
    // 
    // DirectionalLight
    // 
    {      
        float3 directionalLightDiffuse = ComputeDirectionalLight(normal);
        float directionalLightShadow = ComputeDirectionalLightShadow(worldPos, normal);
        totalDiffuse += directionalLightDiffuse * directionalLightShadow;
    }
    
    //
    // Environment
    //
    {
        float3 environmentDiffuse = ComputeEnvironment(worldPos, normal, 3.0f);
        totalDiffuse += environmentDiffuse;
    }
     
    // 
    // FinalColor
    // 
    
    output.color.rgb = baseColor * totalDiffuse;
    
    output.color.a = alpha;
    
    return output;
}
