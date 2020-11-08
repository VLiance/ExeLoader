/*  -== ExeLoader ==-
 *
 *  Load .exe / .dll from memory and remap functions
 *  Run your binaries on any x86 hardware
 *
 *  @autors
 *   - Maeiky
 *   - Sebastien FAVIER
 *  
 * Copyright (c) 2020 - V·Liance / SPinti-Software. All rights reserved.
 *
 * The contents of this file are subject to the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * If a copy of the Apache License Version 2.0 was not distributed with this file,
 * You can obtain one at https://www.apache.org/licenses/LICENSE-2.0.html

* Description:
* 
* Remap system function to custom ones 
*
*
* Usefull declaration                ------------------------    decorated
//void __cdecl foo(void);            ----------------------->    _foo
//void __cdecl foo(int a);           ----------------------->    _foo
//void __cdecl foo(int a, int b);    ----------------------->    _foo
//void __stdcall foo(void);          ----------------------->    _foo@0
//void __stdcall foo(int a);         ----------------------->    _foo@4
//void __stdcall foo(int a, int b);  ----------------------->    _foo@8
//void __fastcall foo(void);         ----------------------->    @foo@0
//void __fastcall foo(int a);        ----------------------->    @foo@4
//void __fastcall foo(int a, int b); ----------------------->    @foo@8
*
*/
#ifndef EXELOADER_FuncTable_H
#define EXELOADER_FuncTable_H

#include "ExeLoader.h"

#ifndef ImWin
#define InCpcDosCore
#include "CPC_WPR.h"
#endif // ImWin

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <errno.h>
#include <conio.h>
#include <cassert>
#include <cstdarg>
#include <setjmp.h>
#include <stdlib.h>  

#include "FuncTable/DummyTable.h"
#include "FuncTable/FuncTable_Imp.h"
#include "FuncTable/FuncTable_Sys.h"
#include "FuncTable/FuncTable_Pipe.h"
#include "FuncTable/FuncTable_Remap_Common.h"
#include "FuncTable/FuncTable_Remap_Windows.h"
	
#ifndef UseWinFunc
	//onCpcDos
	#include "FuncTableRemap_CpcDos.h"
#endif

 sFunc aTableFunc[] = {
 ////////// CPC DOS ///////////////////
#include "FuncTable/CpcDosFuncTable.h"
//////////////////////////////////////

{"fNotImplemented" 	,(FUNC_) fNotImplemented },    	//Must be first
{"GetProcAddress" 	,(FUNC_) imp_GetProcAddress }, 	//Special
{"LoadLibraryA"    	,(FUNC_) imp_LoadLibraryA },  	//Special
{"LoadLibraryExW"  	,(FUNC_) LoadLibraryExW }, 		//TODO

/////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// FUNC TABLE /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

//////// Implemented ////////////////////////////////////////
{"_aligned_malloc"	,(FUNC_) imp_aligned_malloc },
{"_aligned_realloc"	,(FUNC_) imp_aligned_realloc },
{"_aligned_free"  	,(FUNC_) imp_aligned_free },

{"_strdup"  		,(FUNC_) imp_strdup },
{"strncpy"  		,(FUNC_) imp_strncpy },
{"isspace"  		,(FUNC_) imp_isspace },
{"isupper"  		,(FUNC_ )imp_isupper },
{"islower"  		,(FUNC_) imp_islower },

{"_lock"  			,(FUNC_) imp_lock },
{"_unlock"  		,(FUNC_) imp_unlock },
{"_initterm"  		,(FUNC_) imp_initterm },
{"_initterm_e"  	,(FUNC_) imp_initterm_e },
{"_vscprintf"  		,(FUNC_) imp_vscprintf },
////////////////////////////////////////////////////////////

////// Special func WGL ////////////////////////////////////
{"GetPixelFormat"  		,(FUNC_) pipe_GetPixelFormat }, 
{"SetPixelFormat"  		,(FUNC_) pipe_SetPixelFormat }, 
{"ChoosePixelFormat"  	,(FUNC_) pipe_ChoosePixelFormat },
{"DescribePixelFormat"  ,(FUNC_) pipe_DescribePixelFormat },
{"SwapBuffers"  		,(FUNC_) pipe_SwapBuffers },
////////////////////////////////////////////////////////////

{"VirtualAlloc"  		,(FUNC_) pipe_VirtualAlloc },
{"VirtualFree"  		,(FUNC_) pipe_VirtualFree },

{"setvbuf"  			,(FUNC_) pipe_setvbuf },
{"_set_error_mode" 		,(FUNC_) pipe_set_error_mode },

{"RegisterClassW"  		,(FUNC_) pipe_RegisterClassW }, 
{"AdjustWindowRectEx"  	,(FUNC_) pipe_AdjustWindowRectEx }, 

{"DispatchMessageA"  	,(FUNC_) pipe_DispatchMessageA }, 

{"GetSystemInfo"  		,(FUNC_) pipe_GetSystemInfo }, 
{"WindowFromDC"  		,(FUNC_) pipe_WindowFromDC }, 
{"ClientToScreen"  		,(FUNC_) pipe_ClientToScreen }, 

{"StretchDIBits"  		,(FUNC_) pipe_StretchDIBits },

{"CallNextHookEx"  		,(FUNC_) pipe_CallNextHookEx },
{"EnumDisplaySettingsA" ,(FUNC_) pipe_EnumDisplaySettingsA },

{"CreateToolhelp32Snapshot"  ,(FUNC_) pipe_CreateToolhelp32Snapshot },
{"Thread32First"  		,(FUNC_) pipe_Thread32First },
{"Thread32Next"  		,(FUNC_) pipe_Thread32Next },

{"ShowWindow"  			,(FUNC_) pipe_ShowWindow },

{"SetWindowsHookExA" 	,(FUNC_) pipe_SetWindowsHookExA },
{"GetModuleHandleA" 	,(FUNC_) pipe_GetModuleHandleA },
{"GetModuleHandleW"  	,(FUNC_) pipe_GetModuleHandleW },

{"GetDC"  				,(FUNC_) pipe_GetDC },
{"CloseHandle"  		,(FUNC_) pipe_CloseHandle },

{"LoadCursorA" 			,(FUNC_) pipe_LoadCursorA },
{"LoadCursorW" 			,(FUNC_) pipe_LoadCursorW },
{"LoadIconA"   			,(FUNC_) pipe_LoadIconA },
{"LoadIconW"   			,(FUNC_) pipe_LoadIconW },

///////////////// System //////////////
{"GetDpiForMonitor"  		,(FUNC_) sys_GetDpiForMonitor }, //Shcore.dll
{"SetProcessDpiAwareness"  	,(FUNC_) sys_SetProcessDpiAwareness }, //Shcore.dll
{"SetProcessDPIAware"  		,(FUNC_) sys_SetProcessDPIAware }, //Shcore.dll

{"CommandLineToArgvW"  	,(FUNC_) pipe_CommandLineToArgvW },
{"GetCommandLineW"  	,(FUNC_) pipe_GetCommandLineW },
{"ScreenToClient"  		,(FUNC_) pipe_ScreenToClient },

////////////////////////////////////////

{"GetCursorInfo"  ,(FUNC_) pipe_GetCursorInfo },
{"SetWindowPos"  ,(FUNC_) SetWindowPos },
{"MonitorFromRect"  ,(FUNC_) MonitorFromRect },
{"GetMonitorInfoW"  ,(FUNC_) GetMonitorInfoW },

{"InitializeCriticalSectionEx" ,(FUNC_) My_InitializeCriticalSectionEx },
{"IsProcessorFeaturePresent" ,(FUNC_) My_IsProcessorFeaturePresent },

{"InitializeCriticalSectionAndSpinCount"  ,(FUNC_) InitializeCriticalSectionAndSpinCount },
{"HeapAlloc"  ,(FUNC_) HeapAlloc },
{"GetProcessHeap"  ,(FUNC_) GetProcessHeap },
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


{"CreateThread" ,(FUNC_) CreateThread },
{"EnterCriticalSection" ,(FUNC_) EnterCriticalSection },//BUG
{"LeaveCriticalSection" ,(FUNC_) LeaveCriticalSection }, //BUG
{"DeleteCriticalSection" ,(FUNC_) DeleteCriticalSection },
{"ExitProcess" ,(FUNC_) ExitProcess },
{"LoadLibraryExW" ,(FUNC_) LoadLibraryExW },
{"GetSystemTimeAsFileTime" ,(FUNC_) GetSystemTimeAsFileTime },
{"IsProcessorFeaturePresent" ,(FUNC_) IsProcessorFeaturePresent },

	
{"InitializeCriticalSection" ,(FUNC_) InitializeCriticalSection },
//{"InitializeCriticalSectionEx" ,(FUNC_) InitializeCriticalSectionEx },

{"GetCurrentProcessId"  ,(FUNC_) GetCurrentProcessId },

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
{"printf"  ,(FUNC_) printf },
{"Sleep", 	(FUNC_) Sleep },
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


{"CreateCompatibleDC"  ,(FUNC_) CreateCompatibleDC },
{"CreateDIBSection"  ,(FUNC_) CreateDIBSection },
{"SelectObject"  ,(FUNC_) SelectObject },




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

	//{"LoadCursorW"  ,(FUNC_) LoadCursorW },
	//{"LoadIconW"  ,(FUNC_) LoadIconW },
	{"MessageBoxW"  ,(FUNC_) MessageBoxW },

	
	{"fgets"  ,(FUNC_) fgets },

#ifdef InCpcDosCore
	////////////////////
	////// CpcDos //////
	////////////////////
  
	//{"abs"  ,(FUNC_) abs_CPC },  ///BUG!!
	
	{"fgets"  ,(FUNC_) cpc_fgets },
	{"_fgets"  ,(FUNC_) cpc_fgets },

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
	{"fread"  ,(FUNC_) fread },

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

	{"GetCurrentProcessId "  ,(FUNC_) My_GetCurrentProcessId },
	{"GetTickCount "  ,(FUNC_) clock() },

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

//	{"LoadCursorW"  ,(FUNC_) My_LoadCursorW },
// 	{"LoadIconW"  ,(FUNC_) My_LoadIconW },
	{"MessageBoxW"  ,(FUNC_) My_MessageBoxW },
	
	
#endif /////////////////////////////////////


{"GetLastError"  ,(FUNC_) My_GetLastError },

{"CreateSemaphoreW"  ,(FUNC_) pipe_CreateSemaphore },
{"___lc_codepage_func"  ,(FUNC_) My__lc_codepage_func },
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
{"exit" ,(FUNC_) My_exit },
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


{"vfprintf"  ,(FUNC_) vfprintf },





#ifdef USE_Platform_ThreadStorage
	{"TlsAlloc"  ,	 (FUNC_) imp_TlsAlloc },
	{"TlsGetValue"  ,(FUNC_) imp_TlsGetValue },
	{"TlsSetValue"  ,(FUNC_) imp_TlsSetValue },
	{"TlsFree"  	,(FUNC_) imp_TlsFree },
#else
	{"TlsAlloc"  ,	 (FUNC_) TlsAlloc },
	{"TlsGetValue"  ,(FUNC_) TlsGetValue },
	{"TlsSetValue"  ,(FUNC_) TlsSetValue },
	{"TlsFree"  	,(FUNC_) TlsFree },
#endif




//Todo a implémenter
#ifdef InCpcDosCore
#else
	/// UNICODE (not supported on CpcDos)
	{"wcslen"  ,(FUNC_) wcslen },
	{"_strnicmp"  ,(FUNC_) _strnicmp },
	{"fwprintf"  ,(FUNC_) fwprintf },
	{"_vsnprintf"  ,(FUNC_) _vsnprintf },
	{"fputwc"  ,(FUNC_) fputwc },
	{"putwc"  ,(FUNC_) putwc },
	{"getwc"  ,(FUNC_) getwc },

	{"_stricmp"  ,(FUNC_) _stricmp }, //Use stricmp?
	{"GetFileAttributesW"  ,(FUNC_) GetFileAttributesW },
#endif

//{"setbuf"  ,(FUNC_) My_setbuf }, //!!!! Warning Dangerous function!
//{"LocalFree"  			,(FUNC_) LocalFree },

{"FreeLibrary"  		,(FUNC_) FreeLibrary },
{"localeconv"  ,(FUNC_) localeconv },






/*
{"_write"  		,(FUNC_) fwrite }, //??
{"_snwprintf"  	,(FUNC_) snprintf },//??
{"wcscpy"  		,(FUNC_) strcpy },  //??
*/

/////////////////////////////////////////////
/////////// DIRECT MAPPING //////////////////
/// *safe enough for a direct replacement* ///
/////////////////////////////////////////////
/////////////////////////////////////////////

/////////// MEM ////////////////////
{"memcmp"  		,(FUNC_) memcmp },
{"memmove"  	,(FUNC_) memmove },
////////////////////////////////////

/////////// LOG ////////////////////
{"fflush"  	,(FUNC_) fflush },
{"fprintf"  ,(FUNC_) fprintf },
////////////////////////////////////

/////////// STRING ///////////////
{"strcmp"  		,(FUNC_) strcmp },
{"stricmp"		,(FUNC_) stricmp },
{"sprintf"  ,(FUNC_) sprintf },
{"strcat"  	,(FUNC_) strcat },
{"strchr"  	,(FUNC_) strchr },
{"strcpy"  	,(FUNC_) strcpy },
{"strncmp"  ,(FUNC_) strncmp },
{"strtok"  	,(FUNC_) strtok },
{"strtol"  	,(FUNC_) strtol },
{"time"  	,(FUNC_) time },
{"strrchr"  ,(FUNC_) strrchr },
{"srand"  	,(FUNC_) srand },
{"atoi"  	,(FUNC_) atoi },
{"isdigit"  ,(FUNC_) isdigit },
{"strstr"  	,(FUNC_) strstr },
{"strlen"  	,(FUNC_) strlen },
{"memchr"  	,(FUNC_) memchr },
{"tolower"  ,(FUNC_) tolower },
////////////////////////////////

/////////// CHAR ////////////////
{"putchar"  ,(FUNC_) putchar },
{"puts"  	,(FUNC_) puts },
{"getc"  	,(FUNC_) getc },
{"fgetc"  	,(FUNC_) fgetc },
{"putc"  	,(FUNC_) putc },
{"fputs"  	,(FUNC_) fputs },
{"putchar"  ,(FUNC_) putchar },
{"getchar"  ,(FUNC_) getchar },
{"getch"  	,(FUNC_) getch },
/////////////////////////////////

/////////// MATH ////////////////
{"floor"  	,(FUNC_) floor },
{"ceil"  	,(FUNC_) ceil },
{"ceil"  	,(FUNC_) ceil },
{"fabs"  	,(FUNC_) fabs  }, 
{"qsort"  	,(FUNC_) qsort },
{"tan"  	,(FUNC_) tan },
{"rand"  	,(FUNC_) rand },
{"abs"  	,(FUNC_) imp_abs }, 
/////////////////////////////////

/////////// FILE ////////////////
{"fopen"  	,(FUNC_) fopen },
{"fclose"  	,(FUNC_) fclose },
{"fseek"  	,(FUNC_) fseek },
{"ftell"  	,(FUNC_) ftell },
{"rewind"  	,(FUNC_) rewind },
/////////////////////////////////

};

#endif //EXELOADER_FuncTable_H