// an ultra simple hlsl vertex shader
// TODO: Part 2i
#pragma pack_matrix(row_major)

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

ConstantBuffer<SCENE_DATA> cameraAndLights : register(b0, Space0);
ConstantBuffer<MESH_DATA> meshInfo : register(b1, Space0);
// TODO: Part 4f
// TODO: Part 4a
struct OUTPUT_TO_RASTERIZER
{
    float4 hProjectionSpace : SV_POSITION;
    float3 normalWorld : NORMAL;
    float3 positionWorld : WORLD;
};
// TODO: Part 1f
// TODO: Part 4b
OUTPUT_TO_RASTERIZER main(float4 inputVertex : POSITION, float4 inputVertexUV : UVMAP, float4 inputVertexN : NORMAL)
{
	// TODO: Part 1h
	//inputVertex.y -= 0.75f;
	//inputVertex.z += 0.75f;
	
    OUTPUT_TO_RASTERIZER rValue;

    inputVertex = mul(inputVertex, meshInfo.gwWorldMatrix);

    float4 globalMatrix = inputVertex;

    inputVertex = mul(inputVertex, cameraAndLights.gwViewMatrix);
    inputVertex = mul(inputVertex, cameraAndLights.gwProjectionMatrix);

    rValue.hProjectionSpace = inputVertex;
    rValue.normalWorld = mul(inputVertexN, meshInfo.gwWorldMatrix).xyz;
    rValue.positionWorld = globalMatrix;

    return rValue;
};
	// TODO: Part 2i

	// TODO: Part 4b