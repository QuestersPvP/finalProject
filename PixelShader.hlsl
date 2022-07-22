// an ultra simple hlsl pixel shader
// TODO: Part 2b
struct OBJ_ATTRIBUTES
{
    float3 Kd; // diffuse reflectivity
    float d; // dissolve (transparency) 
    float3 Ks; // specular reflectivity
    float Ns; // specular exponent
    float3 Ka; // ambient reflectivity
    float sharpness; // local reflection map sharpness
    float3 Tf; // transmission filter
    float Ni; // optical density (index of refraction)
    float3 Ke; // emissive reflectivity
    uint illum; // illumination model
};
struct SCENE_DATA
{
    float4 lightDirection;
    float4 lightColor;
    float4 lightAmbient;
    float4 lightPos;
    //float4 camPos;
    float4x4 gwViewMatrix;
    float4x4 gwProjectionMatrix;
    float4 padding[4];
};
struct MESH_DATA
{
    float4x4 gwWorldMatrix;
    float4x4 gwWorldMatrixTwo;
    OBJ_ATTRIBUTES material;
    uint padding[12];
};
//struct ModelLocation
//{
//    float4x4 gwWorldMatrix;
//};

ConstantBuffer<SCENE_DATA> cameraAndLights : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);
//StructuredBuffer<ModelLocation> SceneData : register(t0, Space0);
// TODO: Part 4f
// TODO: Part 4b
struct OUTPUT_TO_RASTERIZER
{
    float4 hProjectionSpace : SV_POSITION;
    float3 normalWorld : NORMAL;
    float3 positionWorld : WORLD;
};

float4 main(float4 hProjectionSpace : SV_POSITION, float3 normalWorld : NORMAL, float3 positionWorld : WORLD) : SV_TARGET
{

    // LIGHTRATIO = CLAMP(DOT(-LIGHTDIR, SURFACENORMAL))
    // RESULT = LIGHTRATIO * LIGHTCOLOR * SURFACECOLOR

    //  VIEWDIR = NORMALIZE( CAMWORLDPOS – SURFACEPOS ) 
    //  HALFVECTOR = NORMALIZE(( -LIGHTDIR ) + VIEWDIR ) 
    //  INTENSITY = MAX( CLAMP( DOT( NORMAL, HALFVECTOR ))SPECULARPOWER , 0 )
    //  REFLECTEDLIGHT = LIGHTCOLOR * SPECULARINTENSITY * INTENSITY 

    
    float3 normal = normalize(normalWorld).xyz;

    float3 lightRatio = saturate(dot(-cameraAndLights.lightDirection.xyz, normal));

    float result = lightRatio * cameraAndLights.lightColor.xyz * meshInfo.material.Kd;

    float3 view = normalize(cameraAndLights.lightPos.xyz - positionWorld.xyz);

    float3 halfVec = normalize((-cameraAndLights.lightDirection.xyz) + view);

    float3 intensity = pow(max(saturate(dot(halfVec, normal)), 0), meshInfo.material.Ns);

    float3 reflectedLight = cameraAndLights.lightColor.xyz * meshInfo.material.Ks * intensity;

    float4 rValue = float4(saturate(result + cameraAndLights.lightAmbient) * meshInfo.material.Kd + reflectedLight + meshInfo.material.Ke,0);

    return rValue;

        //return float4(meshInfo.material.Kd,1);
        //return float4(0,1,0,1);

};