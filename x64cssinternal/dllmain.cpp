#include <Windows.h>
#include <thread>
#include <time.h>
#include "interfaces.h"
#include "hooks.h"

DWORD WINAPI main_thread(LPVOID lpArguments)
{
	interfaces::init();
	hooks::init();
	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(350));
	hooks::uninit();
	// uninject
	FreeLibraryAndExitThread(reinterpret_cast<HMODULE>(lpArguments), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		if (const HANDLE hMain = CreateThread(0, 0, main_thread, hInstance, 0, 0))
			CloseHandle(hMain);

	return true;
}