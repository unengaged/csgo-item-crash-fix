#include <Windows.h>
#include <iostream>
#include "MinHook.h"
#include <print>

#pragma comment(lib, "libMinHook.x86.lib")

typedef bool(__thiscall* DispatchUserMessage_t)(void*, int, int, int, const void*);
DispatchUserMessage_t oDispatchUserMessage = nullptr;
void* VClient018 = nullptr;

const int CS_UM_SendPlayerItemFound = 63;

template <typename T>
static T* GetInterface(const char* name, const char* library) 
{
    const HINSTANCE handle = GetModuleHandleA(library);
    if (!handle) {

        std::println("[GetInterface] Failed to get handle for library: {}", library);
        return nullptr;
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    using Fn = T * (*)(const char*, int);
    const Fn CreateInterface = reinterpret_cast<Fn>(GetProcAddress(handle, "CreateInterface"));

    if (!CreateInterface) 
    {
        std::println("[GetInterface] Failed to get CreateInterface");
        return nullptr;
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    return CreateInterface(name, 0);
}

bool __fastcall hkDispatchUserMessage(void* thisptr, int unk, int msg_type, int nFlags, int size, const void* msg) 
{
    if (msg_type == CS_UM_SendPlayerItemFound)
    {
        std::println("[+] filtered crash :D ( CS_UM_SendPlayerItemFound )");
        return false;
    }

    return oDispatchUserMessage(thisptr, msg_type, nFlags, size, msg);
}

void main_thread() 
{
    AllocConsole();
    SetConsoleTitleA("item crash fix by unengaged");

    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);

    std::println("[+] init");

    if (MH_Initialize() != MH_OK) 
    {
        std::println("[-] Failed to initialize MinHook");
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    VClient018 = GetInterface<void*>("VClient018", "client.dll");
    if (!VClient018) 
    {
        std::println("[-] Failed to get VClient018 interface");
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    void** vTable = *reinterpret_cast<void***>(VClient018);
    LPVOID hookAddress = reinterpret_cast<LPVOID>(reinterpret_cast<uintptr_t>(vTable[37]) + 44);

    if (MH_CreateHook(hookAddress, &hkDispatchUserMessage, reinterpret_cast<LPVOID*>(&oDispatchUserMessage)) != MH_OK) 
    {
        std::println("[-] Failed to create hook");
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    if (MH_EnableHook(hookAddress) != MH_OK) 
    {
        std::println("[-] Failed to enable hook");
        system("pause");
        TerminateProcess(reinterpret_cast<HANDLE>(-1), 0);
    }

    std::println("[+] hooked DispatchUserMessage @ -> {}", hookAddress);
}

bool __stdcall DllMain(HANDLE hinstDLL, uint32_t fdwReason, void* lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), 0, 0, 0);
    }

    return true;
}