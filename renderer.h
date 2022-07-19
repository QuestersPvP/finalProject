// minimalistic code to draw a single triangle, this is not part of the API.
// required for compiling shaders on the fly, consider pre-compiling instead
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dx12.h" // official helper file provided by microsoft
// Part 1b DONE
#include "RH.h"


// Creation, Rendering & Cleanup
class Renderer
{
	// proxy handles
	GW::SYSTEM::GWindow win;
	GW::GRAPHICS::GDirectX12Surface d3d;
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
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	// TODO: Part 2b
	//UINT8* transferMemoryLocation1;
	UINT8* transferMemoryLocation;

	// TODO: Part 4f
public:
	Renderer(GW::SYSTEM::GWindow _win, GW::GRAPHICS::GDirectX12Surface _d3d)
	{
		win = _win;
		d3d = _d3d;
		ID3D12Device* creator;
		d3d.GetDevice((void**)&creator);

		// create input device
		gInput.Create(_win);

		// Set up Shaders
		std::string vertexShaderString;
		std::string pixelShaderString;
		vertexShaderString = ShaderAsString(vertexShader);
		pixelShaderString = ShaderAsString(pixelShader);
		// Read In File
		ParseFiles();

		// part 2a DONE
		// WORLD
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
		at.x = 0.15f;			//at.x = -0.8553;		 
		at.y = 0.75f;			//at.y = 0.7554;		 
		at.z = 0.0f;			//at.z = 0.9219;		 
		at.w = 1.0f;			//at.w = 1.0f;		 
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
		sceneData.gwViewMatrix = cameraLocations[0];
		sceneData.lightColor = lightColor;
		sceneData.lightDirection = lightDirection;
		sceneData.lightAmbient = lightAmbient;
		sceneData.lightPos = lightPos;
		//sceneData.camPos = eye;

		//meshDataText.material = modelInformation[0].modelMaterial[0];
		//meshDataText.gwWorldMatrix = modelInformation[0].modelLocations;
		//meshDataText.gwWorldMatrixTwo = gwWorldMatrix;

		//meshDataLogo.material = modelInformation[0].modelMaterial[1];
		//meshDataLogo.gwWorldMatrix = modelInformation[0].modelLocations;
		//meshDataLogo.gwWorldMatrixTwo = gwWorldMatrix;

		unsigned int vertexBufferSize = sizeof(H2B::VERTEX) * vertexInfo.size();
		unsigned int indexBufferSize = sizeof(unsigned) * indexInfo.size();

		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBuffer));
		// Transfer triangle data to the vertex buffer.

		vertexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, vertexInfo.data(), vertexBufferSize);
		vertexBuffer->Unmap(0, nullptr);
		// Create a vertex View to send to a Draw() call.
		vertexView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexView.StrideInBytes = sizeof(H2B::VECTOR) * 3; // Part 1e orig 2 DONE
		vertexView.SizeInBytes = vertexBufferSize; // Part 1d DONE
		// Part 1g
		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(indexBufferSize),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&indexBuffer));
		// Transfer triangle data to the vertex buffer.
		//UINT8* transferMemoryLocation;
		indexBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));
		memcpy(transferMemoryLocation, indexInfo.data(), indexBufferSize);
		indexBuffer->Unmap(0, nullptr);
		//// Create a vertex View to send to a Draw() call.
		indexView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		//indexView.StrideInBytes = sizeof(unsigned int) * 1; // Part 1e orig 2 DONE
		indexView.Format = DXGI_FORMAT_R32_UINT;
		indexView.SizeInBytes = indexBufferSize; // Part 1d DONE

		// TODO: Part 2d
		IDXGISwapChain4* temp;
		_d3d.GetSwapchain4(reinterpret_cast<void**>(&temp));
		temp->GetDesc(&swapChainDesc);
		

		//&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + FSLogo_meshcount * sizeof(MESH_DATA) * temp1.BufferCount);

		creator->CreateCommittedResource( // using UPLOAD heap for simplicity
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), // DEFAULT recommend  
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SCENE_DATA) + meshCount
				* sizeof(MESH_DATA) * swapChainDesc.BufferCount),
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&constantBuffer));

		// Transfer triangle data to the vertex buffer.
		constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
			reinterpret_cast<void**>(&transferMemoryLocation));

		unsigned int frame_meshdata = 0;
		std::vector<MESH_DATA> tempMesh;

		for (int i = 0; i < 1; i++)
		{
			// take in scene data
			memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
			frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
			// for as many models are in the level
			for (int j = 0; j < modelInformation.size(); j++)
			{
				// for how many meshes each model has
				for (int k = 0; k < modelInformation[j].modelMeshes.size(); k++)
				{
					// NOTE - TEST THIS IS PASSING IN CORRECT DATA
					int temp = modelInformation[j].meshDataStartLocation + k;
					std::cout << "Passing in: " << modelInformation[j].meshDataStartLocation << std::endl;
					std::cout << "Idea to pass in: " << temp << std::endl;

					//&meshDataModels[modelInformation[j].meshDataStartLocation]
					memcpy(&transferMemoryLocation[frame_meshdata], &meshDataModels[temp], sizeof(MESH_DATA)); // SCENE DATA
					//tempMesh.push_back(meshDataModels[modelInformation[j].meshDataStartLocation]);
					frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
					//memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
					//frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
				}
			}
		}

		constantBuffer->Unmap(0, nullptr);

		//// Create a vertex View to send to a Draw() call.
		constantView.BufferLocation = constantBuffer->GetGPUVirtualAddress();
		constantView.SizeInBytes = sizeof(MESH_DATA); // Part 1d DONE

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
		CD3DX12_ROOT_PARAMETER rootParameters[2];
		rootParameters[0].InitAsConstantBufferView(0, 0);
		rootParameters[1].InitAsConstantBufferView(1, 0);


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

		unsigned int frame_meshdata = 0;// = sizeof(SCENE_DATA);

		if (swappingLevel == false)
		{
			constantBuffer->Map(0, &CD3DX12_RANGE(0, 0),
				reinterpret_cast<void**>(&transferMemoryLocation));

			for (int i = 0; i < 1; i++)
			{
				// take in scene data
				memcpy(&transferMemoryLocation[frame_meshdata], &sceneData, sizeof(SCENE_DATA)); // SCENE DATA
				frame_meshdata = frame_meshdata + sizeof(SCENE_DATA);
				// for as many models are in the level
				for (int j = 0; j < modelInformation.size(); j++)
				{
					for (int k = 0; k < modelInformation[j].modelMeshes.size(); k++)
					{
						int temp = modelInformation[j].meshDataStartLocation + k;
						memcpy(&transferMemoryLocation[frame_meshdata], &meshDataModels[temp], sizeof(MESH_DATA)); // SCENE DATA
						frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
						//memcpy(&transferMemoryLocation[frame_meshdata], &meshDataLogo, sizeof(MESH_DATA)); // SCENE DATA
						//frame_meshdata = frame_meshdata + sizeof(MESH_DATA);
					}
				}
			}

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

			unsigned int drawData = 0;
			for (int i = 0; i < modelInformation.size(); i++)
			{
				for (int j = 0; j < modelInformation[i].modelMeshes.size(); j++)
				{
					drawData = drawData + sizeof(MESH_DATA);
					cmd->SetGraphicsRootConstantBufferView(1, constantBuffer->GetGPUVirtualAddress() + drawData);
					cmd->DrawIndexedInstanced(modelInformation[i].modelMeshes[j].drawInfo.indexCount, 1, modelInformation[i].modelMeshes[j].drawInfo.indexOffset, modelInformation[i].vertexStartLocation, 0);
				}
			}
		}

		// release temp handles
		cmd->Release();
	}

	~Renderer()
	{
		// ComPtr will auto release so nothing to do here 
	}
};
