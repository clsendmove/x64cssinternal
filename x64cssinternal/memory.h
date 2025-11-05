#pragma once
#include <Windows.h>
#include <vector>
//#include <winDNS.h>
typedef unsigned __int64 QWORD;

// x64 version of my memory.h
// very pasted

typedef void* (*CreateInterfaceFn)(const char* pName, int* pReturnCode);

namespace memory { 

    inline void* ignore_this(const HMODULE hModule, const char* const szObject) {
        const FARPROC pfCreateAddr = GetProcAddress(hModule, "CreateInterface");

        if (!pfCreateAddr)
            return nullptr;

        const CreateInterfaceFn pfCreateInterface = reinterpret_cast<CreateInterfaceFn>(pfCreateAddr);

        if (!pfCreateInterface)
            return nullptr;

        return pfCreateInterface(szObject, nullptr);
    }

    template<typename T>
    inline T get_interface(const char* const szModule, const char* const szObject)
    {
        const HMODULE hMod = GetModuleHandleA(szModule);

        if (!hMod)
            return NULL;

        void* pRet = ignore_this(hMod, szObject);

        if (pRet)
        {
            return static_cast<T>(pRet);
        }
        else
        {
            MessageBoxA(HWND_DESKTOP, szObject, "Failed to find interface:", MB_ICONERROR);
            return NULL;
        }
    }

    inline QWORD rel2abs(QWORD Address, QWORD RVAOffset)
    {
        // learn more: https://www.unknowncheats.me/forum/4432156-post17.html        
        // 64 bit relative addresses: (address of ptr in instruction) + instruction + (size of instruction)
        return *reinterpret_cast<int32_t*>(Address + RVAOffset) + Address + (RVAOffset + sizeof(int32_t));
    }

    template <typename T = std::uintptr_t, std::uint16_t offset = 0u>
    inline T find_signature(const char* module_name, const char* signature) {
        const HMODULE module_handle = GetModuleHandleA(module_name);
        if (!module_handle)
            return T{ };

        auto signature_to_byte = [](const char* signature) {
            std::vector<int> bytes;
            const char* end = signature + std::strlen(signature);

            for (const char* current = signature; current < end; ++current) {
                if (*current == '?') {
                    ++current;
                    if (*current == '?')
                        ++current;
                    bytes.push_back(-1);
                }
                else {
                    bytes.push_back(static_cast<int>(std::strtoul(current, const_cast<char**>(&current), 16)));
                }
            }
            return bytes;
            };

        auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
        auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS64>(reinterpret_cast<std::uint8_t*>(module_handle) + dos_header->e_lfanew);

        const std::uintptr_t size_of_image = nt_headers->OptionalHeader.SizeOfImage;
        const auto pattern_bytes = signature_to_byte(signature);
        const std::uint8_t* scan_bytes = reinterpret_cast<std::uint8_t*>(module_handle);

        const size_t s = pattern_bytes.size();
        const int* d = pattern_bytes.data();

        for (std::uintptr_t i = 0; i < size_of_image - s; ++i) {
            bool found = true;

            for (size_t j = 0; j < s; ++j) {
                if (scan_bytes[i + j] != d[j] && d[j] != -1) {
                    found = false;
                    break;
                }
            }

            if (found) {
                auto address = reinterpret_cast<std::uintptr_t>(&scan_bytes[i]) + offset;
                return static_cast<T>(address);
            }
        }

        return T{ };
    }
    /* vfunc */
    inline void**& get_vtable(void* inst, const unsigned int offset)
    {
        return *reinterpret_cast<void***>(reinterpret_cast<uintptr_t>(inst) + offset);
    }

    inline const void** get_vtable(const void* inst, const unsigned int offset)
    {
        return *reinterpret_cast<const void***>(reinterpret_cast<uintptr_t>(inst) + offset);
    }

    template<typename T>
    inline T get_vfunc(void* inst, const unsigned int index, const unsigned int offset = 0u) {
        return reinterpret_cast<T>(get_vtable(inst, offset)[index]);
    }
}
