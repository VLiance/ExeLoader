
#include "win.h"

#ifndef ImWin
#define InCpcDosCore
#include "CPC_WPR.h"

#endif // ImWin

//#define WinLastError

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <errno.h>
#include <math.h>
//#include <internal.h>
#include <cassert>
#include <stdarg.h>



#include "WinRemplaceFuncTable.h"







//Declaration                        ----------------------->    decorated name
//void __cdecl foo(void);            ----------------------->    _foo
//void __cdecl foo(int a);           ----------------------->    _foo
//void __cdecl foo(int a, int b);    ----------------------->    _foo
//void __stdcall foo(void);          ----------------------->    _foo@0
//void __stdcall foo(int a);         ----------------------->    _foo@4
//void __stdcall foo(int a, int b);  ----------------------->    _foo@8
//void __fastcall foo(void);         ----------------------->    @foo@0
//void __fastcall foo(int a);        ----------------------->    @foo@4
//void __fastcall foo(int a, int b); ----------------------->    @foo@8

/*
#define EXPLICIT_SYMBOL2(SYMFROM, SYMTO)                                       \
  if (!strcmp(symbolName, #SYMFROM))                                           \
    return (void *)&SYMTO;

 	EXPLICIT_SYMBOL2(_Znwj, _Znwm) //New
    EXPLICIT_SYMBOL2(_Znaj, _Znam) //New[]
    */
  //  (void *)&SYMTO


 // extern "C" void STDCALL Sleep(int a);


#ifndef UseWinFunc
 //   void STDCALL _Sleep(int a);
  // void __attribute__((stdcall)) _Sleep(int a);
 //__attribute((dllimport)) void __attribute__((stdcall)) Sleep (unsigned long);
 //  extern void* _Znam;
  // extern void* Sleep;

#endif

extern  FARPROC MyMemoryDefaultGetProcAddress(HCUSTOMMODULE module, LPCSTR name, void *userdata);





/*
void EnterCriticalSection(){
    printf("EnterCriticalSection not implemented!\n");
}
*/

LPTSTR  My_GetCommandLineA(){
    printf("My_GetCommandLineA not implemented!\n");
    return (LPTSTR)"TestCmdLine";
}
DWORD   My_GetCurrentThreadId(){
    printf("My_GetCurrentThreadId not implemented!\n");
    return 55;
}

HANDLE   My_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL      bInitialOwner, LPCTSTR               lpName){
    printf("CreateMutexA not implemented!\n");
    return 0;
}
DWORD    My_WaitForSingleObject( HANDLE hHandle, DWORD  dwMilliseconds){
    printf("WaitForSingleObject not implemented!\n");
    return 0;
}
ATOM    My_FindAtomA( LPCTSTR lpString){
    printf("FindAtomA not implemented!\n");
    return 0;
}
UINT     My_GetAtomNameA(  ATOM   nAtom,  LPTSTR lpBuffer,  int    nSize){
    printf("GetAtomNameA not implemented!\n");
    return 0;
}
ATOM      My_AddAtomA(  LPCTSTR lpString){
    printf("AddAtomA not implemented!\n");
    return 0;
}

LONG  My_InterlockedExchange( LONG volatile *Target,  LONG Value){
    return 0;
}
BOOL   My_ReleaseMutex( HANDLE hMutex){
    return 0;
}

void WINAPI My_SetLastError(_In_ DWORD dwErrCode){
    printf("My_SetLastError not implemented!\n");
}


/*
void My_GetStartupInfoA(LPSTARTUPINFO lpStartupInfo){
    printf("GetStartupInfoA not implemented!\n");
}*/


void fNotImplemented(){
    printf("Function not implemented!\n");
}

LONG  My_InterlockedDecrement( LONG volatile *Addend){

    return (LONG)Addend;
}

bool STDCALL My_WriteConsoleA( HANDLE  hConsoleOutput, const VOID* lpBuffer, DWORD   nNumberOfCharsToWrite,LPDWORD lpNumberOfCharsWritten, LPVOID  lpReserved ){
    for(int i = 0; i < nNumberOfCharsToWrite; i++){
        printf("%c", ((char*)lpBuffer)[i]);
    }
    return true;
}

char * My_getenv(const char *varname ){
    return 0;
    printf("Mygetenv: %s\n", varname);
   // return (char*)varname;
   errno = EINVAL;
    return (char*)"10";
}
int My_fputc(int character, FILE * stream){
    printf("%c", character);
}

void My_localeconv(){
    printf("localeconv Function not implemented!\n");
}

void My_GetStartupInfo( LPSTARTUPINFO lpStartupInfo){
    printf("MyGetStartupInfo Function not implemented!\n");
lpStartupInfo->cb = sizeof(LPSTARTUPINFO);
lpStartupInfo->lpDesktop = (LPTSTR)L"";
lpStartupInfo->lpTitle = (LPTSTR)"";
lpStartupInfo->dwX =0;
lpStartupInfo->dwY =0;
lpStartupInfo->dwXSize =0;
lpStartupInfo->dwYSize =0;
lpStartupInfo->dwXCountChars =0;
lpStartupInfo->dwYCountChars =0;
lpStartupInfo->dwFillAttribute =0;
lpStartupInfo->dwFlags =0;
lpStartupInfo->wShowWindow =0;
lpStartupInfo->cbReserved2 =0;
lpStartupInfo->lpReserved2 =0;

lpStartupInfo->hStdInput =0;
lpStartupInfo->hStdOutput =0;
lpStartupInfo->hStdError =0;


}

HMODULE  My_LoadLibraryEx(  LPCTSTR lpFileName, HANDLE  hFile, DWORD   dwFlags){
  //  printf("LoadLibraryEx not implemented! : %s\n", lpFileName);
    #ifdef ImWin
        wprintf(L"LoadLibraryEx not implemented! : %s\n", lpFileName);
    #else // CpcDos
        printf("LoadLibraryEx not implemented! : %s\n", lpFileName);
    #endif
    return 0;
}

//LoadLibraryExW
FARPROC WINAPI  My_GetProcAddress(  HMODULE hModule, LPCSTR  lpProcName){
    printf("My_GetProcAddress -- : : %s \n", lpProcName);
    return MyMemoryDefaultGetProcAddress(0, lpProcName, 0);
}

void WINAPI  My_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
    printf("EnterCriticalSection not implemented!\n");
}

void WINAPI  My_LeaveCriticalSection(){
    printf("LeaveCriticalSection not implemented!\n");
}
void WINAPI  My_DeleteCriticalSection(){
    printf("DeleteCriticalSection not implemented!\n");
}
void My_ExitProcess( UINT uExitCode){
    printf("ExitProcess not implemented! %d \n", uExitCode);
    throw;
}
void WINAPI  My_GetModuleHandleA( LPCTSTR lpModuleName ){
    printf("GetModuleHandleA not implemented! Modulename: %s\n" , lpModuleName);
}
void WINAPI  My_InitializeCriticalSection(){
    printf("InitializeCriticalSection not implemented!\n");
}
BOOL  WINAPI  My_InitializeCriticalSectionEx( LPCRITICAL_SECTION lpCriticalSection, DWORD              dwSpinCount, DWORD              Flags){
    printf("My_InitializeCriticalSectionEx not implemented!\n");
    return false;
}

LPTOP_LEVEL_EXCEPTION_FILTER  My_SetUnhandledExceptionFilter( LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter){
    printf("SetUnhandledExceptionFilter not implemented!\n");
    return 0;
}


DWORD WINAPI My_TlsAlloc(void){
    printf("TlsAlloc not implemented!\n");
}
LPVOID WINAPI My_TlsGetValue(  DWORD dwTlsIndex){
    printf("TlsGetValue not implemented!\n");
    return (LPVOID)0;
}
BOOL  WINAPI My_TlsSetValue(  DWORD dwTlsIndex, _In_opt_ LPVOID lpTlsValue){
    printf("TlsSetValue not implemented!\n");
    return false;
}

void My_VirtualProtect(){
    printf("VirtualProtect not implemented!\n");
}
void My_VirtualQuery(){
    printf("VirtualQuery not implemented!\n");
}
int My_getmainargs( int * _Argc, char *** _Argv,  char *** _Env, int _DoWildCard,void * _StartInfo){
    printf("__getmainargs: %d\n", *_Argc);
}

void   My_set_app_type(int at){
    printf("__set_app_type not implemented! --> App type: %d\n", at); //_UNKNOWN_APP , _CONSOLE_APP, _GUI_APP
}



void My__signal(){
    printf("signal not implemented!\n");
}

void exited(){
    printf("\n-----AppExit------\n");
}

int My_atexit(void (*func)(void)){
    func = exited;
    printf("atexit not implemented!\n");
    return 0;
}
int My_setmode( int fd, int mode ){
    printf("_setmode not implemented! : %d : %d \n", fd, mode);
    return  0x0004; //O_BINARY
}
void My_cexit(){
    printf("_cexit not implemented!\n");
    throw;
}

void My_Onexit(){
    printf("_onexit not implemented!\n");
}
void iob(){
    printf("_iob not implemented!\n");
}

BOOL WINAPI My_IsProcessorFeaturePresent(
  _In_ DWORD ProcessorFeature){
    printf("My_IsProcessorFeaturePresent not implemented! ---- : %d \n", ProcessorFeature);
    return false;
}

DWORD  WINAPI My_GetCurrentProcessId(void){
    printf("My_GetCurrentProcessId not implemented!\n");
    return 0;
}
//aas






void Nothing(){
  //  #ifndef OnWin
    printf("\nNothing");
   // #endif
}

void STDCALL StdNothing(){
  //  #ifndef OnWin
    printf("\nNothing");
   // #endif
}


char* sEnviron[] = {(char*)"aaaabbbvvv", (char*)"aaaaa"};
char*** cEnviron;
 char ***  My_penviron(){
     cEnviron = (char***)&sEnviron;
    printf("__p__environ not implemented!\n");
    return cEnviron;
}
int fmode =  0x0004; //O_BINARY; //???????
int* My_pfmode(){
    printf("__p__fmode not implemented!\n");
    return &fmode; //Pointer to the _fmode global variable.
}

/*
I:\Cpp\ExeLoader\FuncTable.h|406|error: cannot convert 'LPCTSTR {aka const char*}' to 'LPCWSTR {aka const wchar_t*}' for argument '4' to 'void* CreateSemaphoreW(LPSECURITY_ATTRIBUTES, LONG, LONG, LPCWSTR)'|
HANDLE ghSemaphore;
*/




HANDLE  STDCALL  My_CreateSemaphore( //Must have  __stdcall
 _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  _In_     LONG                  lInitialCount,
  _In_     LONG                  lMaximumCount,
  _In_opt_ LPCTSTR               lpName
){
       printf("My_CreateSemaphore not implemented!\n");
return 0;
}



/*
typedef void(__cdecl * _INITTERMFUN)(void);
// Constructor prototypes
typedef void (__cdecl *_PVFV)(void);
typedef int  (__cdecl *_PIFV)(void);
// Call C++ constructors
static  void   My_initterm(_INITTERMFUN*  	start, _INITTERMFUN * 	end )
{

    // walk the table of function pointers from the bottom up, until
    // the end is encountered.  Do not skip the first entry.  The initial
    // value of pfbegin points to the first valid entry.  Do not try to
    // execute what pfend points to.  Only entries before pfend are valid.
     _INITTERMFUN* current = start;
     while (current<end) {
        if (*current){
      printf("Call init function %p\n",*current);
      (**current)();
         printf("returned\n");
      }
     current++;
     }
}
*/


typedef void* (*FUNC_)();
typedef struct {
    const char* sFuncName;
    FUNC_ dFunc;
} sFunc;


//extern "C"  __attribute__(( __declspec(dllexport))) void STDCALL Sleep(int);
//extern "C"    void  STDCALL Sleep(int);

//> _SetCallback@4

/*
void STDCALL _SleepAss(int a){

}*/

//__declspec (dllexport) extern  void STDCALL _SleepAss(int a);
/*
#ifdef OnWin
    #define a4(_Func) _Func##Z4
#else
    #define a4(_Func) _Func##Z4
#endif

extern "C" void a4(Sleep)(int);
*/
sFunc aTableFunc[] = {

 // {"Sleep"  ,(FUNC_) a4(Sleep) },

{"fNotImplemented" ,(FUNC_) fNotImplemented }, //Must be first

{"GetProcAddress" ,(FUNC_) My_GetProcAddress },

//{"_initterm" ,(FUNC_) My_initterm }, //Must be first



#ifdef UseWinFunc


{"LoadLibraryExW"  ,(FUNC_) LoadLibraryExW },
{"InitializeCriticalSectionEx" ,(FUNC_) My_InitializeCriticalSectionEx },
{"IsProcessorFeaturePresent" ,(FUNC_) My_IsProcessorFeaturePresent },

    {"LoadLibraryExW"  ,(FUNC_) LoadLibraryExW },
  //  {"GetProcAddress"  ,(FUNC_) GetProcAddress },
    {"IsProcessorFeaturePresent"  ,(FUNC_) IsProcessorFeaturePresent },
    {"InitializeCriticalSectionAndSpinCount"  ,(FUNC_) InitializeCriticalSectionAndSpinCount },
    {"HeapAlloc"  ,(FUNC_) HeapAlloc },
    {"GetProcessHeap"  ,(FUNC_) GetProcessHeap },
    {"IsDebuggerPresent"  ,(FUNC_) IsDebuggerPresent },
    {"IsDebuggerPresent"  ,(FUNC_) IsDebuggerPresent },
    {"UnhandledExceptionFilter"  ,(FUNC_) UnhandledExceptionFilter },
    {"GetCurrentProcess"  ,(FUNC_) GetCurrentProcess },
    {"TerminateProcess"  ,(FUNC_) TerminateProcess },
    {"GetStartupInfoW"  ,(FUNC_) GetStartupInfoW },
    {"GetFileType"  ,(FUNC_) GetFileType },
    {"GetStdHandle"  ,(FUNC_) GetStdHandle },

    {"GetACP"  ,(FUNC_) GetACP },
    {"MultiByteToWideChar"  ,(FUNC_) MultiByteToWideChar },
    {"WideCharToMultiByte"  ,(FUNC_) WideCharToMultiByte },
    {"GetStringTypeW"  ,(FUNC_) GetStringTypeW },
    {"GetCommandLineA"  ,(FUNC_) GetCommandLineA },
    {"GetCommandLineW"  ,(FUNC_) GetCommandLineW },
    {"GetCPInfo"  ,(FUNC_) GetCPInfo },
    {"IsValidCodePage"  ,(FUNC_) IsValidCodePage },
    {"InitializeSListHead"  ,(FUNC_) InitializeSListHead },
    {"GetEnvironmentStringsW"  ,(FUNC_) GetEnvironmentStringsW },
    {"FreeEnvironmentStringsW"  ,(FUNC_) FreeEnvironmentStringsW },
    {"HeapFree"  ,(FUNC_) HeapFree },
    {"GetModuleFileNameA"  ,(FUNC_) GetModuleFileNameA },
    {"WriteFile"  ,(FUNC_) WriteFile },

      {"GetCurrentProcessId"  ,(FUNC_)GetCurrentProcessId },
      {"GetCurrentThreadId"  ,(FUNC_)GetCurrentThreadId },
      {"GetTickCount"  ,(FUNC_) GetTickCount },
      {"QueryPerformanceCounter"  ,(FUNC_) QueryPerformanceCounter },
      {"GetSystemTimeAsFileTime"  ,(FUNC_) GetSystemTimeAsFileTime },
      {"SetUnhandledExceptionFilter"  ,(FUNC_) SetUnhandledExceptionFilter },


      {"EnterCriticalSection" ,(FUNC_) EnterCriticalSection },//BUG
    {"LeaveCriticalSection" ,(FUNC_) LeaveCriticalSection }, //BUG
    {"DeleteCriticalSection" ,(FUNC_) DeleteCriticalSection },
    {"ExitProcess" ,(FUNC_) ExitProcess },
    {"LoadLibraryExW" ,(FUNC_) LoadLibraryExW },
    {"GetSystemTimeAsFileTime" ,(FUNC_) GetSystemTimeAsFileTime },
    {"IsProcessorFeaturePresent" ,(FUNC_) IsProcessorFeaturePresent },
    {"GetModuleHandleA" ,(FUNC_) GetModuleHandleA },
    {"InitializeCriticalSection" ,(FUNC_) InitializeCriticalSection },
  //  {"InitializeCriticalSectionEx" ,(FUNC_) InitializeCriticalSectionEx },


    {"GetCurrentProcessId"  ,(FUNC_) GetCurrentProcessId },
    {"GetModuleHandleW"  ,(FUNC_) GetModuleHandleW },
    {"CreateEventW"  ,(FUNC_) CreateEventW },
    {"IsDBCSLeadByte"  ,(FUNC_) IsDBCSLeadByte },
    {"GetConsoleMode"  ,(FUNC_) GetConsoleMode },
    {"SetConsoleCtrlHandler"  ,(FUNC_) SetConsoleCtrlHandler },
    {"LoadStringW"  ,(FUNC_) LoadStringW },
    {"SetErrorMode"  ,(FUNC_) SetErrorMode },
    {"RegOpenKeyExW"  ,(FUNC_) RegOpenKeyExW },
    {"RegQueryValueExW"  ,(FUNC_) RegQueryValueExW },
    {"RegCloseKey"  ,(FUNC_) RegCloseKey },
    {"GetVersionExW"  ,(FUNC_) GetVersionExW },
    {"GetVersionExW"  ,(FUNC_) GetVersionExW },
    {"LoadLibraryW"  ,(FUNC_) LoadLibraryW },
    {"CreateFileW"  ,(FUNC_) CreateFileW },
    {"GetCurrentDirectoryW"  ,(FUNC_) GetCurrentDirectoryW },
    {"CharUpperW"  ,(FUNC_) CharUpperW },

  /*
    {"EnterCriticalSection" ,(FUNC_) My_EnterCriticalSection },//BUG
    {"LeaveCriticalSection" ,(FUNC_) My_LeaveCriticalSection }, //BUG
    {"DeleteCriticalSection" ,(FUNC_) My_DeleteCriticalSection },
    {"ExitProcess" ,(FUNC_) My_ExitProcess },
    {"LoadLibraryExW" ,(FUNC_) My_LoadLibraryEx },
    {"GetSystemTimeAsFileTime" ,(FUNC_) My_GetSystemTimeAsFileTime },
    {"IsProcessorFeaturePresent" ,(FUNC_) My_IsProcessorFeaturePresent },
    {"GetModuleHandleA" ,(FUNC_) My_GetModuleHandleA },
    {"InitializeCriticalSection" ,(FUNC_) My_InitializeCriticalSection },
    {"InitializeCriticalSectionEx" ,(FUNC_) My_InitializeCriticalSectionEx },
    {"SetUnhandledExceptionFilter" ,(FUNC_) My_SetUnhandledExceptionFilter },
    {"QueryPerformanceCounter"  ,(FUNC_) My_QueryPerformanceCounter },
    {"GetCurrentProcessId"  ,(FUNC_) My_GetCurrentProcessId },
*/
  #endif

//  {"_initterm"  ,(FUNC_) My_initterm },
  //{"__lconv_init"  ,(FUNC_) My_lconv_init },


    //// main ///
    //{"CreateSemaphoreW"  ,(FUNC_) CreateSemaphore },


   // {"CreateSemaphoreW"  ,(FUNC_) My_CreateSemaphore },
    {"CreateSemaphoreW"  ,(FUNC_) My_CreateSemaphore },
    {"GetLastError"  ,(FUNC_) My_GetLastError },








//{"TlsGetValue" ,(FUNC_) My_TlsGetValue },

{"getenv"  ,(FUNC_) My_getenv },
{"__getmainargs" ,(FUNC_) My_getmainargs },
{"__set_app_type" ,(FUNC_) My_set_app_type },
{"_setmode" ,(FUNC_) My_setmode },
{"__p__environ" ,(FUNC_) My_penviron },
{"__p__fmode" ,(FUNC_) My_pfmode },
{"signal" ,(FUNC_) My__signal },
{"atexit" ,(FUNC_) My_atexit },
{"_cexit" ,(FUNC_) My_cexit },
{"_onexit" ,(FUNC_) My_Onexit },
{"GetStartupInfo" ,(FUNC_) My_GetStartupInfo },

{"GetCommandLineA"  ,(FUNC_) My_GetCommandLineA },
{"GetStartupInfoA"  ,(FUNC_) My_GetStartupInfo },
{"GetCurrentThreadId"  ,(FUNC_) My_GetCurrentThreadId },

{"fputc"  ,(FUNC_) My_fputc },
{"WriteConsoleA"  ,(FUNC_) My_WriteConsoleA },



{"realloc"  ,(FUNC_) realloc },
{"memcpy"  ,(FUNC_) memcpy },
{"memset"  ,(FUNC_) memset },
{"free"  ,(FUNC_) free },
{"malloc"  ,(FUNC_) malloc },
{"calloc"  ,(FUNC_) calloc },
{"abort"  ,(FUNC_) abort },
{"scanf"  ,(FUNC_) scanf },
{"fwrite"  ,(FUNC_) fwrite },
{"printf"  ,(FUNC_) printf },
{"vfprintf"  ,(FUNC_) vfprintf },
{"strcmp"  ,(FUNC_) strcmp },
//{"localeconv"  ,(FUNC_) My_localeconv },
//{"localeconv"  ,(FUNC_) localeconv },


{"strlen"  ,(FUNC_) strlen },


//Math
{"abs"  ,(FUNC_) floor },
{"floor"  ,(FUNC_) floor },
{"ceil"  ,(FUNC_) ceil },
{"qsort"  ,(FUNC_) qsort },



///// Thread local Important? ///////
{"TlsAlloc"  ,(FUNC_) My_TlsAlloc },
{"TlsGetValue"  ,(FUNC_) My_TlsGetValue },
{"TlsSetValue"  ,(FUNC_) My_TlsSetValue },
///////////////////////////////////////



///////////////// ///////////////// /////////////////
///////////////// Windows  func //////////////////
#ifdef UseWinFunc

{"GetStdHandle"  ,(FUNC_) GetStdHandle },
{"GetConsoleWindow"  ,(FUNC_) GetConsoleWindow },
{"SetConsoleWindowInfo"  ,(FUNC_) SetConsoleWindowInfo },
{"setlocale"  ,(FUNC_) setlocale },
{"SetConsoleOutputCP"  ,(FUNC_) SetConsoleOutputCP },
{"SetConsoleTextAttribute"  ,(FUNC_) SetConsoleTextAttribute },
{"SetConsoleScreenBufferSize"  ,(FUNC_) SetConsoleScreenBufferSize },
{"MoveWindow"  ,(FUNC_) MoveWindow },

{"GetModuleFileNameW"  ,(FUNC_) GetModuleFileNameW },
{"GetFileAttributesExW"  ,(FUNC_) GetFileAttributesExW },


{"_wfopen"  ,(FUNC_) _wfopen },

{"wprintf"  ,(FUNC_) wprintf },
{"wsprintfW"  ,(FUNC_) wsprintfW }, //Not work??

{"SetLastError"  ,(FUNC_) SetLastError }, //Required !?
{"fread"  ,(FUNC_) fread },
{"MoveWindow"  ,(FUNC_) MoveWindow },
{"GetModuleFileNameW"  ,(FUNC_) GetModuleFileNameW }, //useless?

//Windows
{"SetWindowLongW"  ,(FUNC_) SetWindowLongW },
{"RegisterClassExW"  ,(FUNC_) RegisterClassExW },
{"CreateWindowExW"  ,(FUNC_) CreateWindowExW },
{"ShowWindow"  ,(FUNC_) ShowWindow },
{"CreateCompatibleDC"  ,(FUNC_) CreateCompatibleDC },
{"CreateDIBSection"  ,(FUNC_) CreateDIBSection },
{"SelectObject"  ,(FUNC_) SelectObject },
{"GetDC"  ,(FUNC_) GetDC },



//Timer
{"QueryPerformanceCounter"  ,(FUNC_) QueryPerformanceCounter },
{"QueryPerformanceFrequency"  ,(FUNC_) QueryPerformanceFrequency },

{"Sleep"  ,(FUNC_) Sleep },
{"BitBlt"  ,(FUNC_) BitBlt },

{"GetClientRect"  ,(FUNC_) GetClientRect },
{"GetWindowRect"  ,(FUNC_) GetWindowRect },

{"TranslateMessage"  ,(FUNC_) TranslateMessage },
{"DispatchMessageW"  ,(FUNC_) DispatchMessageW },
{"PeekMessageW"  ,(FUNC_) PeekMessageW },
{"GetWindowLongW"  ,(FUNC_) GetWindowLongW },
{"DefWindowProcW"  ,(FUNC_) DefWindowProcW },

{"LoadCursorW"  ,(FUNC_) LoadCursorW },
{"LoadIconW"  ,(FUNC_) LoadIconW },
{"MessageBoxW"  ,(FUNC_) MessageBoxW },


#else  //OnCpcDOs


{"GetStdHandle"  ,(FUNC_) My_GetStdHandle },
{"GetConsoleWindow"  ,(FUNC_) My_GetConsoleWindow },
{"SetConsoleWindowInfo"  ,(FUNC_) My_SetConsoleWindowInfo },
{"setlocale"  ,(FUNC_) My_setlocale },
{"SetConsoleOutputCP"  ,(FUNC_) My_SetConsoleOutputCP },
{"SetConsoleTextAttribute"  ,(FUNC_) My_SetConsoleTextAttribute },
{"SetConsoleScreenBufferSize"  ,(FUNC_) My_SetConsoleScreenBufferSize },
{"MoveWindow"  ,(FUNC_) My_MoveWindow },

{"GetModuleFileNameW"  ,(FUNC_) My_GetModuleFileNameW },
{"GetFileAttributesExW"  ,(FUNC_) My_GetFileAttributesExW },



{"_wfopen"  ,(FUNC_) My_wfopen },

{"wprintf"  ,(FUNC_) My_wprintf },
{"wsprintfW"  ,(FUNC_) My_wprintf }, //Not work??


{"SetLastError"  ,(FUNC_) My_SetLastError }, //Required !?
{"fread"  ,(FUNC_) My_fread },

{"MoveWindow"  ,(FUNC_) Nothing },
{"GetModuleFileNameW"  ,(FUNC_) Nothing }, //useless?

//Windows
{"SetWindowLongW"  ,(FUNC_) My_SetWindowLongW },
{"RegisterClassExW"  ,(FUNC_) My_RegisterClassExW },
{"CreateWindowExW"  ,(FUNC_) My_CreateWindowExW },
{"ShowWindow"  ,(FUNC_) My_ShowWindow },
{"CreateCompatibleDC"  ,(FUNC_) My_CreateCompatibleDC },
{"CreateDIBSection"  ,(FUNC_) My_CreateDIBSection },
{"SelectObject"  ,(FUNC_) My_SelectObject },
{"GetDC"  ,(FUNC_) My_GetDC },


//Timer
{"QueryPerformanceCounter"  ,(FUNC_) My_QueryPerformanceCounter },
{"QueryPerformanceFrequency"  ,(FUNC_) My_QueryPerformanceFrequency },

//{"Sleep"  ,(FUNC_) My_Sleep },


{"BitBlt"  ,(FUNC_) My_BitBlt },

{"GetClientRect"  ,(FUNC_) My_GetClientRect },
{"GetWindowRect"  ,(FUNC_) My_GetWindowRect },

{"TranslateMessage"  ,(FUNC_) My_TranslateMessage },
{"DispatchMessageW"  ,(FUNC_) My_DispatchMessageW },
{"PeekMessageW"  ,(FUNC_) My_PeekMessageW },
{"GetWindowLongW"  ,(FUNC_) My_GetWindowLongW },
{"DefWindowProcW"  ,(FUNC_) My_DefWindowProcW },

{"LoadCursorW"  ,(FUNC_) My_LoadCursorW },
{"LoadIconW"  ,(FUNC_) My_LoadIconW },
{"MessageBoxW"  ,(FUNC_) My_MessageBoxW },









#ifndef UseWinFunc
   // {"Sleep"  ,(FUNC_) Sleep },
#endif

#endif // UseWinFunc


////////////////////////////////////////////////////


#include "CpcDosFuncTable.h"
{"putchar"  ,(FUNC_) putchar },
{"puts"  ,(FUNC_) puts }


};










