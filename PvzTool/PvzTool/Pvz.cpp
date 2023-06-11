#include "stdafx.h"
#include "Pvz.h"

CPvz::CPvz()
{
}


CPvz::~CPvz()
{
}

// 获取游戏的 PID
DWORD CPvz::GetGamePid()
{
    HWND hWnd = ::FindWindow(NULL, GAME_NAME);

    if (hWnd == NULL)
    {
        return -1;
    }

    DWORD dwPid = 0;
    ::GetWindowThreadProcessId(hWnd, &dwPid);

    return dwPid;
}


// 修改阳光的值
VOID CPvz::ModifySunValue(DWORD dwSun)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 0044BA45 - add[edi + 00005578], eax EDI = 1E4A0B40
    // 00475373 - mov edi, [esi + 00000868] ESI = 0286B490
    // 7794F8

    DWORD dwNum = 0;
    ReadProcessMemory(hProcess, (LPCVOID)0x6a9ec0, &dwNum, sizeof(DWORD), NULL);
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x768), &dwNum, sizeof(DWORD), NULL);

    WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x5560), &dwSun, sizeof(DWORD), NULL);

    CloseHandle(hProcess);
}


// 种植不减阳光
VOID CPvz::SunNop()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0041BA74 2B F3 sub esi,ebx
    // 修改后的指令：
    //     0041BA74 90 nop 
    //     0041BA75 90 nop
    char *nop = "\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0041BA74, nop, 2, NULL);

    CloseHandle(hProcess);
}


// 种植免冷却
VOID CPvz::NoCd()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：00488250 75 24 jne 00488276
    // 修改后的指令：00488250 EB 24 jmp 00488276
    char *patch1 = "\xeb\x24";
    WriteProcessMemory(hProcess, (LPVOID)0x00488250, patch1, 2, NULL);

    // 原指令：00488688 0F85 79010000 jne 00488807
    // 修改后的指令：
    //     00488688 E9 7A010000 jmp 00488807
    //     0048868D 90          nop
    char *patch2 = "\xe9\x7a\x01\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x00488688, patch2, 6, NULL);

    CloseHandle(hProcess);
}


// 修改金币
VOID CPvz::ModifyCoinValue(DWORD dwCoin)
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    DWORD dwNum = 0;
    ReadProcessMemory(hProcess, (LPCVOID)0x6a9ec0, &dwNum, sizeof(DWORD), NULL);
    ReadProcessMemory(hProcess, (LPCVOID)(dwNum + 0x82c), &dwNum, sizeof(DWORD), NULL);

    WriteProcessMemory(hProcess, (LPVOID)(dwNum + 0x28), &dwCoin, sizeof(DWORD), NULL);

    CloseHandle(hProcess);
}


// 重复建造，无需花盆、无需荷叶
VOID CPvz::Build()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0040FE2F 0F84 1F090000 je 00410754
    // 修改后的指令：
    //     0040FE2F E9 20090000 jmp 00410754
    //     0040FE34 90          nop
    char *patch = "\xe9\x20\x09\x00\x00\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0040FE2F, patch, 6, NULL);

    CloseHandle(hProcess);
}


// 自动收集阳光
VOID CPvz::Auto()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0043158F 75 08 jne 00431599
    // 修改后的指令：0043158F EB 08 jmp 00431599
    char *patch = "\xeb\x08";
    WriteProcessMemory(hProcess, (LPVOID)0x0043158F, patch, 2, NULL);

    CloseHandle(hProcess);
}


// 加速攻击
VOID CPvz::Speed()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：00464A96 0F85 98FEFFFF jne 00464934
    // 修改后的指令：
    //     00464A96 90 nop 
    //     00464A97 90 nop
    //     00464A98 90 nop
    //     00464A99 90 nop
    //     00464A9A 90 nop
    //     00464A9B 90 nop
    char *patch = "\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x00464a96, patch, 6, NULL);

    CloseHandle(hProcess);
}


// 加速阳光金币
VOID CPvz::More()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0045FA48 83 47 58 FF add dword ptr [edi+58],-01 { 255 }
    // 修改后的指令：0045FA48 83 47 58 9C add dword ptr [edi+58],-64 { 156 }
    char *patch = "\x83\x47\x58\x9c";
    WriteProcessMemory(hProcess, (LPVOID)0x0045FA48, patch, 4, NULL);

    CloseHandle(hProcess);
}


// 全屏僵尸
VOID CPvz::AllScreen()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：00413FD9 0F85 0D010000 jne 004140EC
    // 修改后的指令：
    //     00413FD9 90 nop 
    //     00413FDA 90 nop
    //     00413FDB 90 nop
    //     00413FDC 90 nop
    //     00413FDD 90 nop
    //     00413FDE 90 nop
    char *patch = "\x90\x90\x90\x90\x90\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x00413FD9, patch, 6, NULL);

    CloseHandle(hProcess);
}


// 取消全屏僵尸
VOID CPvz::UnAllScreen()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 指令与 AllScreen 相反
    char *patch = "\x0f\x85\x0d\x01\x00\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00413FD9, patch, 6, NULL);

    CloseHandle(hProcess);
}


// 僵尸冰冻状态
VOID CPvz::Cool()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：0052AB3E E9 BD63EDFF jmp 00400F00
    char *patch1 = "\xe9\xbd\x63\xed\xff\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0052AB3E, patch1, 6, NULL);

    // 申请新的空间
    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)0x00400f00, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    // 在新申请的空间写入的指令
    //     00400F00 C7 86 AC000000 00100000 mov [esi+000000AC],00001000 { 4096 }
    //     00400F0A 0F85 D89C1200 jne 0052ABE8
    //     00400F10 E9 2F9C1200 jmp 0052AB44
    char *patch2 = "\xc7\x86\xac\x00\x00\x00\x00\x10\x00\x00\x0F\x85\xD8\x9C\x12\x00\xE9\x2F\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00400f00, patch2, 21, NULL);

    CloseHandle(hProcess);
}


// 僵尸奶油状态
VOID CPvz::Stop()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\xe9\xbd\x63\xed\xff\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0052AB3E, patch1, 6, NULL);

    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)0x00400f00, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);


    // 在新申请的空间写入的指令
    // 00400F00 60 pushad 
    // 00400F01 9C pushfd
    // 00400F02 D9 46 2C fld dword ptr[esi + 2C]
    // 00400F05 C7 46 2C 56FB4843 mov[esi + 2C], 4348FB56{ 200.98 }
    // 00400F0C D8 56 2C fcom dword ptr[esi + 2C]
    // 00400F0F 9BDFE0 wait:fstsw ax
    // 00400F12 9E sahf
    // 00400F13 77 0A ja 00400F1F
    // 00400F15 C7 86 B0000000 00010000 mov[esi + 000000B0], 00000100 { 256 }
    // 00400F1F D9 5E 2C fstp dword ptr[esi + 2C]
    // 00400F22 9D popfd
    // 00400F23 61 popad
    // 00400F24 0F85 BE9C1200 jne 0052ABE8
    // 00400F2A E9 159C1200 jmp 0052AB44
    char *patch2 = "\x60"
        "\x9c"
        "\xd9\x46\x2c"
        "\xc7\x46\x2c\x56\xFB\x48\x43"
        "\xD8\x56\x2C"
        "\x9B\xDF\xE0"
        "\x9E"
        "\x77\x0A"
        "\xC7\x86\xB0\x00\x00\x00\x00\x01\x00\x00"
        "\xD9\x5E\x2C"
        "\x9D"
        "\x61"
        "\x0F\x85\xBE\x9C\x12\x00"
        "\xE9\x15\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00400f00, patch2, 47, NULL);

    CloseHandle(hProcess);
}


// 僵尸死亡
VOID CPvz::Dead()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);


    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\xe9\xbd\x63\xed\xff\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0052AB3E, patch1, 6, NULL);

    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)0x00400f00, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);

    // 在新申请的空间写入的指令
    //     00400F00 C7 46 28 01000000 mov [esi+28],00000001 { 1 }
    //     00400F07 0F85 DB9C1200     jne 0052ABE8
    //     00400F0D E9 329C1200       jmp 0052AB44
    char *patch2 = "\xc7\x46\x28\x01\x00\x00\x00\x0F\x85\xdb\x9C\x12\x00\xE9\x32\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00400f00, patch2, 18, NULL);

    CloseHandle(hProcess);
}


// 吸怪
VOID CPvz::Attract()
{
    DWORD dwPid = GetGamePid();
    if (dwPid == -1)
    {
        MessageBox(NULL, L"游戏未找到", L"提示", MB_OK);
        return;
    }

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);

    // 原指令：0052AB3E 0F85 A4000000 jne 0052ABE8
    // 修改后的指令：
    //    0052AB3E E9 BD63EDFF jmp 00400F00
    //    0052AB43 90          nop
    char *patch1 = "\xe9\xbd\x63\xed\xff\x90";
    WriteProcessMemory(hProcess, (LPVOID)0x0052AB3E, patch1, 6, NULL);

    DWORD dwOldProtect = 0;
    VirtualProtectEx(hProcess, (LPVOID)0x00400f00, 64, PAGE_EXECUTE_READWRITE, &dwOldProtect);
    
    // 在新申请的空间写入的指令
    //     00400F00 C7 46 1C 02000000 mov [esi+1C],00000002 { 2 }
    //     00400F07 0F85 DB9C1200     jne 0052ABE8
    //     00400F0D E9 329C1200       jmp 0052AB44
    char *patch2 = "\xc7\x46\x1c\x02\x00\x00\x00\x0F\x85\xdb\x9C\x12\x00\xE9\x32\x9C\x12\x00";
    WriteProcessMemory(hProcess, (LPVOID)0x00400f00, patch2, 18, NULL);

    CloseHandle(hProcess);
}
