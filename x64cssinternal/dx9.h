#pragma once
#include <d3d9.h>
#include <intrin.h>

// clutters hooks, i moved it here

namespace dx9_hooks {
	HRESULT __fastcall present(IDirect3DDevice9* device, const RECT* src, const RECT* dest, HWND window,
		const RGNDATA* dirty);
	inline HWND    game_windows;
	inline WNDPROC original;
	LONG __stdcall wnd_proc(HWND window, UINT message, WPARAM wide_param, LPARAM long_param);

	void        startup();
	void        shutdown();
}
