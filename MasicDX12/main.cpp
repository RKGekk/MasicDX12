#include <Windows.h>
#include <utility>
#include <tuple>
#include <string>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"DirectXTK12.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"D3DCompiler.lib")
#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <dxgidebug.h>
#pragma comment(lib,"assimp-vc142-mtd.lib")
#else
#pragma comment(lib,"assimp-vc142-mt.lib")
#endif
#pragma comment(lib, "winmm.lib")

#include "engine/engine_options.h"
#include "engine/render_window_config.h"
#include "engine/render_window.h"
#include "engine/engine.h"
#include "events/event_manager.h"


using namespace std::literals;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	Engine* pEngine = Engine::GetEngine();
	bool can_run = pEngine->Initialize(
		RenderWindowConfig{ EngineOptions("EngineOptions.xml"s) }
			.set_hInstance(hInstance)
			.set_window_title("CG2_2")
			.set_window_class("MyTestWindowsClass")
	);
	if (can_run) {
		pEngine->Run();
	}

	return 0;
}