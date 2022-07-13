// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dx12.h" // official helper file provided by microsoft

#include <fstream>		// FILE I/O
#include <vector>		// TO HOLD FILE I/O INFO
#include "h2bParser.h"	// USED GET MODEL INFO AFTER GameLevel.txt IS READ

using namespace std::chrono;
// Simple Vertex Shader
const char* vertexShaderSource = R"(
// an ultra simple hlsl vertex shader
// TODO: Part 2b
cbuffer SHADER_VARS
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

	// TODO: Part 2f
	// TODO: Part 3b
// Part 1c DONE
float4 main(float4 inputVertex : POSITION) : SV_POSITION
{
	// TODO: Part 2d, Part 2f, Part 3b worldMatrix  inputVertex

	inputVertex = mul(worldMatrix, inputVertex);
	inputVertex = mul(viewMatrix, inputVertex);
	inputVertex = mul(projectionMatrix, inputVertex);

	return float4(inputVertex);
}
)";
// Simple Pixel Shader
const char* pixelShaderSource = R"(
// an ultra simple hlsl pixel shader
float4 main() : SV_TARGET 
{	
	return float4(0.5f,0.25f,0.75f,0); // TODO: Part 1a
}
)";
// Creation, Rendering & Cleanup
class Renderer
{
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GDirectX12Surface d3d;
	// Part 4a
	GW::INPUT::GInput gInput;
	GW::INPUT::GController gController;
	//cPart 2a DONE
	GW::MATH::GMATRIXF gwTempWorldMatrix;
	GW::MATH::GMATRIXF gwWorldMatrixOne;
	GW::MATH::GMATRIXF gwWorldMatrixTwo;
	GW::MATH::GMATRIXF gwWorldMatrixThree;
	GW::MATH::GMATRIXF gwWorldMatrixFour;
	GW::MATH::GMATRIXF gwWorldMatrixFive;
	GW::MATH::GMATRIXF gwWorldMatrixSix;
	// Part 3c DONE
	unsigned int tempHeight = 0;
	unsigned int tempWidth = 0;
	float aspectRatio = 0.0f;
	float mouseYLast = 0.0f;
	float mouseXLast = 0.0f;
	// Part 2e DONE
	//GW::MATH::GMATRIXF gwTempViewMatrix;
	GW::MATH::GMATRIXF gwViewMatrix;
	GW::MATH::GMATRIXF gwViewMatrixCopy;
	// Part 3a DONE
	GW::MATH::GMATRIXF gwProjectionMatrix;
	// what we need at a minimum to draw a triangle
	D3D12_VERTEX_BUFFER_VIEW					vertexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	pipeline;
public:
	// Part 1c DONE
	struct Vertex 
	{
		float x, y, z, w;
	};

	// Part 2b DONE
	struct SHADER_VARS 
	{
		GW::MATH::GMATRIXF gwWorldMatrix;
		GW::MATH::GMATRIXF gwViewMatrix;
		GW::MATH::GMATRIXF gwProjectionMatrix;
	};

	float DegreeToRadians(float degree)
	{
		return degree * (3.14 / 180);
	}

	

	// Part 2f DONE
	//  Part 3b DONE
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GDirectX12Surface _d3d)
	{
		win = _win;
		d3d = _d3d;
		ID3D12Device* creator;
		d3d.GetDevice((void**)&creator);
		//  Part 4a
		gInput.Create(_win);
		gController.Create();
		// Part 2a DONE
		GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
		gwGmatrix.Create();
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		gwGmatrix.RotateXGlobalF(gwTempWorldMatrix, DegreeToRadians(90.0f), gwTempWorldMatrix);
		GW::MATH::GVECTORF worldMatrixTranslation;
		worldMatrixTranslation.x = 0.0f;
		worldMatrixTranslation.y = -0.5f;
		worldMatrixTranslation.z = 0.0f;
		worldMatrixTranslation.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, worldMatrixTranslation, gwTempWorldMatrix);
		gwWorldMatrixOne = gwTempWorldMatrix;
		//worldMatrixTranslation = { (0.0f, 0.5f, 0.0f) };
		//gwGmatrix.Create();
		//  Part 3c DONE
		GW::MATH::GVECTORF matrixTransformer;

#pragma region Lab1
		// LEFT WALL
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		gwGmatrix.RotateYGlobalF(gwTempWorldMatrix, DegreeToRadians(90.0f), gwTempWorldMatrix);
		matrixTransformer.x = -0.5f;
		matrixTransformer.y = 0.0f;
		matrixTransformer.z = 0.0f;
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, matrixTransformer, gwTempWorldMatrix);
		gwWorldMatrixTwo = gwTempWorldMatrix;

		// BACK WALL
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		//gwGmatrix.RotateYGlobalF(gwTempWorldMatrix, DegreeToRadians(0.0f), gwTempWorldMatrix);
		matrixTransformer.x = 0.0f;
		matrixTransformer.y = 0.0f;
		matrixTransformer.z = 0.5f;
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, matrixTransformer, gwTempWorldMatrix);
		gwWorldMatrixThree = gwTempWorldMatrix;

		// TOP WALL
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		gwGmatrix.RotateXGlobalF(gwTempWorldMatrix, DegreeToRadians(90.0f), gwTempWorldMatrix);
		matrixTransformer.x = 0.0f;
		matrixTransformer.y = 0.5f;
		matrixTransformer.z = 0.0f;
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, matrixTransformer, gwTempWorldMatrix);
		gwWorldMatrixFour = gwTempWorldMatrix;


		// RIGHT WALL
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		gwGmatrix.RotateYGlobalF(gwTempWorldMatrix, DegreeToRadians(90.0f), gwTempWorldMatrix);
		matrixTransformer.x = 0.5f;
		matrixTransformer.y = 0.0f;
		matrixTransformer.z = 0.0f;
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, matrixTransformer, gwTempWorldMatrix);
		gwWorldMatrixFive = gwTempWorldMatrix;

		// FRONT WALL
		gwGmatrix.IdentityF(gwTempWorldMatrix);
		//gwGmatrix.RotateYGlobalF(gwTempWorldMatrix, DegreeToRadians(0.0f), gwTempWorldMatrix);
		matrixTransformer.x = 0.0f;
		matrixTransformer.y = 0.0f;
		matrixTransformer.z = -0.5f;
		matrixTransformer.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwTempWorldMatrix, matrixTransformer, gwTempWorldMatrix);
		gwWorldMatrixSix = gwTempWorldMatrix;
#pragma endregion


		// Part 2e DONE
		gwGmatrix.IdentityF(gwViewMatrix);
		//gwGmatrix.RotateZGlobalF(gwViewMatrix, DegreeToRadians(-18.0f), gwViewMatrix);
		//worldMatrixTranslation.x = 0.25f; // 0.25f
		//worldMatrixTranslation.y = -0.125f; // -0.125f;
		//worldMatrixTranslation.z = -0.25f; // -0.25f;
		//worldMatrixTranslation.w = 1.0f;
		GW::MATH::GVECTORF eye;
		eye.x = 0.25f;
		eye.y = -0.125f;
		eye.z = -0.25f;
		eye.w = 1.0f;
		gwGmatrix.TranslateGlobalF(gwViewMatrix, eye, gwViewMatrix);
		//gwViewMatrix = gwTempViewMatrix;
		//gwGmatrix.InverseF(gwTempViewMatrix, gwTempViewMatrix);
		GW::MATH::GVECTORF up;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;
		up.w = 0;

		//gwGmatrix.LookAtLHF(eye, worldMatrixTranslation, up, gwViewMatrix);
		gwViewMatrixCopy = gwViewMatrix;

		//GW::GRAPHICS::GDirectX12Surface::GetAspectRatio(temp);
		_win.GetClientHeight(tempHeight);
		_win.GetClientWidth(tempWidth);
		//_win.GetClientWidth();
		// 
		gwGmatrix.IdentityF(gwProjectionMatrix);
		aspectRatio = ((float)tempWidth / (float)tempHeight);
		gwGmatrix.ProjectionDirectXLHF(DegreeToRadians(65), aspectRatio, 0.1f, 100, gwProjectionMatrix);


		/// <summary>
		/// READING FILE START
		/// </summary>
		#pragma region File I/O
		// run h2b file parser on the meshes you read in
		// array of matrix
		// parse gamelevel.txt
		//1.) Use either std::ifstream or FILE * to open the file.
		std::ifstream reader;
		std::string lineRead;
		std::string mFileName = "../GameLevel.txt";
		reader.open(mFileName);

		std::vector<std::string> modelNames;			// vector of model names
		std::vector<GW::MATH::GMATRIXF> modelLocations;	// vector of model location matrixes
		int modelsReadIn = 0;							// keep track of how many models
		bool sameModelName = false;						// bool to keep track of if a model name has been read in already or not

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
					//5.) Read the next line, this is the name of the .h2b file to load in. (duplicates?)
					std::cout << "FOUND A MESH" << std::endl;
					std::getline(reader, lineRead);
					for (int i = 0; i < modelNames.size(); i++)
					{
						if (lineRead == modelNames[i])
							sameModelName = true;
					}
					if (sameModelName == false)
					{
						std::cout << "Model Name: " << lineRead << std::endl;
						modelNames.push_back(lineRead);

						GW::MATH::GMATRIXF theNewMatrix;

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
								theNewMatrix.row1.x = lineFloats[0];
								theNewMatrix.row1.y = lineFloats[1];
								theNewMatrix.row1.z = lineFloats[2];
								theNewMatrix.row1.w = lineFloats[3];
								break;
							case 1:
								theNewMatrix.row2.x = lineFloats[0];
								theNewMatrix.row2.y = lineFloats[1];
								theNewMatrix.row2.z = lineFloats[2];
								theNewMatrix.row2.w = lineFloats[3];
								break;
							case 2:
								theNewMatrix.row3.x = lineFloats[0];
								theNewMatrix.row3.y = lineFloats[1];
								theNewMatrix.row3.z = lineFloats[2];
								theNewMatrix.row3.w = lineFloats[3];
								break;
							case 3:
								theNewMatrix.row4.x = lineFloats[0];
								theNewMatrix.row4.y = lineFloats[1];
								theNewMatrix.row4.z = lineFloats[2];
								theNewMatrix.row4.w = lineFloats[3];
								std::cout << "Saved MESH matrix to modelLocations vector!" << std::endl;
								modelLocations.push_back(theNewMatrix);
								break;
							}
						}
					}
					else
					{
						std::cout << "ERROR: Mesh name " << lineRead << " is a duplicate!" << std::endl;
						sameModelName = false;
					}
				}
			}
			std::cout << "ERROR: File is open - time to close" << std::endl;
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





		Vertex verts[104];
		float vertPointsHoriz = -0.5f;
		float vertPointsVert = -0.5f;

		for (int i = 0; i < 104; i+=2)
		{

			if (i >= 52)
			{
				verts[i] = { vertPointsVert, 0.5f, 0, 1};       //verts[i] = { vertPointsVert, 0, 0.5f, 1};     
				verts[i+1] = { vertPointsVert, -0.5f, 0, 1 };	//verts[i+1] = { vertPointsVert, 0, -0.5f, 1 };

				vertPointsVert += 0.04f;
			}
			else
			{
				verts[i] = { 0.5f, vertPointsHoriz, 0, 1 };              // verts[i] = { 0.5f, 0, vertPointsHoriz, 1 };		
				verts[i + 1] = { -0.5f, vertPointsHoriz, 0, 1 };	 // verts[i + 1] = { -0.5f, 0, vertPointsHoriz, 1 };

				vertPointsHoriz += 0.04f;
			}
		}

		// TODO: Part 1d
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(sizeof(verts)),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		// Transfer triangle data to the vertex buffer.
		UINT8* transferMemoryLocation;
		vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0), 
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, verts, sizeof(verts));
		vertexBuffer->Unmap(0, nullptr);
		// Part 1c DONE
		// Create a vertex View to send to a Draw() call.
		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(float) * 4; // Orig 2
		vertexView.SizeInBytes = sizeof(verts);
		// Create Vertex Shader
		UINT compilerFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compilerFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, errors;
		if (FAILED(D3DCompile(vertexShaderSource, strlen(vertexShaderSource),
			nullptr, nullptr, nullptr, "main", "vs_5_0", compilerFlags, 0, 
			vsBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Create Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob; errors.Reset();
		if (FAILED(D3DCompile(pixelShaderSource, strlen(pixelShaderSource),
			nullptr, nullptr, nullptr, "main", "ps_5_0", compilerFlags, 0, 
			psBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Part 1c DONE
		// Create Input Layout
		D3D12_INPUT_ELEMENT_DESC format[] = {
			{ 
				"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, // Orig DXGI_FORMAT_R32G32B32A32_FLOAT
				D3D12_APPEND_ALIGNED_ELEMENT, 
				D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
			}
		};
		// Part 2c DONE
		CD3DX12_ROOT_PARAMETER rootParameter;
		//rootParameter.InitAsConstants(16, ID3D12DescriptorHeap, ; // idea (16, vertexShaderSource)
		rootParameter.InitAsConstants(48, 0, 0, D3D12_SHADER_VISIBILITY_VERTEX);
		// create root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(1, &rootParameter, 0, nullptr,
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
		psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE; //Part 1b FINISHED
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
		// grab the context & render target
		ID3D12GraphicsCommandList* cmd;
		D3D12_CPU_DESCRIPTOR_HANDLE rtv;
		D3D12_CPU_DESCRIPTOR_HANDLE dsv;
		d3d.GetCommandList((void**)&cmd);
		d3d.GetCurrentRenderTargetView((void**)&rtv);
		d3d.GetDepthStencilView((void**)&dsv);
		// setup the pipeline
		cmd->SetGraphicsRootSignature(rootSignature.Get());
		cmd->OMSetRenderTargets(1, &rtv, FALSE, &dsv);
		cmd->SetPipelineState(pipeline.Get());
		// Part 3a DONE
		// Part 2b DONE
		SHADER_VARS shaderVars;
		shaderVars.gwWorldMatrix = gwWorldMatrixOne;
		shaderVars.gwViewMatrix = gwViewMatrixCopy;
		shaderVars.gwProjectionMatrix = gwProjectionMatrix;
		// Part 2d DONE
		cmd->SetGraphicsRoot32BitConstants(0,48,&shaderVars,0);
		// now we can draw
		cmd->IASetVertexBuffers(0, 1, &vertexView);
		// Part 1b DONE
		cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);  // ORIG - D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST

		cmd->DrawInstanced(104, 1, 0, 0);

		shaderVars.gwWorldMatrix = gwWorldMatrixTwo;
		//shaderVars.gwViewMatrix = gwTempViewMatrix;
		//shaderVars.gwProjectionMatrix = gwProjectionMatrix;
		// Part 2d DONE
		cmd->SetGraphicsRoot32BitConstants(0, 48, &shaderVars, 0);
		cmd->DrawInstanced(104, 1, 0, 0);

		shaderVars.gwWorldMatrix = gwWorldMatrixThree;
		cmd->SetGraphicsRoot32BitConstants(0, 48, &shaderVars, 0);
		cmd->DrawInstanced(104, 1, 0, 0);

		shaderVars.gwWorldMatrix = gwWorldMatrixFour;
		cmd->SetGraphicsRoot32BitConstants(0, 48, &shaderVars, 0);
		cmd->DrawInstanced(104, 1, 0, 0);

		shaderVars.gwWorldMatrix = gwWorldMatrixFive;
		cmd->SetGraphicsRoot32BitConstants(0, 48, &shaderVars, 0);
		cmd->DrawInstanced(104, 1, 0, 0);

		shaderVars.gwWorldMatrix = gwWorldMatrixSix;
		cmd->SetGraphicsRoot32BitConstants(0, 48, &shaderVars, 0);
		cmd->DrawInstanced(104, 1, 0, 0);

		//  Part 3d DONE
		// release temp handles
		cmd->Release();
	}
	// TODO: Part 4b
		// TODO: Part 4c
		// TODO: Part 4d
		// TODO: Part 4e
		// TODO: Part 4f
		// TODO: Part 4g
		// TODO: Part 4c

	high_resolution_clock::time_point timeEnd = high_resolution_clock::now();

	void UpdateCamera()
	{
		high_resolution_clock::time_point timeStart = high_resolution_clock::now();
		duration<double> timePassed = duration_cast<duration<double>>(timeStart - timeEnd);

		//std::cout << timePassed.count() << std::endl;

		GW::MATH::GMatrix gwGmatrix;// = gwGmatrix.Create();
		gwGmatrix.Create();
		gwGmatrix.InverseF(gwViewMatrixCopy, gwViewMatrixCopy);
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
		gwViewMatrixCopy.row4.y += (changeYPos * cameraVertSpeed) * timePassed.count();
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
		gwGmatrix.TranslateLocalF(gwViewMatrixCopy, matrixTransformer, gwViewMatrixCopy);
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
			gwGmatrix.MultiplyMatrixF(tempMatrix, gwViewMatrixCopy, gwViewMatrixCopy);
			gwGmatrix.IdentityF(tempMatrix);
			gwGmatrix.RotationYawPitchRollF(yaw, 0, 0, tempMatrix);
			// SAVE CAMERA POS
			matrixTransformer.x = gwViewMatrixCopy.row4.x;
			matrixTransformer.y = gwViewMatrixCopy.row4.y;
			matrixTransformer.z = gwViewMatrixCopy.row4.z;
			matrixTransformer.w = gwViewMatrixCopy.row4.w;
			gwGmatrix.MultiplyMatrixF(gwViewMatrixCopy, tempMatrix, gwViewMatrixCopy);
			// RESTORE CAMERA POS
			gwViewMatrixCopy.row4.x = matrixTransformer.x;
			gwViewMatrixCopy.row4.y = matrixTransformer.y;
			gwViewMatrixCopy.row4.z = matrixTransformer.z;
			gwViewMatrixCopy.row4.w = matrixTransformer.w;
			//gwViewMatrix = gwViewMatrixCopy;
		}
		//gwViewMatrixCopy = gwViewMatrix;
		mouseXLast = mouseDeltaX;
		mouseYLast = mouseDeltaY;
		// CAMERA TILT CODE

		timeEnd = high_resolution_clock::now();
		gwGmatrix.InverseF(gwViewMatrixCopy, gwViewMatrixCopy);
	}

	~Renderer()
	{
		// ComPtr will auto release so nothing to do here 
	}
};
