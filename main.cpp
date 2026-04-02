#include "utils.h"

namespace Rift {
    namespace System {
        
        DWORD ProcessUtility::GetProcessIdByName(const std::string& processName) {
            DWORD processId = 0;
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
            
            if (snapshot != INVALID_HANDLE_VALUE) {
                PROCESSENTRY32 processEntry;
                processEntry.dwSize = sizeof(processEntry);
                
                if (Process32First(snapshot, &processEntry)) {
                    do {
                        if (!strcmp(processEntry.szExeFile, processName.c_str())) {
                            processId = processEntry.th32ProcessID;
                            break;
                        }
                    } while (Process32Next(snapshot, &processEntry));
                }
            }
            CloseHandle(snapshot);
            return processId;
        }

        uintptr_t ProcessUtility::GetModuleBaseAddress(DWORD procId, const wchar_t* modName) {
            uintptr_t modBaseAddr = 0;
            HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
            
            if (hSnap != INVALID_HANDLE_VALUE) {
                MODULEENTRY32W modEntry;
                modEntry.dwSize = sizeof(modEntry);
                if (Module32FirstW(hSnap, &modEntry)) {
                    do {
                        if (!_wcsicmp(modEntry.szModule, modName)) {
                            modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                            break;
                        }
                    } while (Module32NextW(hSnap, &modEntry));
                }
            }
            CloseHandle(hSnap);
            return modBaseAddr;
        }

    } // namespace System
} // namespace Rift
