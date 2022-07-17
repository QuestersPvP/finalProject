// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dx12.h" // official helper file provided by microsoft
// Part 1b DONE
//#include "FSLogo.h"
#include "h2bParser.h"
#include "RH.h"

using namespace std::chrono;

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

	D3D12_INDEX_BUFFER_VIEW						indexView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		indexBuffer;

	D3D12_CONSTANT_BUFFER_VIEW_DESC				constantView;
	Microsoft::WRL::ComPtr<ID3D12Resource>		constantBuffer;
	D3D12_DESCRIPTOR_HEAP_DESC					descriptorView;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>descriptorHeap;

	D3D12_SHADER_RESOURCE_VIEW_DESC				structuredViewMatrix;
	Microsoft::WRL::ComPtr<ID3D12Resource>		structuredBufferMatrix;
	D3D12_SHADER_RESOURCE_VIEW_DESC				structuredViewMaterial;
	Microsoft::WRL::ComPtr<ID3D12Resource>		structuredBufferMaterial;


	D3D12_DESCRIPTOR_HEAP_DESC					structuredDescriptorView;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>structuredHeap;


	Microsoft::WRL::ComPtr<ID3D12RootSignature>	rootSignature;
	Microsoft::WRL::ComPtr<ID3D12PipelineState>	pipeline;

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

	MESH_DATA meshDataText;
	MESH_DATA meshDataLogo;
	ModelLocation tempLocation;
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
		std::string vertexShaderString;
		std::string pixelShaderString;
		vertexShaderString = ShaderAsString(vertexShader);
		pixelShaderString = ShaderAsString(pixelShader);

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
		//sceneData.lightColor = lightColor;
		//sceneData.lightDirection = lightDirection;
		//sceneData.lightAmbient = lightAmbient;
		//sceneData.lightPos = lightPos;
		//sceneData.camPos = eye;

		meshDataText.gwWorldMatrix = gwWorldMatrix;
		meshDataText.gwWorldMatrixTwo = gwWorldMatrix;
		//meshDataText.material = FSLogo_materials[0].attrib;

		//meshDataLogo.material = FSLogo_materials[1].attrib;
		meshDataLogo.gwWorldMatrix = gwWorldMatrix;
		meshDataLogo.gwWorldMatrixTwo = gwWorldMatrix;
#pragma endregion

		// FILE IO
		ParseFiles();


		unsigned int vertexSize = (vertexInfo.size() * sizeof(H2B::VERTEX));
		unsigned int indexSize = (indexInfo.size() * sizeof(unsigned int));

		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vertexSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		// Transfer triangle data to the vertex buffer.

		UINT8* transferMemoryLocation;

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
		indexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, indexInfo.data(), sizeof(indexSize));
		indexBuffer->Unmap(0, nullptr);
		//// Create a vertex View to send to a Draw() call.
		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexView.Format = DXGI_FORMAT_R32_UINT;
		indexView.SizeInBytes = indexSize; // Part 1d DONE

		// TODO: Part 2d
		IDXGISwapChain4* temp;
		_d3d.GetSwapchain4(reinterpret_cast<void**>(&temp));
		temp->GetDesc(&swapChainDesc);


		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + FSLogo_meshcount * sizeof(MESH_DATA) * temp1.BufferCount);

#pragma region ConstantBuffer
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + 1 // need to replace with meshCount?
				* sizeof(MESH_DATA) * swapChainDesc.BufferCount),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBuffer));

		// Transfer triangle data to the vertex buffer.
		constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));

		unsigned int frame_meshdata = 0;

		memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA

		constantBuffer->Unmap(0, nullptr);


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
#pragma endregion

#pragma region StructuredBuffer
		// model locations resource
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ModelLocation) * modelLocations.gwWorldMatrix.size()), //* modelLocations.gwWorldMatrix.size()
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&structuredBufferMatrix));

		// model material resource
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(ModelMaterial) * modelMaterials.material.size()), //* modelMaterials.material.size()
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&structuredBufferMaterial));

		UINT8* transferMemoryLocation1;
		UINT8* transferMemoryLocation2;

		// Transfer triangle data to the vertex buffer.
		structuredBufferMatrix->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation1));
		// memcopy model matrixes
		memcpy(&transferMemoryLocation1, &modelLocations.gwWorldMatrix, sizeof(ModelLocation)); // SCENE DATA
		structuredBufferMatrix->Unmap(0, nullptr);

		// Transfer triangle data to the vertex buffer.
		structuredBufferMaterial->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation2));
		// memcopy model materials
		memcpy(&transferMemoryLocation2, &modelMaterials.material, sizeof(ModelMaterial)); // SCENE DATA
		structuredBufferMaterial->Unmap(0, nullptr);

		// set matrix buffer and format
		structuredViewMatrix.Buffer.StructureByteStride = sizeof(ModelLocation);
		structuredViewMatrix.Buffer.FirstElement = 0;
		structuredViewMatrix.Buffer.NumElements = modelLocations.gwWorldMatrix.size();
		structuredViewMatrix.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		structuredViewMatrix.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		structuredViewMatrix.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		structuredViewMatrix.Format = DXGI_FORMAT_UNKNOWN;
		// set material buffer and format
		structuredViewMaterial.Buffer.StructureByteStride = sizeof(ModelMaterial);
		structuredViewMaterial.Buffer.FirstElement = 0;
		structuredViewMaterial.Buffer.NumElements = modelLocations.gwWorldMatrix.size();
		structuredViewMaterial.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
		structuredViewMaterial.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		structuredViewMaterial.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		structuredViewMaterial.Format = DXGI_FORMAT_UNKNOWN;
		// create the heap
		structuredDescriptorView.NumDescriptors = 1;
		structuredDescriptorView.NodeMask = 0;
		structuredDescriptorView.Type = D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		structuredDescriptorView.Flags = D3D12_DESCRIPTOR_HEAP_FLAGS::D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		creator->CreateDescriptorHeap(&structuredDescriptorView, IID_PPV_ARGS(&structuredHeap));
		// TODO: Part 2f
		creator->CreateShaderResourceView(structuredBufferMatrix.Get(), &structuredViewMatrix, structuredHeap->GetCPUDescriptorHandleForHeapStart());
		creator->CreateShaderResourceView(structuredBufferMaterial.Get(), &structuredViewMaterial, structuredHeap->GetCPUDescriptorHandleForHeapStart());
#pragma endregion



		// Create Vertex Shader
		UINT compilerFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
		compilerFlags |= D3DCOMPILE_DEBUG;
#endif
		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, errors;
		if (FAILED(D3DCompile(vertexShaderString.c_str(), strlen(vertexShaderString.c_str()),
			nullptr, nullptr, nullptr, "main", "vs_5_1", compilerFlags, 0,
			vsBlob.GetAddressOf(), errors.GetAddressOf())))
		{
			std::cout << (char*)errors->GetBufferPointer() << std::endl;
			abort();
		}
		// Create Pixel Shader
		Microsoft::WRL::ComPtr<ID3DBlob> psBlob; errors.Reset();
		if (FAILED(D3DCompile(pixelShaderString.c_str(), strlen(pixelShaderString.c_str()),
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
		CD3DX12_ROOT_PARAMETER rootParameters[3];
		rootParameters[0].InitAsConstantBufferView(0, 0);
		rootParameters[1].InitAsConstantBufferView(1, 0);
		rootParameters[2].InitAsShaderResourceView(0,0);

		// create root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(3, rootParameters, 0, nullptr,
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

		UINT8* transferMemoryLocation;
		UINT8* transferMemoryLocation1;
		UINT8* transferMemoryLocation2;

		constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));

		memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA

		constantBuffer->Unmap(0, nullptr);

		// Transfer triangle data to the vertex buffer.
		structuredBufferMatrix->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation1));
		// memcopy model matrixes
		memcpy(&transferMemoryLocation1, &modelLocations.gwWorldMatrix, sizeof(ModelLocation)); // SCENE DATA
		structuredBufferMatrix->Unmap(0, nullptr);

		// Transfer triangle data to the vertex buffer.
		structuredBufferMaterial->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation2));
		// memcopy model materials
		memcpy(&transferMemoryLocation2, &modelMaterials.material, sizeof(ModelMaterial)); // SCENE DATA
		structuredBufferMaterial->Unmap(0, nullptr);


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

		cmd->DrawInstanced(778, 1, 0, 0);

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
