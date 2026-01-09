#include "pch.h"

#include "base-dllmain.h" // Don't forget to add `version.lib` to Linker -> Input -> Additional Dependencies. Linker will give you unresolved symbols without it.
#include "AllocateMemory.h"
#include "AoBSwap.h"
#include "Common.h"
#include "Logger.h"
#include "PatchThreading.h"
#include "Util.h"

BOOL APIENTRY DllMain(const HMODULE hModule, const DWORD ulReasonForCall, const LPVOID lpReserved) {
    SetupLog(GetLogPathAsCurrentDllDotLog());
    EmptyDllProxy proxy;
    return BaseDllMain(hModule, ulReasonForCall, lpReserved, proxy);
}

bool NeuterDahliaFloatDecrypt(const std::string& moduleName, const PTR_SIZE moduleAddress, AllocateMemory* allocator, LogBuffer* logBuffer) {
    auto scanBytes   = "41 56 56 57 53 48 83 ec 38 c5 f9 7f 74 24 20 4c 89 cf 48 89 ce";
    auto newMemBytes = StringToByteVector(
        "49 8B 00 " // mov rax,[r8]
        "48 89 01 " // mov [rcx],rax
        "49 8B 40 08 " // mov rax,[r8+08]
        "48 89 41 08 " // mov [rcx+08],rax
        "48 8B C1 " // mov rax,rcx
        "C3" // ret
    );
    return DoSimplePatch(moduleName, moduleAddress, "Neuter Dahlia Float Decrypt", scanBytes, newMemBytes, logBuffer);
}

bool NeuterDahliaFloatEncrypt(const std::string& moduleName, const PTR_SIZE moduleAddress, AllocateMemory* allocator, LogBuffer* logBuffer) {
    // 41 ?? 41 ?? 41 ?? 41 ?? 56 57 55 53 48 81 EC ?? ?? ?? ?? C5 79 7F ?? 24 ?? 00 00 00 C5 79 ?? B4 ?? C0 00 ?? 00 C5 79 ?? AC 24 ?? 00 00 00 C5 79
    auto scanBytes   = "41 57 41 56 41 55 41 54 56 57 55 53 48 81 ec e8 00 00 00 c5 79 7f bc 24 d0 00 00 00 c5 79 7f b4 24 c0 00 00 00 c5 79 7f ac 24 b0 00 00 00 c5 79 7f a4 24 a0 00 00 00 c5 79 7f 9c 24 90 00 00 00 c5 79 7f 94 24 80 00 00 00";
    auto newMemBytes = StringToByteVector(
        "49 8B 00 " // mov rax,[r8]
        "48 89 01 " // mov [rcx],rax
        "49 8B 40 08 " // mov rax,[r8+08]
        "48 89 41 08 " // mov [rcx+08],rax
        "48 8B C1 " // mov rax,rcx
        "C3" // ret
    );
    return DoSimplePatch(moduleName, moduleAddress, "Neuter Dahlia Float Encrypt", scanBytes, newMemBytes, logBuffer);
}

void DoInjection() {
    const auto moduleName   = "MonsterHunterWilds.exe";
    const auto moduleHandle = GetModuleHandle(moduleName);

    if (moduleHandle == nullptr) {
        LOG("Unable to find module, aborting.");
        return;
    }

    const auto moduleAddress = reinterpret_cast<const PTR_SIZE>(moduleHandle);
    LOG("Module base address: " << std::uppercase << std::hex << moduleAddress);

    const auto moduleInfo = GetModuleInfo(GetCurrentProcess(), moduleHandle);
    LOG("Module size: " << std::uppercase << std::hex << moduleInfo.SizeOfImage);

    AllocateMemory allocator;
    /*
    if (!allocator.AllocateGlobalAddresses(moduleName, moduleAddress)) {
        MessageBoxW(nullptr, L"Patching failed.", L"Patching Failed", MB_ICONERROR | MB_OK);
        return;
    }
    */

    const std::vector<PatchFunction> injectorFunctions{
        &NeuterDahliaFloatDecrypt,
        &NeuterDahliaFloatEncrypt,
    };

    const auto result = DoPatchFunctionsAsync(moduleName, moduleAddress, &allocator, injectorFunctions);

    LOG("");

    if (result) {
        //MessageBoxW(nullptr, L"Patching done!", L"Patching Done", MB_ICONINFORMATION | MB_OK);
    } else {
        MessageBoxW(nullptr, L"Patching failed.", L"Patching Failed", MB_ICONERROR | MB_OK);
    }

    LOG("Patching done!");
}