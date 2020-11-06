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
* FuncPipe will remap a system function to a same signature custom function to see what is called,
* and then we choose what to do with it.
*
* Warning: Windows WINAPI function are __stdcall instead of __cdecl. 
* __stdcall remapped function must have the EXACT same paramters and must be specified as __stdcall
*  If not, your app will likely crash when the function return.
*
*/


#define showfunc_unimplt(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de Fonction non implémenté: " name, "\n-->Call not implemented func: " name , __VA_ARGS__);
#define showfunc(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de: " name, "\n-->Call: " name , __VA_ARGS__);
#define showfunc_ret(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Retour: " name, "\n-->Return: " name , __VA_ARGS__);

//#define Pipe_Show_AllFunc
#ifdef Pipe_Show_AllFunc
#define showfunc_opt showfunc
#else
#define showfunc_opt
#endif

//!ATOM RegisterClassW(const WNDCLASSW *lpWndClass)
inline ATOM WINAPI pipe_RegisterClassW(void* value){
	showfunc("RegisterClassW( value: %p )", value);
	#ifdef Func_Win
		return RegisterClassW((WNDCLASSW*)value);
	#else
		return 0;
	#endif
}

//!HMODULE LoadLibraryA(LPCSTR lpLibFileName)
inline HMODULE WINAPI pipe_LoadLibraryA(LPCSTR lpLibFileName){
	showfunc("LoadLibraryA( lpLibFileName: %s )", lpLibFileName);
	#ifdef USE_Windows_LoadLibrary
		HMODULE _ret = LoadLibraryA(lpLibFileName);
		if(!_ret){My_GetLastError();}return _ret;
	#else
		return (HMODULE)AddLibrary(lpLibFileName);
	#endif
}

//!LRESULT DispatchMessageA(const MSG *lpMsg)
inline LRESULT WINAPI pipe_DispatchMessageA(const MSG *lpMsg){
	showfunc_opt("DispatchMessageA( lpMsg: %s )", lpMsg);
	#ifdef Func_Win
		return DispatchMessageA(lpMsg);
	#else
		return 0;
	#endif
}


extern funcPtr_int _dFunc_wglGetPixelFormat;
//!int GetPixelFormat(HDC hdc)
inline BOOL WINAPI pipe_GetPixelFormat(HDC hdc){
	showfunc("GetPixelFormat( hdc: %p )", hdc);
	if(_dFunc_wglGetPixelFormat != 0){
		return _dFunc_wglGetPixelFormat(hdc);
	}
	
	#ifdef Func_Win
		return GetPixelFormat(hdc);
	#else
		return true;
	#endif
}
extern funcPtrIntPtr_bool _dFunc_wglSetPixelFormat;
//!BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR *ppfd)
inline BOOL WINAPI pipe_SetPixelFormat(void* hdc, int format, void* ppfd){
	showfunc("SetPixelFormat( hdc: %p, format: %d, ppfd: %p )", hdc, format, ppfd);
	if(_dFunc_wglSetPixelFormat != 0){
		return _dFunc_wglSetPixelFormat(hdc, format, ppfd);
	}
	
	#ifdef Func_Win
		return SetPixelFormat((HDC)hdc, format, (PIXELFORMATDESCRIPTOR*)ppfd);
	#else
		return true;
	#endif
}


extern funcPtrPtr_int _dFunc_wglChoosePixelFormat;
//!int ChoosePixelFormat( HDC hdc, const PIXELFORMATDESCRIPTOR *ppfd)
inline int WINAPI pipe_ChoosePixelFormat(void* hdc, void* ppfd){
	showfunc("ChoosePixelFormat( hdc: %p, ppfd: %p )", hdc, ppfd);
	if(_dFunc_wglChoosePixelFormat != 0){
		return _dFunc_wglChoosePixelFormat(hdc, ppfd);
	}
	
	//Check if we have wglChoosePixelFormat which is a better replacement
	//////////////////////////////////
	#ifdef Func_Win
		//_sapp.wgl.ChoosePixelFormat(_sapp.wgl.msg_dc, &pfd);
		int _ret = ChoosePixelFormat((HDC)hdc, (PIXELFORMATDESCRIPTOR*)ppfd);
		showfunc_ret("ChoosePixelFormat[ int: %d ]", _ret);return _ret;
	#else
		return 0;
	#endif
}

extern funcPtrIntIntPtr_int _dFunc_wglDescribePixelFormat;
//!int DescribePixelFormat(HDC hdc,int iPixelFormat,UINT nBytes,LPPIXELFORMATDESCRIPTOR ppfd)
inline int WINAPI pipe_DescribePixelFormat(HDC hdc,int iPixelFormat,UINT nBytes,LPPIXELFORMATDESCRIPTOR ppfd){
	showfunc("DescribePixelFormat( hdc: %p, iPixelFormat: %d, nBytes: %d, ppfd: %p )", hdc, iPixelFormat, nBytes, ppfd);
	if(_dFunc_wglDescribePixelFormat != 0){
		return _dFunc_wglDescribePixelFormat(hdc, iPixelFormat, nBytes, ppfd);
	}
	#ifdef Func_Win
		//_sapp.wgl.ChoosePixelFormat(_sapp.wgl.msg_dc, &pfd);
		return DescribePixelFormat((HDC)hdc, iPixelFormat, nBytes, ppfd);
	#else
		return 0;
	#endif
	
}

extern funcPtr_bool _dFunc_wglSwapBuffers;
//!BOOL SwapBuffers(HDC Arg1)
inline BOOL WINAPI pipe_SwapBuffers(HDC hdc){
	showfunc("SwapBuffers( hdc: %p )", hdc);
	if(_dFunc_wglSwapBuffers != 0){
		return _dFunc_wglSwapBuffers(hdc);
	}
	#ifdef Func_Win
		//_sapp.wgl.ChoosePixelFormat(_sapp.wgl.msg_dc, &pfd);
		return SwapBuffers((HDC)hdc);
	#else
		return false;
	#endif
}

//!int StretchDIBits(HDC hdc,int xDest,int yDest,int DestWidth,int DestHeight,int xSrc,int ySrc, int SrcWidth, int SrcHeight, const VOID *lpBits, const BITMAPINFO *lpbmi, UINT iUsage, DWORD rop)
int WINAPI pipe_StretchDIBits(HDC hdc,int xDest,int yDest,int DestWidth,int DestHeight,int xSrc,int ySrc, int SrcWidth, int SrcHeight, const VOID *lpBits, const BITMAPINFO *lpbmi, UINT iUsage, DWORD rop){
	showfunc("StretchDIBits( hdc: %p )", hdc);
	#ifdef Func_Win
		//_sapp.wgl.ChoosePixelFormat(_sapp.wgl.msg_dc, &pfd);
		return StretchDIBits(hdc, xDest, yDest, DestWidth, DestHeight, xSrc, ySrc, SrcWidth, SrcHeight, lpBits, lpbmi, iUsage, rop);
	#else
		return false;
	#endif
}


//!BOOL AdjustWindowRectEx(LPRECT lpRect,DWORD  dwStyle,BOOL bMenu,DWORD  dwExStyle)
inline BOOL WINAPI pipe_AdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle){
	showfunc("AdjustWindowRectEx( lpRect: %p, dwStyle: %d, bMenu: %d, dwExStyle: %p )", lpRect, dwStyle, bMenu, dwExStyle);
	#ifdef Func_Win
		return AdjustWindowRectEx(lpRect, dwStyle, bMenu, dwExStyle);
	#else
		return true;
	#endif
}

//!BOOL ShowWindow(HWND hWnd,int nCmdShow);
inline BOOL WINAPI  pipe_ShowWindow(HWND hWnd,int nCmdShow){
	showfunc("ShowWindow( hWnd: %p, nCmdShow: %d)", hWnd, nCmdShow);
	#ifdef Func_Win
		return ShowWindow(hWnd, nCmdShow);
	#else
		return true;
	#endif
}


//!HMODULE GetModuleHandleA(LPCSTR  lpModuleName)
//!HMODULE GetModuleHandleW(LPCWSTR lpModuleName)
inline HMODULE  WINAPI pipe_GetModuleHandleA(LPCSTR lpModuleName){
	showfunc("GetModuleHandleA( lpModuleName: %s)", lpModuleName);
	#ifdef Func_Win
		return GetModuleHandleA(lpModuleName);
	#else
		return 0;
	#endif
}
inline HMODULE  WINAPI pipe_GetModuleHandleW(LPCWSTR lpModuleName){
	showfunc("GetModuleHandleW( lpModuleName: %s)", lpModuleName); //TODO ewide str
	#ifdef Func_Win
		return GetModuleHandleW(lpModuleName);
	#else
		return 0;
	#endif
}

//!HCURSOR LoadCursorA(HINSTANCE hInstance,LPCSTR lpCursorName)
//!HCURSOR LoadCursorW(HINSTANCE hInstance,LPCWSTR lpCursorName)
HCURSOR WINAPI pipe_LoadCursorA(HINSTANCE hInstance, LPCSTR lpCursorName){
	//showfunc("LoadCursorA( hInstance: %p, lpCursorName: %s)", hInstance, lpCursorName);
	showfunc("LoadCursorA( hInstance: %p, lpCursorName: %p)", hInstance, lpCursorName);
	#ifdef Func_Win
		return LoadCursorA(hInstance, lpCursorName);
	#else
		return 0;
	#endif
}
HCURSOR WINAPI pipe_LoadCursorW(HINSTANCE hInstance, LPCWSTR lpCursorName){
	showfunc("LoadCursorW( hInstance: %p, lpCursorName: %p)", hInstance, lpCursorName);//TODO ewide str
	#ifdef Func_Win
		return LoadCursorW(hInstance, lpCursorName);
	#else
		return 0;
	#endif
}

//!HICON LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName)
//!HICON LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName)
HICON WINAPI pipe_LoadIconA(HINSTANCE hInstance, LPCSTR lpIconName){
	showfunc("LoadIconA( hInstance: %p, lpIconName: %p)", hInstance, lpIconName);
	#ifdef Func_Win
		return LoadIconA(hInstance, lpIconName);
	#else
		return 0;
	#endif
}
HICON WINAPI pipe_LoadIconW(HINSTANCE hInstance, LPCWSTR lpIconName){
	showfunc("LoadIconW( hInstance: %p, lpIconName: %p)", hInstance, lpIconName);//TODO ewide str
	#ifdef Func_Win
		return LoadIconW(hInstance, lpIconName);
	#else
		return 0;
	#endif
}

//!HDC GetDC(HWND hWnd)
inline HDC WINAPI pipe_GetDC(HWND hWnd){
	showfunc("GetDC( lpModuleName: %p)", hWnd); 
	#ifdef Func_Win
		return GetDC(hWnd);
	#else
		return 0;
	#endif
}

//!BOOL WINAPI CloseHandle(HANDLE hObject)
inline BOOL WINAPI pipe_CloseHandle(HANDLE hObject){
	showfunc("CloseHandle( hObject: %p)", hObject); 
	#ifdef Func_Win
		return CloseHandle(hObject);
	#else
		return true;
	#endif
}



//!SetWindowsHookExA(int idHook,HOOKPROC lpfn,HINSTANCE hmod,DWORD     dwThreadId)
inline HHOOK WINAPI pipe_SetWindowsHookExA(int idHook,HOOKPROC lpfn,HINSTANCE hmod,DWORD     dwThreadId){
	showfunc("SetWindowsHookExA( idHook: %d, lpfn:%p, hmod:%p, dwThreadId:%p )", idHook, lpfn, hmod, dwThreadId);
	#ifdef Func_Win
		return SetWindowsHookExA(idHook,lpfn,hmod,dwThreadId);
	#else
		return 0;
	#endif
}

//!LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam,LPARAM lParam)
inline LRESULT WINAPI pipe_CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam,LPARAM lParam){
	showfunc("CallNextHookEx( hhk: %d, nCode:%d, wParam:%p, lParam:%p )", hhk, nCode, wParam, lParam);
	#ifdef Func_Win
		return CallNextHookEx(hhk,nCode,wParam,lParam);
	#else
		return 0;
	#endif
}

//!BOOL EnumDisplaySettingsA(LPCSTR lpszDeviceName,DWORD ModeNum,DEVMODEA *lpDevMode)
inline BOOL WINAPI pipe_EnumDisplaySettingsA(LPCSTR lpszDeviceName,DWORD ModeNum,DEVMODEA *lpDevMode){
	showfunc("EnumDisplaySettingsA( lpszDeviceName: %p, ModeNum: %d, lpDevMode: %d)", lpszDeviceName, ModeNum, lpDevMode);
	#ifdef Func_Win
		return EnumDisplaySettingsA(lpszDeviceName,ModeNum,lpDevMode);
	#else
		return 0;
	#endif

}

//!int _set_error_mode(int mode_val)
int pipe_set_error_mode(int mode_val){
	showfunc("_set_error_mode( mode_val: %d )", mode_val);
	return 0;
}

//!int setvbuf ( FILE * stream, char * buffer, int mode, size_t size );
inline int pipe_setvbuf( FILE * stream, char * buffer, int mode, size_t size ){
	showfunc("pipe_setvbuf( stream: %p, buffer: %p, mode: %d, size:%d )", stream, buffer, mode, size);
	return 0; //TODO crash?
//	#ifdef Func_Win
//		return setvbuf(stream, buffer, mode, size);
	/*
	#else
	return 0;
	#endif
	*/
}

//!void __cdecl _lock(int locknum)
inline void  pipe_lock(int locknum){
	showfunc_opt("_lock( locknum: %d )", locknum);
	#ifdef Func_Win
		//_lock(locknum);
	#else
	#endif
}

//!void __cdecl _unlock(int locknum)
inline void  pipe_unlock(int locknum){
	showfunc_opt("_unlock( locknum: %d )", locknum);
	#ifdef Func_Win
	//_unlock(locknum);
	#else
	#endif
}

//!HANDLE  CreateToolhelp32Snapshot(DWORD  dwFlags,DWORD  th32ProcessID)
inline HANDLE pipe_CreateToolhelp32Snapshot(DWORD  dwFlags,DWORD  th32ProcessID){
	showfunc("CreateToolhelp32Snapshot( dwFlags: %p, th32ProcessID: %p )", dwFlags,th32ProcessID);
	return 0;
}

//!BOOL Thread32First(HANDLE hSnapshot,LPTHREADENTRY32 lpte)
BOOL pipe_Thread32First(HANDLE hSnapshot,void* lpte){
	showfunc("Thread32First( hSnapshot: %p, lpte: %p )", hSnapshot,lpte);
	return 0;
}

//!BOOL Thread32Next(HANDLE hSnapshot,LPTHREADENTRY32 lpte)
BOOL pipe_Thread32Next(HANDLE hSnapshot,void* lpte){
	showfunc("Thread32Next( hSnapshot: %p, lpte: %p )", hSnapshot,lpte);
	return 0;
}

//!void __cdecl _initterm(PVFV *,PVFV *);
typedef void (__cdecl *_PVFV)();
inline void pipe_initterm(_PVFV* ppfn,_PVFV* end){
	showfunc("_initterm( ppfn: %p, end: %p )", ppfn,end);
	do {
        if (_PVFV pfn = *++ppfn){
            pfn();
        }
    } while (ppfn < end);
}

//!void __cdecl _initterm(PVFV *,PVFV *);
typedef int  (__cdecl *_PIFV)();
inline int pipe_initterm_e(_PIFV* ppfn,_PIFV* end){
	showfunc("_initterm_e( ppfn: %p, end: %p )", ppfn,end);
	do {
        if (_PIFV pfn = *++ppfn){
            if (int err = pfn()) return err;
        }
    } while (ppfn < end);
    return 0;
}

//!HANDLE CreateSemaphore(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,_In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCTSTR lpName)
HANDLE  WINAPI  pipe_CreateSemaphore( //Must have __stdcall
 _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  _In_     LONG                  lInitialCount,
  _In_     LONG                  lMaximumCount,
  _In_opt_ LPCTSTR               lpName
)
{
	showfunc_unimplt("_CreateSemaphore(  )","");
	#ifdef Func_Win
	return CreateSemaphoreA(lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName);
	#else
	#endif
	
	return 0;
}




//!void GetSystemInfo( LPSYSTEM_INFO lpSystemInfo)
inline void pipe_GetSystemInfo( LPSYSTEM_INFO lpSystemInfo){
	//A pointer to a SYSTEM_INFO structure that receives the information.
	showfunc("GetSystemInfo( lpSystemInfo:%p )",lpSystemInfo);
	#ifdef Func_Win
	GetSystemInfo(lpSystemInfo);
	#else
	#endif
}

//!HWND WindowFromDC(HDC hDC)
inline WINAPI HWND pipe_WindowFromDC(HDC hDC){
	showfunc("WindowFromDC( hDC:%p )",hDC);
	#ifdef Func_Win
	return WindowFromDC(hDC);
	#else
	return 0;
	#endif
}

//!BOOL ClientToScreen(HWND    hWnd,LPPOINT lpPoint)
inline WINAPI BOOL pipe_ClientToScreen(HWND hWnd,LPPOINT lpPoint){
	showfunc("ClientToScreen( hWnd:%p, lpPoint.x:%d, lpPoint.y:%d )",hWnd, lpPoint->x, lpPoint->y);
	#ifdef Func_Win
	return ClientToScreen(hWnd, lpPoint);
	#else
	return true;
	#endif
}


//!int _vscprintf(const char *format,va_list argptr)
inline int pipe_vscprintf(const char *format,va_list argptr){
	showfunc_opt("_vscprintf( )");
    int retval = 0; 
    va_list argcopy;
    va_copy(argcopy, argptr); 
	#ifdef Func_Win
    retval = vsnprintf(NULL, 0, format, argcopy); 
	#else
	//TOODO
	#endif
    va_end(argcopy); 
    return retval;
 }

//! void * _aligned_malloc(size_t size,size_t alignment)
inline void* pipe_aligned_malloc(size_t size,size_t alignment){
	showfunc_opt("aligned_malloc( size: %d, alignment: %d )", size,alignment);
	void* p1; // original block
    void** p2; // aligned block
    int offset = alignment - 1 + sizeof(void*);
    if ((p1 = (void*)malloc(size + offset)) == NULL)
    {
       return NULL;
    }
    p2 = (void**)(((size_t)(p1) + offset) & ~(alignment - 1));
    p2[-1] = p1;
    return p2;
}
//!void _aligned_free (void *memblock)
inline void pipe_aligned_free(void *memblock){
	showfunc_opt("aligned_free( memblock: %p )", memblock);
	if(memblock != 0){
		free(((void**)memblock)[-1]);
	}
}
//! void * _aligned_realloc(void *memblock,size_t size,size_t alignment);
inline void* pipe_aligned_realloc(void *memblock,size_t size,size_t alignment){
	showfunc_opt("aligned_realloc( size: %d, alignment: %d )", size,alignment);
	pipe_aligned_free(memblock);
	return pipe_aligned_malloc(size, alignment);
}


//!char *_strdup(const char *strSource)
inline char* pipe_strdup(const char *strSource){
	showfunc("_strdup( strSource: %s )", strSource);
	size_t size = strlen(strSource) + 1;
	char* str = (char*)malloc(size);
	if (str) {memcpy(str, strSource, size);}
	return str;
}


//!char * strncpy( char * destination, const char * source, size_t num )
inline char* pipe_strncpy( char * destination, const char * source, size_t num ){
	showfunc_opt("strncpy( destination: %p, source: %p, num: %d )", destination, source, num);
	size_t i = 0;
	while(i++ != num && (*destination++ = *source++));
	return destination;
}

//!int isspace ( int c )
inline int pipe_isspace( int c ){
	showfunc_opt("isspace( c %d )", c);
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; // || whatever other char you consider space
}

//!int isupper ( int c )
inline int  pipe_isupper( int c ){
	showfunc_opt("isupper( c %d )", c);
	return (c >= 'A' && c <= 'Z');
}

//!int islower ( int c )
inline int  pipe_islower( int c ){
	showfunc_opt("islower( c %d )", c);
	return (c >= 'a' && c <= 'z');
}

//!LPVOID VirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect)
inline LPVOID pipe_VirtualAlloc(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect){
	showfunc_opt("VirtualAlloc( lpAddress %p, dwSize: %d, flAllocationType: %d, flProtect:%d )", lpAddress, dwSize, flAllocationType, flProtect);
	#ifdef Func_Win
    return VirtualAlloc(lpAddress, dwSize, flAllocationType, flProtect); 
	#else
	//TOODO
	#endif
}

//!BOOL VirtualFree(LPVOID lpAddress,SIZE_T dwSize,DWORD  dwFreeType)
inline BOOL pipe_VirtualFree(LPVOID lpAddress,SIZE_T dwSize,DWORD  dwFreeType){
	showfunc_opt("VirtualFree( lpAddress %p, dwSize: %d, dwFreeType:%d )", lpAddress, dwSize, dwFreeType);
	#ifdef Func_Win
    return VirtualFree(lpAddress, dwSize, dwFreeType); 
	#else
	//TOODO
	#endif
}