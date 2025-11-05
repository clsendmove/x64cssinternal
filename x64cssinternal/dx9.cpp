#include "dx9.h"
#include "drawmanager.h"
#include "safetyhook/safetyhook.hpp"
#include <wrl/client.h>
#include <d3d9.h>
#include <shared_mutex>
//#include "newmenu.h"
#include <winuser.h>
#include "memory.h"

#pragma comment(lib, "d3d9.lib")

using namespace dx9_hooks;

std::once_flag init;

template <typename T> using com_ptr = Microsoft::WRL::ComPtr<T>;

SafetyHookInline dx{};

HRESULT __fastcall dx9_hooks::present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window,
	const RGNDATA* dirty) {
    std::call_once(init, [&] {
        D3DDEVICE_CREATION_PARAMETERS params;
        device->GetCreationParameters(&params);
        u::drawsystem.initialize(device, params.hFocusWindow);
        //f::menu.theme();
    });

    u::drawsystem.start_paint(device);
    {
        auto* list = ImGui::GetBackgroundDrawList();
        u::draw.paint_traverse(list);
        /*
        if (GetAsyncKeyState(VK_INSERT) & 1)
            f::menu.open = !f::menu.open;

        if (f::menu.open) {
            f::menu.start();
        }*/

    }
    u::drawsystem.finish_paint(device);

    return dx.fastcall<HRESULT>(device, src, dest, window, dirty);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam,
    LPARAM lParam);

LONG __stdcall dx9_hooks::wnd_proc(HWND window, UINT message, WPARAM wide_param, LPARAM long_param) {
    /*
    if (f::menu.open) {
        ImGui_ImplWin32_WndProcHandler(window, message, wide_param, long_param);

        if (ImGui::GetIO().WantTextInput) {
            return 1;
        }
        if (message >= WM_MOUSEFIRST && WM_MOUSELAST >= message)
            return 1;
    }*/
    
    return CallWindowProc(original, window, message, wide_param, long_param);
}

void dx9_hooks::startup() {
    com_ptr<IDirect3D9>   d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
    auto                  window = FindWindowA("Valve001", nullptr);
    game_windows = window;
    D3DPRESENT_PARAMETERS params{
        .BackBufferFormat = D3DFMT_UNKNOWN,
        .SwapEffect = D3DSWAPEFFECT_DISCARD,
        .Windowed = TRUE,
        .EnableAutoDepthStencil = TRUE,
        .AutoDepthStencilFormat = D3DFMT_D16,
        .PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT,
    };

    com_ptr<IDirect3DDevice9> d3d_device{};

    d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params,
        &d3d_device);
    dx = safetyhook::create_inline(memory::get_vfunc<void*>(d3d_device.Get(), 17), present);
    original = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(game_windows, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wnd_proc)));
}

void dx9_hooks::shutdown() {
    if (game_windows) {
        SetWindowLongPtrW(game_windows, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(original));
    }
    dx = {};
    
    //SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)original);
}
