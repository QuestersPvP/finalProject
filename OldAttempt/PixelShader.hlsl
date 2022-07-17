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
    float4 camPos;
    float4x4 gwViewMatrix;
    float4x4 gwProjectionMatrix;
    float4 padding[3];
};
struct MESH_DATA
{
    float4x4 gwWorldMatrix;
    float4x4 gwWorldMatrixTwo;
    OBJ_ATTRIBUTES material;
    uint padding[12];
};
struct ModelLocation
{
    float4x4 gwWorldMatrix;
};

ConstantBuffer<SCENE_DATA> cameraAndLights : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);
StructuredBuffer<ModelLocation> SceneData : register(t0, Space0);
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

//  VIEWDIR = NORMALIZE( CAMWORLDPOS – SURFACEPOS ) 
//  HALFVECTOR = NORMALIZE(( -LIGHTDIR ) + VIEWDIR ) 
//  INTENSITY = MAX( CLAMP( DOT( NORMAL, HALFVECTOR ))SPECULARPOWER , 0 )
//  REFLECTEDLIGHT = LIGHTCOLOR * SPECULARINTENSITY * INTENSITY 


    float3 normal = normalize(normalWorld).xyz;
    float3 view = normalize(cameraAndLights.camPos.xyz - positionWorld.xyz);
	//float3 reflectivity = reflect( (-view), normal) ;
    float3 halfVec = normalize((-cameraAndLights.lightDirection.xyz) + view);
	//float3 highlight = normalize(-cameraAndLights.lightDirection.xyz + view + reflectivity); // + R
	
    float3 ambient = meshInfo.material.Ka * cameraAndLights.lightAmbient.xyz * 0.65f;
    float3 diffuse = meshInfo.material.Kd * saturate(dot(normal, -cameraAndLights.lightDirection.xyz)) * cameraAndLights.lightColor.xyz;
    float3 specular = meshInfo.material.Ks * max(pow(dot(halfVec, normal), meshInfo.material.Ns), 0) * cameraAndLights.lightColor.xyz;
	
    return float4(0, 0, 1, 1);
	//return float4((ambient+diffuse)+ specular, 0);
	//return float4(ambient + (diffuse + specular) * cameraAndLights.lightColor.xyz, 1); // TODO: Part 1a
};