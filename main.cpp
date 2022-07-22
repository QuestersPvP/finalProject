// Simple basecode showing how to create a window and attatch a d3d12surface
#define GATEWARE_ENABLE_CORE // All libraries need this
#define GATEWARE_ENABLE_SYSTEM // Graphics libs require system level libraries
#define GATEWARE_ENABLE_GRAPHICS // Enables all Graphics Libraries
// Part 2a DONE
#define GATEWARE_ENABLE_MATH
#define GATEWARE_ENABLE_INPUT
#define GATEWARE_ENABLE_AUDIO 
// Ignore some GRAPHICS libraries we aren't going to use
#define GATEWARE_DISABLE_GDIRECTX11SURFACE // we have another template for this
#define GATEWARE_DISABLE_GOPENGLSURFACE // we have another template for this
#define GATEWARE_DISABLE_GVULKANSURFACE // we have another template for this
#define GATEWARE_DISABLE_GRASTERSURFACE // we have another template for this
// With what we want & what we don't defined we can include the API
#include "../gateware/Gateware/Gateware.h"
#include "renderer.h"
#include "RH.h"
#include <Windows.h>
// open some namespaces to compact the code a bit
using namespace GW;
using namespace CORE;
using namespace SYSTEM;
using namespace GRAPHICS;

// lets pop a window and use D3D12 to clear to a jade colored screen
int main()
{
	GWindow win;
	GWindow win2;
	GEventResponder msgs;
	GEventResponder msgs2;
	GDirectX12Surface d3d12;
	GDirectX12Surface d3d122;

	GW::AUDIO::GAudio audio;
	audio.Create();

	GW::AUDIO::GAudio horn;
	GW::AUDIO::GMusic playHorn;
	horn.Create();
	playHorn.Create("../models/horn.wav", horn, 0.1f);
	//audio.PlayMusic();
	
	GW::AUDIO::GMusic music;
	music.Create("../models/music.wav", audio, 0.1f);
	
	music.Play(true);
	//audio.PlayMusic();


	if (+win.Create(0, 0, 800, 600, GWindowStyle::WINDOWEDBORDERED))
	{
		// create window to render minimap
		+win2.Create(0, 0, 200, 150, GWindowStyle::WINDOWEDBORDERLESS);

		// Part 1a DONE
		win.SetWindowName("Benjamin Russell - Final Project - DirectX12");
		float clr[] = { 0, 10/255.0f, 107/255.0f, 1 }; // start with a jade color
		msgs.Create([&](const GW::GEvent& e) {
			GW::SYSTEM::GWindow::Events q;
			if (+e.Read(q) && q == GWindow::Events::RESIZE)
				clr[0] += 0.01f; // move towards a orange as they resize
		});

		//msgs2.Create([&](const GW::GEvent& f) {
		//	GW::SYSTEM::GWindow::Events q;
		//	if (+f.Read(q) && q == GWindow::Events::RESIZE)
		//		clr[0] += 0.01f; // move towards a orange as they resize
		//	});

		win.Register(msgs);
		win2.Register(msgs);

		if (+d3d12.Create(win, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT) && +d3d122.Create(win2, GW::GRAPHICS::DEPTH_BUFFER_SUPPORT))
		{
			// create d3d12 events for window

			Renderer* renderer = new Renderer(win, d3d12); // init
			Renderer* renderer2 = new Renderer(win2, d3d122); // init
			unsigned int tempX, tempY;

			while (+win.ProcessWindowEvents() && +win2.ProcessWindowEvents())
			{
				win.GetX(tempX);
				win.GetY(tempY);
				win2.ReconfigureWindow(tempX+5, tempY+30, 200, 150, GWindowStyle::WINDOWEDBORDERLESS);
				// process events for window2
				//+win2.ProcessWindowEvents();

				if (GetAsyncKeyState('H') & 0x0001)
				{
					playHorn.Play(false);
				}

				if (+d3d12.StartFrame() && +d3d122.StartFrame())
				{
					

					ID3D12GraphicsCommandList* cmd;
					D3D12_CPU_DESCRIPTOR_HANDLE rtv;
					D3D12_CPU_DESCRIPTOR_HANDLE dsv;

					ID3D12GraphicsCommandList* cmd2;
					D3D12_CPU_DESCRIPTOR_HANDLE rtv2;
					D3D12_CPU_DESCRIPTOR_HANDLE dsv2;

					if (+d3d12.GetCommandList((void**)&cmd) && 
						+d3d12.GetCurrentRenderTargetView((void**)&rtv) &&
						+d3d12.GetDepthStencilView((void**)&dsv))
					{
						cmd->ClearRenderTargetView(rtv, clr, 0, nullptr);
						cmd->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);
						
						if (makeNewRenderer == true)
						{
							renderer = new Renderer(win, d3d12);
							makeNewRenderer = false;
						}
						if (swappingLevel == false)
						{
							UpdateCamera();// move camera 
							renderer->Render(true); // draw 
						}
						d3d12.EndFrame(false);
					}

					if (+d3d122.GetCommandList((void**)&cmd2) &&
						+d3d122.GetCurrentRenderTargetView((void**)&rtv2) &&
						+d3d122.GetDepthStencilView((void**)&dsv2))
					{
						cmd2->ClearRenderTargetView(rtv2, clr, 0, nullptr);
						cmd2->ClearDepthStencilView(dsv2, D3D12_CLEAR_FLAG_DEPTH, 1, 0, 0, nullptr);

						if (makeNewRenderer == true)
						{
							renderer2 = new Renderer(win2, d3d122);
						}
						if (swappingLevel == false)
						{
							renderer2->Render(false); // draw
						}
						d3d122.EndFrame(false);
					}
				}
			}// clean-up when renderer falls off stack
		}
	}
	return 0; // that's all folks
}