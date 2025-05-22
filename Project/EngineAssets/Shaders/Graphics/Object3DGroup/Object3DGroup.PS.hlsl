#include "Object3DGroup.hlsli"

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<Camera> gCamera : register(b1);
ConstantBuffer<ModelMaterial> gModelMaterial : register(b2);
ConstantBuffer<LightCount> gLightCount : register(b3);
Texture2D<float4> gTexture : register(t0);
StructuredBuffer<PunctualLight> gLights : register(t1);
Texture2D<float4> gNormalMap : register(t2);
SamplerState gSampler : register(s0);


PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = float4(0, 0, 0, 1);

    // 前処理
    float3 normalWS = normalize(input.normal);
    float3 tangentWS = normalize(input.tangent);
    
    // Binormal(=Bitangent) を再構築 (T x N)
    float3 binormalWS = normalize(cross(normalWS, tangentWS));
    
    float3 normalMapTS; // Tangent Space上の法線（= normalMap）
    if (gModelMaterial.enableNormalMap != 0)
    {
        // (1) 法線マップのサンプリング
        normalMapTS = gNormalMap.Sample(gSampler, input.texcoord).xyz;
        // [0,1] → [-1,1]へ
        normalMapTS = normalMapTS * 2.0f - 1.0f;
    }
    else
    {
    // (2) ノーマルマップ無しの場合 → Tangent空間では +Z を使う
    //     = ジオメトリ本来の法線をそのまま使う意味になる
        normalMapTS = float3(0, 0, 1);
    }
        
    // (2) TBN行列 (3x3) を作成
    float3x3 TBN = float3x3(
        tangentWS,
        binormalWS,
        normalWS
    );

    // TBN行列でWorldSpaceへ変換
    float3 normal = mul(normalMapTS, TBN);
    normal = normalize(normal);
    
    float3 toEye = normalize(gCamera.worldPosition - input.worldPosition);

    // Diffuse,Specularを蓄積するための変数
    float3 totalDiffuse = 0.0f;
    float3 totalSpecular = 0.0f;
    
    // UV情報を取得
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), mul(gMaterial.uvTransform, gModelMaterial.uvTransform));
    // テクスチャサンプリング
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);

    // discard処理
    if (textureColor.a <= 0.5f)
    {
        discard;
    }
    
    // 元のカラーを保存
    float3 baseColor = gModelMaterial.color.rgb * gMaterial.color.rgb * textureColor.rgb;
    float alpha = gModelMaterial.color.a * gMaterial.color.a * textureColor.a;
    
    
    if (gMaterial.enableLighting != 0)
    {
        // シーン中のすべてのライトをループ
        [loop]
        for (uint i = 0; i < gLightCount.num; i++)
        {
            PunctualLight light = gLights[i];

            switch (light.type)
            {
                case 0: // Directional
            {
                        float3 L = -normalize(light.direction);
                        float NdotL = saturate(0.5 * dot(normal, L) + 0.5);
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));

                        // Diffuse
                        float3 diffuse = light.color * light.intensity * NdotL;
                        totalDiffuse += diffuse;

                        // Specular
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow;
                        totalSpecular += specular;
                  
                        
                    }
                    break;

                case 1: // Point
            {
                        float3 lightDir = light.position - input.worldPosition;
                        float distance = length(lightDir);
                        float3 L = normalize(lightDir);
                        float NdotL = saturate(0.5 * dot(normal, L) + 0.5);
                    
                        // 距離減衰
                        float atten = pow(saturate(-distance / light.radius + 1.0f), light.decay);

                        // Diffuse                   
                        float3 diffuse = light.color * light.intensity * NdotL * atten;
                        totalDiffuse += diffuse;

                        // Specular
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow * atten;
                        totalSpecular += specular;
                    }
                    break;

                case 2: // Spot
            {
                        float3 lightDir = light.position - input.worldPosition;
                        float distance = length(lightDir);
                        float3 L = normalize(lightDir);
                        float NdotL = saturate(0.5 * dot(normal, L) + 0.5);
                    
                        // 距離減衰
                        float atten = pow(saturate(-distance / light.radius + 1.0f), light.decay);

                        // スポットライトの角度による減衰
                        float cosAngle = dot(normalize(lightDir), normalize(light.direction));
                        float falloff = saturate((cosAngle - light.cosAngle) /
                                          (light.cosFalloffStart - light.cosAngle));

                        // Diffuse                        
                        float3 diffuse = light.color * light.intensity * NdotL * atten * falloff;
                        totalDiffuse += diffuse;

                        // Specular
                        float3 H = normalize(L + toEye);
                        float NdotH = saturate(dot(normal, H));
                        float specPow = (gMaterial.shininess >= 1.0f && gMaterial.enableSpeculaerRef) ? pow(NdotH, gMaterial.shininess) : 0.0f;
                        float3 specular = light.color * light.intensity * specPow * atten * falloff;
                        totalSpecular += specular;
                    }
                    break;
            
            }
        }
        
        // 累積したライトの値を加算
        output.color.rgb = baseColor * totalDiffuse + totalSpecular;
        output.color.a = alpha;
    }
    else
    {
        // ライトを適用しない場合はそのまま
        output.color.rgb = baseColor;
        output.color.a = alpha;
    }
    
    
    // アルファカット
    if (output.color.a <= 0.0f)
    {
        discard;
    }
    
    // ガンマ補正
    output.color.rgb = pow(output.color.rgb, 2.2);
       
    return output;
}
