// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dx12.h" // official helper file provided by microsoft
// Part 1b DONE
#include "FSLogo.h"

#include <vector>
#include "h2bParser.h"

using namespace std::chrono;

#pragma region Shaders
// Simple Vertex Shader
const char* vertexShaderSource = R"(
// an ultra simple hlsl vertex shader
// TODO: Part 2i
#pragma pack_matrix(row_major)

// TODO: Part 2b
struct OBJ_ATTRIBUTES
{
	float3    Kd; // diffuse reflectivity
	float	    d; // dissolve (transparency) 
	float3    Ks; // specular reflectivity
	float       Ns; // specular exponent
	float3    Ka; // ambient reflectivity
	float       sharpness; // local reflection map sharpness
	float3    Tf; // transmission filter
	float       Ni; // optical density (index of refraction)
	float3    Ke; // emissive reflectivity
	uint    illum; // illumination model
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
	// TODO: Part 2i

	// TODO: Part 4b
}
)";
//float4 main(float3 inputVertex : POSITION, float3 inputVertexUV : UVMAP, float3 inputVertexN : NORMAL) : SV_POSITION
//{
//	// Part 1h
// 	return float4(inputVertex, 0, 1); // origional
//	return float4(inputVertex, inputVertexUV, inputVertexN, 1);
//// TODO: Part 2i
//// TODO: Part 4b
//}

// Simple Pixel Shader
const char* pixelShaderSource = R"(
// an ultra simple hlsl pixel shader
// TODO: Part 2b
struct OBJ_ATTRIBUTES
{
	float3    Kd; // diffuse reflectivity
	float	    d; // dissolve (transparency) 
	float3    Ks; // specular reflectivity
	float       Ns; // specular exponent
	float3    Ka; // ambient reflectivity
	float       sharpness; // local reflection map sharpness
	float3    Tf; // transmission filter
	float       Ni; // optical density (index of refraction)
	float3    Ke; // emissive reflectivity
	uint    illum; // illumination model
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
	float3 view = normalize(cameraAndLights.camPos.xyz - positionWorld.xyz );
	//float3 reflectivity = reflect( (-view), normal) ;
	float3 halfVec = normalize((-cameraAndLights.lightDirection.xyz) + view);
	//float3 highlight = normalize(-cameraAndLights.lightDirection.xyz + view + reflectivity); // + R
	
	float3 ambient = meshInfo.material.Ka * cameraAndLights.lightAmbient.xyz * 0.65f;
	float3 diffuse = meshInfo.material.Kd * saturate(dot(normal,-cameraAndLights.lightDirection.xyz)) * cameraAndLights.lightColor.xyz;
	float3 specular = meshInfo.material.Ks * max(pow(dot(halfVec, normal), meshInfo.material.Ns), 0) * cameraAndLights.lightColor.xyz; 
	
	return float4 (0,0,1,1);
	//return float4((ambient+diffuse)+ specular, 0);
	//return float4(ambient + (diffuse + specular) * cameraAndLights.lightColor.xyz, 1); // TODO: Part 1a
}
)";
#pragma endregion


float DegreeToRadians(float degree)
{
	return degree * (3.14 / 180);
}

high_resolution_clock::time_point timeEnd = high_resolution_clock::now();
float rotationAmount = 0.0f;

GW::MATH::GMATRIXF UpdateMatrix(GW::MATH::GMATRIXF inMatrix)
{
	high_resolution_clock::time_point timeStart = high_resolution_clock::now();
	duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);

	GW::MATH::GMatrix gwGmatrix;
	gwGmatrix.Create();
	const float rotationSpeed = 0.4f;

	rotationAmount += rotationSpeed * (timePassed.count() + 0.5f);

	gwGmatrix.RotateYGlobalF(inMatrix, DegreeToRadians(rotationAmount), inMatrix);

	if (rotationAmount >= 360.0f)
	{
		rotationAmount -= 360.0f;
	}

	timeEnd = high_resolution_clock::now();

	return inMatrix;
}

// Creation, Rendering & Cleanup
class Renderer
{
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GDirectX12Surface d3d;

	GW::INPUT::GInput gInput;
	GW::INPUT::GController gController;
	
	// what we need at a minimum to draw a triangle
	D3D12_VERTEX_BUFFER_VIEW					vertexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		vertexBuffer;
	//  Part 1g DONE
	D3D12_INDEX_BUFFER_VIEW						indexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		indexBuffer;
	// TODO: Part 2c
	D3D12_CONSTANT_BUFFER_VIEW_DESC				constantView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		constantBuffer;
	// TODO: Part 2e
	//ID3D12DescriptorHeap*						descriptorHeap;
	D3D12_DESCRIPTOR_HEAP_DESC					descriptorView;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;

	Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	pipeline;
	// Part 2a DONE
	GW::MATH::GMATRIXF gwCopyMatrix;
	GW::MATH::GMATRIXF gwWorldMatrix;
	GW::MATH::GMATRIXF gwViewMatrix;
	GW::MATH::GMATRIXF gwProjectionMatrix;
	GW::MATH::GVECTORF lightDirection;
	GW::MATH::GVECTORF lightColor;
	GW::MATH::GVECTORF lightAmbient;
	GW::MATH::GVECTORF lightPos;

	float mouseYLast = 0.0f;
	float mouseXLast = 0.0f;
	unsigned int tempHeight = 0;
	unsigned int tempWidth = 0;
	float aspectRatio = 0.0f;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// TODO: Part 2b
	struct SCENE_DATA
	{
		GW::MATH::GVECTORF lightDirection;
		GW::MATH::GVECTORF lightColor;
		GW::MATH::GVECTORF lightAmbient;
		GW::MATH::GVECTORF lightPos;
		GW::MATH::GVECTORF camPos;
		GW::MATH::GMATRIXF gwViewMatrix;
		GW::MATH::GMATRIXF gwProjectionMatrix;
		GW::MATH::GVECTORF padding[3];
	};
	struct MESH_DATA
	{
		GW::MATH::GMATRIXF gwWorldMatrix;
		GW::MATH::GMATRIXF gwWorldMatrixTwo;
		H2B::ATTRIBUTES material;
		unsigned padding[12];
	};

	struct ModelInfo
	{
		std::string modelNames;						// file path
		GW::MATH::GMATRIXF modelLocations;			// world matrix of model
		std::vector<H2B::MESH> modelMeshes;			// mesh information of model
		std::vector<H2B::MATERIAL> modelMaterial;	// material information
		//std::vector<H2B::VERTEX> modelVertex;		// vertex information
		//std::vector<unsigned> modelIndecies;		// index information

		unsigned int vertexBufferStart;				// keep track of models vertex buffer
		unsigned int indexBufferStart;				// keep track of models index buffer
	};

	std::vector<ModelInfo> modelInformation;		// keep track of every models information
	std::vector<H2B::VERTEX> vertexInfo;			// keep track of all vertecies
	std::vector<unsigned> indexInfo;				// keep track of all indexes

	//UINT8* transferMemoryLocation1;
	UINT8* transferMemoryLocation;
	MESH_DATA meshDataText;
	MESH_DATA meshDataLogo;
	SCENE_DATA sceneData;
	// TODO: Part 4f
public:
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GDirectX12Surface _d3d)
	{
		win = _win;
		d3d = _d3d;
		ID3D12Device* creator;
		d3d.GetDevice((void**)&creator);
		// part 2a DONE
		// WORLD
#pragma region Lab 2 Start
		GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
		gwGmatrix.Create();
		gwGmatrix.IdentityF(gwWorldMatrix);
		//gwGmatrix.RotateYGlobalF(gwWorldMatrix, DegreeToRadians(90.0f), gwWorldMatrix);
		// VIEW
		//viewMatrixTranslation.x = 0.75f;
		//viewMatrixTranslation.y = 0.25f;
		//viewMatrixTranslation.z = -1.5f;
		//viewMatrixTranslation.w = 1.0f;
		//gwGmatrix.TranslateGlobalF(gwViewMatrix, viewMatrixTranslation, gwViewMatrix);
		//GW::MATH::GVECTORF viewMatrixTranslation;
		gwGmatrix.IdentityF(gwViewMatrix);
		GW::MATH::GVECTORF eye;
		eye.x = 0.75f;
		eye.y = 0.25f;
		eye.z = -1.5f;
		eye.w = 1.0f;
		GW::MATH::GVECTORF at;
		at.x = -0.8553; //-0.8553, 0.7554,  0.9219, 1.0000
		at.y = 0.7554;
		at.z = 0.9219;
		at.w = 1.0f;
		GW::MATH::GVECTORF up;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
		up.w = 0.0f;
		gwGmatrix.LookAtLHF(eye, at, up, gwViewMatrix);
		// PROJECTION
		_win.GetClientHeight(tempHeight);
		_win.GetClientWidth(tempWidth);
		gwGmatrix.IdentityF(gwProjectionMatrix);
		aspectRatio = ((float)tempWidth / (float)tempHeight);
		gwGmatrix.ProjectionDirectXLHF(DegreeToRadians(65), aspectRatio, 0.1f, 100, gwProjectionMatrix);
		// LIGHTING
		lightDirection.x = -1.0f;
		lightDirection.y = -1.0f;
		lightDirection.z = 2.0f;
		lightDirection.w = 0.0f;
		lightColor.x = 0.9f;
		lightColor.y = 0.9f;
		lightColor.z = 1.0f;
		lightColor.w = 0.0f;
		lightAmbient.x = 0.25f;
		lightAmbient.y = 0.25f;
		lightAmbient.z = 0.35f;
		lightAmbient.w = 0.0f;
		lightPos.x = 0.15f;
		lightPos.y = 0.75f;
		lightPos.z = -1.5f;
		lightPos.w = 0.0f;

		// TODO: part 2b
		sceneData.gwProjectionMatrix = gwProjectionMatrix;
		sceneData.gwViewMatrix = gwViewMatrix;
		sceneData.lightColor = lightColor;
		sceneData.lightDirection = lightDirection;
		sceneData.lightAmbient = lightAmbient;
		sceneData.lightPos = lightPos;
		sceneData.camPos = eye;

		meshDataText.gwWorldMatrix = gwWorldMatrix;
		meshDataText.gwWorldMatrixTwo = gwWorldMatrix;
		//meshDataText.material = FSLogo_materials[0].attrib;

		//meshDataLogo.material = FSLogo_materials[1].attrib;
		meshDataLogo.gwWorldMatrix = gwWorldMatrix;
		meshDataLogo.gwWorldMatrixTwo = gwWorldMatrix;
#pragma endregion



#pragma region File IO
		/// <summary>
		/// READING FILE START
		/// </summary>
		
		// run h2b file parser on the meshes you read in
		// array of matrix
		// parse gamelevel.txt
		//1.) Use either std::ifstream or FILE * to open the file.
		std::ifstream reader;
		std::string lineRead;
		std::string mFileName = "../GameLevel.txt";
		reader.open(mFileName);

		//int modelsReadIn = 0;							// keep track of how many models
		//bool sameModelName = false;						// bool to keep track of if a model name has been read in already or not

		//2.) Enter a infinite loop that stops when there are no more lines to read from the file.

		if (reader.is_open())
		{
			std::cout << "File is open - time to read" << std::endl;

			while (true)
			{
				if (reader.eof())
					break;
				//3.) Read the current line of text from the file up to the newline "\n". (can use std::getline)
				//	3b.) Exit the loop if nothing to read.
				std::getline(reader, lineRead);
				//4.) String compare for the name of what you want. (intially this will be "MESH")[std::strcmp]
				if (std::strcmp(lineRead.c_str(), "MESH") == 0)
				{
					std::vector<std::string> fileNames; // vector of saved file names
					ModelInfo tempModelInfo;			// Hold temporary model information
					//GW::MATH::GMATRIXF theNewMatrix;	// a matrix to store the global position

					//5.) Read the next line, this is the name of the .h2b file to load in. (duplicates?)
					std::cout << "FOUND A MESH" << std::endl;
					std::getline(reader, lineRead);

					// separate the .001 ect. from the string
					std::stringstream findName(lineRead);
					while (std::getline(findName, lineRead, '.'))
					{
						lineRead = "../models/" + lineRead + ".h2b"; // convert to correct file path
						fileNames.push_back(lineRead);
					}

					std::cout << "Model Name: " << fileNames[0] << std::endl; // for debugging
					tempModelInfo.modelNames = fileNames[0];

					//6.) Read the next 4 lines.Each line contains a row of the 4x4 matrix of where the model is.
					//	6b.) You can walk through the stringand use std::atof / std::stof to grab the floats.
					//	6c.) Alternatley you could use std::sscanf or just std::string helpers to pull the data.
					for (int i = 0; i < 4; i++)
					{
						std::vector<std::string> lineStrings;	// for separating out the strings
						std::vector<float> lineFloats;			// for separating out the floats

						std::getline(reader, lineRead);
						// breaking the matrix into 1.000, 1.000, 1.000, 1.000)>
						std::stringstream findParen(lineRead);
						while (std::getline(findParen, lineRead, '('))
						{
							lineStrings.push_back(lineRead);
						}
						// breaking the matrix into 1.000, 1.000, 1.000, 1.000)
						std::stringstream findCarrot(lineStrings[1]);
						while (std::getline(findCarrot, lineRead, '>'))
						{
							lineStrings.push_back(lineRead);
						}
						// breaking the matrix into 1.000, 1.000, 1.000, 1.000
						std::stringstream find2ndParen(lineStrings[2]);
						while (std::getline(find2ndParen, lineRead, ')'))
						{
							lineStrings.push_back(lineRead);
						}
						// breaking the matrix into 1.000 1.000 1.000 1.000
						std::stringstream findFloats(lineStrings[3]);
						int counter = 4;
						while (std::getline(findFloats, lineRead, ','))
						{
							lineStrings.push_back(lineRead);
							// convert the strings into a float
							lineFloats.push_back(std::stof(lineStrings[counter]));

							counter++;
						}
						// deposit the information into a new matrix
						switch (i)
						{
						case 0:
							tempModelInfo.modelLocations.row1.x = lineFloats[0];
							tempModelInfo.modelLocations.row1.y = lineFloats[1];
							tempModelInfo.modelLocations.row1.z = lineFloats[2];
							tempModelInfo.modelLocations.row1.w = lineFloats[3];
							break;
						case 1:
							tempModelInfo.modelLocations.row2.x = lineFloats[0];
							tempModelInfo.modelLocations.row2.y = lineFloats[1];
							tempModelInfo.modelLocations.row2.z = lineFloats[2];
							tempModelInfo.modelLocations.row2.w = lineFloats[3];
							break;
						case 2:
							tempModelInfo.modelLocations.row3.x = lineFloats[0];
							tempModelInfo.modelLocations.row3.y = lineFloats[1];
							tempModelInfo.modelLocations.row3.z = lineFloats[2];
							tempModelInfo.modelLocations.row3.w = lineFloats[3];
							break;
						case 3:
							tempModelInfo.modelLocations.row4.x = lineFloats[0];
							tempModelInfo.modelLocations.row4.y = lineFloats[1];
							tempModelInfo.modelLocations.row4.z = lineFloats[2];
							tempModelInfo.modelLocations.row4.w = lineFloats[3];
							std::cout << "Saved MESH matrix to modelLocations vector!" << std::endl;
							break;
						}

					}
					//7.) After the loop exists you should have collected all the level data, now time to transfer to GPU.  

					H2B::Parser gameLevelParse;
					gameLevelParse.Parse(tempModelInfo.modelNames.c_str());

					// Get material information
					for (int v = 0; v < gameLevelParse.materialCount; v++)
					{
						tempModelInfo.modelMaterial.push_back(gameLevelParse.materials[v]);
					}
					// Get mesh information
					for (int v = 0; v < gameLevelParse.meshCount; v++)
					{
						tempModelInfo.modelMeshes.push_back(gameLevelParse.meshes[v]);
					}
					// Get vertecies
					tempModelInfo.vertexBufferStart = vertexInfo.size();
					for (int v = 0; v < gameLevelParse.vertexCount; v++)
					{
						//tempModelInfo.modelVertex.push_back(gameLevelParse.vertices[v]);
						vertexInfo.push_back(gameLevelParse.vertices[v]);
					}
					// Get Indecies
					tempModelInfo.indexBufferStart = indexInfo.size();
					for (int i = 0; i < gameLevelParse.indexCount; i++)
					{
						//tempModelInfo.modelIndecies.push_back(gameLevelParse.indices[i]);
						indexInfo.push_back(gameLevelParse.indices[i]);
					}

					modelInformation.push_back(tempModelInfo);
				}
			}
			std::cout << "File is open - time to close" << std::endl;
		}
		else
		{
			std::cout << "Could not find file name" << std::endl;
		}

		// 8. Close the file when complete
		reader.close();

		/// <summary>
		/// READING FILE END
		/// </summary>   
#pragma endregion

		unsigned int vertexSize = (vertexInfo.size() * sizeof(H2B::VERTEX));
		unsigned int indexSize = (indexInfo.size() * sizeof(unsigned int));

		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vertexSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		// Transfer triangle data to the vertex buffer.

		vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, vertexInfo.data(), vertexSize);
		vertexBuffer->Unmap(0, nullptr);
		// Create a vertex View to send to a Draw() call.
		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(H2B::VECTOR) * 3; // Part 1e orig 2 DONE
		vertexView.SizeInBytes = vertexSize; // Part 1d DONE
		// Part 1g
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(indexSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));
		// Transfer triangle data to the vertex buffer.
		//UINT8* transferMemoryLocation;
		indexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, indexInfo.data(), sizeof(indexSize));
		indexBuffer->Unmap(0, nullptr);
		//// Create a vertex View to send to a Draw() call.
		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		//indexView.StrideInBytes = sizeof(unsigned int) * 1; // Part 1e orig 2 DONE
		indexView.Format = DXGI_FORMAT_R32_UINT;
		indexView.SizeInBytes = indexSize; // Part 1d DONE

		//creator->CreateCommittedResource( // using UPLOAD heap for simplicity
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
		//	D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(FSLogo_vertices)),
		//	D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		//// Transfer triangle data to the vertex buffer.

		//vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
		//	reinterpret_cast<void**>(&transferMemoryLocation));
		//memcpy(transferMemoryLocation, FSLogo_vertices, sizeof(FSLogo_vertices));
		//vertexBuffer->Unmap(0, nullptr);
		//// Create a vertex View to send to a Draw() call.
		//vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		//vertexView.StrideInBytes = sizeof(_OBJ_VEC3_) * 3; // Part 1e orig 2 DONE
		//vertexView.SizeInBytes = sizeof(FSLogo_vertices); // Part 1d DONE
		//// Part 1g
		//creator->CreateCommittedResource( // using UPLOAD heap for simplicity
		//	&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
		//	D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(FSLogo_indices)),
		//	D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));
		//// Transfer triangle data to the vertex buffer.
		////UINT8* transferMemoryLocation;
		//indexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
		//	reinterpret_cast<void**>(&transferMemoryLocation));
		//memcpy(transferMemoryLocation, FSLogo_indices, sizeof(FSLogo_indices));
		//indexBuffer->Unmap(0, nullptr);
		////// Create a vertex View to send to a Draw() call.
		//indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		////indexView.StrideInBytes = sizeof(unsigned int) * 1; // Part 1e orig 2 DONE
		//indexView.Format = DXGI_FORMAT_R32_UINT;
		//indexView.SizeInBytes = sizeof(FSLogo_indices); // Part 1d DONE

		// TODO: Part 2d
		IDXGISwapChain4* temp;
		_d3d.GetSwapchain4(reinterpret_cast<void**>(&temp));
		temp->GetDesc(&swapChainDesc);


		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + FSLogo_meshcount * sizeof(MESH_DATA) * temp1.BufferCount);

		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + 1
				* sizeof(MESH_DATA) * swapChainDesc.BufferCount),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBuffer));

		// Transfer triangle data to the vertex buffer.
		constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));

		unsigned int frame_meshdata = 0;
		//					//swapChainDesc.BufferCount
		//for (int i = 0; i < 2; i++)
		//{
		//	memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
		//	memcpy(&transferMemoryLocation[frame_meshdata], &meshDataText, sizeof(MESH_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
		//	memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
		//}

		//meshDataLogo.material = modelInformation[0].modelMaterial[0].attrib;
		//meshDataLogo.gwWorldMatrix = gwWorldMatrix;
		//meshDataLogo.gwWorldMatrixTwo = gwWorldMatrix;

		meshDataLogo.material = modelInformation[0].modelMaterial[0].attrib;
		meshDataLogo.gwWorldMatrix = modelInformation[0].modelLocations;

		memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
		frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
		memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
		//frame_meshdata = frame_meshdata + sizeof(MESH_DATA);


		//// Create a vertex View to send to a Draw() call.
		constantView.BufferLocation = constantBuffer->GetGPUVirtualAddress();
		constantView.SizeInBytes = sizeof(MESH_DATA); // Part 1d DONE

		//indexView.StrideInBytes = sizeof(unsigned int) * 1; // Part 1e orig 2 DONE
		//constantView.Format = DXGI_FORMAT_R32_UINT;
		// TODO: Part 2e
		descriptorView.NumDescriptors = 1;
		descriptorView.NodeMask = 0;
		descriptorView.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descriptorView.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		creator->CreateDescriptorHeap(&descriptorView, IID_PPV_ARGS(&descriptorHeap));
		// TODO: Part 2f
		creator->CreateConstantBufferView(&constantView, descriptorHeap->GetCPUDescriptorHandleForHeapStart());
		// Create Vertex Shader
		UINT compilerFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compilerFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, errors;
		if (FAILED(D3DCompile(vertexShaderSource, strlen(vertexShaderSource),
			nullptr, nullptr, nullptr, "main", "vs_5_1", compilerFlags, 0,
			vsBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Create Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob; errors.Reset();
		if (FAILED(D3DCompile(pixelShaderSource, strlen(pixelShaderSource),
			nullptr, nullptr, nullptr, "main", "ps_5_1", compilerFlags, 0,
			psBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Part 1e DONE
		// Create Input Layout
		D3D12_INPUT_ELEMENT_DESC format[] = {
			{
				"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"UVMAP", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			},
			{
				"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
				D3D12_APPEND_ALIGNED_ELEMENT,
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
		};


			// TODO: Part 2g
		CD3DX12_ROOT_PARAMETER rootParameters[2];
		rootParameters[0].InitAsConstantBufferView(0, 0);
		rootParameters[1].InitAsConstantBufferView(1, 0);
		//rootParameterVertex.InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		//rootParameterVertex.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		//CD3DX12_ROOT_PARAMETER rootParameterPixel;
		//rootParameterPixel.InitAsConstantBufferView(1, 1, D3D12_SHADER_VISIBILITY_PIXEL);
		//rootParameterPixel.ShaderVisibility = D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;

		// create root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(2, rootParameters, 0, nullptr,
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		Microsoft::WRL::ComPtr<ID3DBlob> signature;
		D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errors);
		creator->CreateRootSignature(0, signature->GetBufferPointer(),
			signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		// create pipeline state
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc;
		ZeroMemory(&psDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
		psDesc.InputLayout = { format, ARRAYSIZE(format) };
		psDesc.pRootSignature = rootSignature.Get();
		psDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		psDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());
		psDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		psDesc.SampleMask = UINT_MAX;
		psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psDesc.NumRenderTargets = 1;
		psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
		psDesc.SampleDesc.Count = 1;
		creator->CreateGraphicsPipelineState(&psDesc, IID_PPV_ARGS(&pipeline));
		// free temporary handle
		creator->Release();
	}
	void Render()
	{
		// Part 2a DONE
		//GW::MATH::GMatrix gwGmatrix;
		//gwWorldMatrix
		//gwCopyMatrix = gwWorldMatrix;
		//meshDataLogo.gwWorldMatrix = UpdateMatrix(meshDataLogo.gwWorldMatrixTwo);
		//meshDataLogo.gwWorldMatrix = gwWorldMatrix;
		// TODO: Part 4d
		// grab the context & render target
		ID3D12GraphicsCommandList* cmd;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv;
		d3d.GetCommandList((void**)&cmd);
		d3d.GetCurrentRenderTargetView((void**)&rtv);
		d3d.GetDepthStencilView((void**)&dsv);
		// setup the pipeline
		cmd->SetGraphicsRootSignature(rootSignature.Get());
		// TODO: Part 2h

		unsigned int frame_scenedata = 0;
		unsigned int frame_meshdata = 0;// = sizeof(SCENE_DATA);

		constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));



		//for (int i = 0; i < swapChainDesc.BufferCount; i++)
		//{

		//	memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
		//	memcpy(&transferMemoryLocation[frame_meshdata], &meshDataText, sizeof(MESH_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
		//	memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
		//	frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
		//}

		memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
		frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
		memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
		//frame_meshdata = frame_meshdata + sizeof(MESH_DATA);

		//memcpy(transferMemoryLocation, &sceneData, sizeof(SCENE_DATA)); // SCENE DATA 
		////memcpy(transferMemoryLocation + sizeof(SCENE_DATA), &meshData, sizeof(MESH_DATA)); // MESH DATA
		//memcpy(&transferMemoryLocation[frame_meshdata], &meshDataText, sizeof(MESH_DATA)); // MESH DATA
		//memcpy(transferMemoryLocation + sizeof(SCENE_DATA) + sizeof(MESH_DATA), &meshDataLogo, sizeof(MESH_DATA)); // MESH DATA
		//memcpy(transferMemoryLocation
		//	+ sizeof(SCENE_DATA) + sizeof(MESH_DATA) + sizeof(MESH_DATA),
		//	&sceneData, sizeof(SCENE_DATA)); // SCENE DATA
		//memcpy(transferMemoryLocation
		//	+ sizeof(SCENE_DATA) + sizeof(MESH_DATA) + sizeof(MESH_DATA) + sizeof(SCENE_DATA),
		//	&meshDataText, sizeof(MESH_DATA)); // MESH DATA
		//memcpy(transferMemoryLocation
		//	+ sizeof(SCENE_DATA) + sizeof(MESH_DATA) + sizeof(MESH_DATA) + sizeof(SCENE_DATA) + sizeof(MESH_DATA),
		//	&meshDataLogo, sizeof(MESH_DATA)); // MESH DATA

		constantBuffer->Unmap(0, nullptr);


		cmd->SetDescriptorHeaps(0, &descriptorHeap);
		//cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + sizeof(SCENE_DATA)); // MESH_DATA register
		// TODO: Part 4e
		cmd->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		cmd->SetPipelineState(pipeline.Get());
		// now we can draw
		cmd->IASetVertexBuffers(0, 1, &vertexView);
		cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// Part 1h DONE
		cmd->IASetIndexBuffer(&indexView);
		//cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		// TODO: Part 3b

		cmd->SetGraphicsRootConstantBufferView(0, constantBuffer->GetGPUVirtualAddress()); // SCENE_DATA register
		cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + sizeof(SCENE_DATA));
		//cmd->DrawIndexedInstanced(modelInformation[0].modelMeshes[0].drawInfo.indexCount, 1, modelInformation[0].modelMeshes[0].drawInfo.indexOffset, 0, 0);
		cmd->DrawInstanced(778, 1, 0, 0);
		//for (int i = 0; i < swapChainDesc.BufferCount; i++)
		//{

		//	if (i == 1)
		//	{
		//		// FULLSAIL LOGO
		//		//meshData.material.Kd = { (1,1,1) };
		//		cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + sizeof(SCENE_DATA) + sizeof(MESH_DATA));
		//		cmd->DrawIndexedInstanced(FSLogo_meshes[1].indexCount, 1, FSLogo_meshes[1].indexOffset, 0, 0);
		//	}
		//	else if (i == 0)
		//	{
		//		// FULLSAIL TEXT
		//		//meshData.material.Kd = { (0,1,0) };
		//		cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + sizeof(SCENE_DATA));
		//		cmd->DrawIndexedInstanced(FSLogo_meshes[0].indexCount, 1, FSLogo_meshes[0].indexOffset, 0, 0);
		//	}
		//}
		//cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + sizeof(MESH_DATA)); // MESH_DATA register
		//cmd->DrawIndexedInstanced(FSLogo_meshes[0].indexCount, 1, FSLogo_meshes[0].indexOffset, 0, 0);
		//cmd->DrawIndexedInstanced(FSLogo_meshes[1].indexCount, 1, FSLogo_meshes[1].indexOffset, 0, 0);


			// TODO: Part 3c
			// TODO: Part 4e

		//cmd->DrawInstanced(3885, 1, 0, 0); // Part 1c DONE 3885
		// release temp handles
		cmd->Release();
	}

	void UpdateCamera()
	{
		high_resolution_clock::time_point timeStart = high_resolution_clock::now();
		duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);

		//std::cout << timePassed.count() << std::endl;

		GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
		gwGmatrix.Create();
		gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);
		unsigned int controllerIndex = 0;
		GW::MATH::GMATRIXF tempMatrix;
		gwGmatrix.IdentityF(tempMatrix);

		const float cameraVertSpeed = 0.3f;
		float changeYPos, spaceOutput, rTriggerOutput, lTriggerOutput, shiftOutput,
			wOutput, sOutput, lStickYOutput, dOutput, aOutput, lStickXOutput,
			mouseDeltaX, mouseDeltaY, frameSpeed, rStickYOutput, rStickXOutput;
		bool controllerCheck;

		// CAMERA UP / DOWN CODE
		gInput.GetState(G_KEY_SPACE, spaceOutput);
		gInput.GetState(G_KEY_LEFTSHIFT, shiftOutput);
		gController.IsConnected(0, controllerCheck);
		if (controllerCheck)
		{
			gController.GetState(0, G_RIGHT_THUMB_BTN, rTriggerOutput);
			gController.GetState(0, G_LEFT_THUMB_BTN, lTriggerOutput);
		}
		else
		{
			lTriggerOutput = 0.0f;
			rTriggerOutput = 0.0f;
		}
		changeYPos = spaceOutput - shiftOutput + rTriggerOutput - lTriggerOutput;
		sceneData.gwViewMatrix.row4.y += (changeYPos * cameraVertSpeed) * timePassed.count();
		// CAMERA UP / DOWN CODE

		// CAMERA STRAFE CODE
		gInput.GetState(G_KEY_W, wOutput);
		gInput.GetState(G_KEY_S, sOutput);
		gInput.GetState(G_KEY_D, dOutput);
		gInput.GetState(G_KEY_A, aOutput);
		if (controllerCheck)
		{
			//gController.CONTRO
			gController.GetState(0, G_LY_AXIS, lStickYOutput);
			gController.GetState(0, G_LX_AXIS, lStickXOutput);
		}
		else
		{
			lStickXOutput = 0.0f;
			lStickYOutput = 0.0f;
		}

		frameSpeed = cameraVertSpeed * timePassed.count();
		float zChange;
		zChange = wOutput - sOutput + lStickYOutput + lStickYOutput;
		float xChange;
		xChange = dOutput - aOutput + lStickXOutput + lStickXOutput;

		GW::MATH::GVECTORF matrixTransformer;
		matrixTransformer.x = xChange * timePassed.count();
		matrixTransformer.y = 0.0f;
		matrixTransformer.z = zChange * timePassed.count();
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateLocalF(sceneData.gwViewMatrix, matrixTransformer, sceneData.gwViewMatrix);
		// CAMERA STRAFE CODE

		// CAMERA TILT CODE
		if (controllerCheck)
		{
			//gController.CONTRO
			gController.GetState(0, G_RY_AXIS, rStickYOutput);
			gController.GetState(0, G_RX_AXIS, rStickXOutput);
		}
		else
		{
			rStickXOutput = 0.0f;
			rStickYOutput = 0.0f;
		}

		float mouseSpeed = 3.14 * timePassed.count();
		gInput.GetMouseDelta(mouseDeltaX, mouseDeltaY);
		//mouseDeltaX = mouseDeltaX - mouseXLast;
		//mouseDeltaY = mouseDeltaY - mouseYLast;
		if (mouseDeltaX != mouseXLast && mouseDeltaY != mouseYLast)
		{
			float pitch = 65 * mouseDeltaY / (tempHeight + rStickYOutput) * mouseSpeed;
			float yaw = 65 * aspectRatio * mouseDeltaX / (tempWidth + rStickXOutput) * mouseSpeed;
			gwGmatrix.IdentityF(tempMatrix);
			gwGmatrix.RotationYawPitchRollF(0, pitch, 0, tempMatrix);
			gwGmatrix.MultiplyMatrixF(tempMatrix, sceneData.gwViewMatrix, sceneData.gwViewMatrix);
			gwGmatrix.IdentityF(tempMatrix);
			gwGmatrix.RotationYawPitchRollF(yaw, 0, 0, tempMatrix);
			// SAVE CAMERA POS
			matrixTransformer.x = sceneData.gwViewMatrix.row4.x;
			matrixTransformer.y = sceneData.gwViewMatrix.row4.y;
			matrixTransformer.z = sceneData.gwViewMatrix.row4.z;
			matrixTransformer.w = sceneData.gwViewMatrix.row4.w;
			gwGmatrix.MultiplyMatrixF(sceneData.gwViewMatrix, tempMatrix, sceneData.gwViewMatrix);
			// RESTORE CAMERA POS
			sceneData.gwViewMatrix.row4.x = matrixTransformer.x;
			sceneData.gwViewMatrix.row4.y = matrixTransformer.y;
			sceneData.gwViewMatrix.row4.z = matrixTransformer.z;
			sceneData.gwViewMatrix.row4.w = matrixTransformer.w;
			//gwViewMatrix = gwViewMatrixCopy;
		}
		//gwViewMatrixCopy = gwViewMatrix;
		mouseXLast = mouseDeltaX;
		mouseYLast = mouseDeltaY;
		// CAMERA TILT CODE

		timeEnd = high_resolution_clock::now();
		gwGmatrix.InverseF(sceneData.gwViewMatrix, sceneData.gwViewMatrix);
	}

	~Renderer()
	{
		// ComPtr will auto release so nothing to do here 
	}
};
