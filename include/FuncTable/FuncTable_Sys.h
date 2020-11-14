/*  -== ExeLoader ==-
 *
 *  Load .exe / .dll from memory and remap functions
 *  Run your binaries on any x86 hardware
 *
 *  @autors
 *   - Maeiky
 *  
 * Copyright (c) 2020 - V·Liance / SPinti-Software. All rights reserved.
 *
 * The contents of this file are subject to the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * If a copy of the Apache License Version 2.0 was not distributed with this file,
 * You can obtain one at https://www.apache.org/licenses/LICENSE-2.0.html

* Description:
* 
* FuncTable_Sys is an attempt to remake system functions, or redirect them, 
* this is a mix between FuncTable_Pipe & FuncTable_Imp
* 
*/

//!VOID WINAPI SetLastError (DWORD dwErrCode)
DWORD last_error = 0;
VOID WINAPI sys_SetLastError (DWORD dwErrCode){
	showfunc_opt("SetLastError( dwErrCode: %d)", dwErrCode); 
	#ifdef Func_Win
		SetLastError(dwErrCode);
	#else
	last_error = dwErrCode;
	#endif
}

//!DWORD WINAPI GetLastError (VOID)
DWORD WINAPI sys_GetLastError(VOID){
	showfunc_opt("GetLastError( )", ""); 
	#ifdef Func_Win
	DWORD error = GetLastError();
	if (error){
		LPVOID lpMsgBuf;
		DWORD bufLen = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER |
										FORMAT_MESSAGE_FROM_SYSTEM |
										FORMAT_MESSAGE_IGNORE_INSERTS,
										NULL,error,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
		if (bufLen){
		  LPCSTR lpMsgStr = (LPCSTR)lpMsgBuf;
		  std::string result(lpMsgStr, lpMsgStr+bufLen);
		  LocalFree(lpMsgBuf);
		  showinf("GetLastError:%s", result.c_str());
		}

	}
	return error;
	#else
	 return last_error;
	#endif
}

//!HDC GetDC(HWND hWnd)
int aDC[10] = {};
int aDC_curr = 0;
inline HDC WINAPI sys_GetDC(HWND hWnd){
	showfunc("GetDC( lpModuleName: %p)", hWnd); 
	#ifdef Func_Win
		return GetDC(hWnd);
	#else
		aDC_curr++;
		return (HDC)aDC_curr;  //TODO
	#endif
}

//!HWND WindowFromDC(HDC hDC)
inline WINAPI HWND pipe_WindowFromDC(HDC hDC){
	showfunc("WindowFromDC( hDC:%p )",hDC);
	#ifdef Func_Win
	return WindowFromDC(hDC);
	#else
	return (HWND)hDC;//Test (Required for SetPixelFormat)
	#endif
}


//!HWND WINAPI CreateWindowExW(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam)
HWND WINAPI pipe_CreateWindowExW(DWORD dwExStyle,LPCWSTR lpClassName,LPCWSTR lpWindowName,DWORD dwStyle,int X,int Y,int nWidth,int nHeight,HWND hWndParent,HMENU hMenu,HINSTANCE hInstance,LPVOID lpParam){
	showfunc("CreateWindowExW( dwExStyle: %d, lpClassName: %p, lpWindowName :%d, dwStyle: %d, X: %d, Y: %d, nWidth: %d, nHeight: %d, hWndParent: %p, hMenu: %p, hInstance: %d, lpParam: %d )",
								dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	#ifdef Func_Win
		return CreateWindowExW( dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam );
	#else
		return (HWND)1; //TODO
	#endif
}

//!HRESULT GetDpiForMonitor(HMONITOR hmonitor, MONITOR_DPI_TYPE dpiType,UINT *dpiX,UINT *dpiY)
HRESULT sys_GetDpiForMonitor(HMONITOR hmonitor,int dpiType,UINT* dpiX,UINT* dpiY){
	showfunc("GetDpiForMonitor( hmonitor: %p, dpiType: %d, dpiX: %p,  dpiY: %p )", hmonitor, dpiType, dpiX, dpiY);
	*dpiX = 0;
	*dpiY = 0;
	return 0;
}

//!BOOL SetProcessDPIAware()
inline BOOL sys_SetProcessDPIAware(){
	showfunc("SetProcessDPIAware( )","");
	return true;
}

//!HRESULT SetProcessDpiAwareness(PROCESS_DPI_AWARENESS value)
inline HRESULT sys_SetProcessDpiAwareness(int value){
	showfunc("SetProcessDpiAwareness( value: %d )",value);
	return 0;
}

//!WINBOOL WINAPI QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount)

WINBOOL WINAPI sys_QueryPerformanceCounter (LARGE_INTEGER *lpPerformanceCount){
   	showfunc("QueryPerformanceCounter(lpPerformanceCount)", lpPerformanceCount);
	#ifdef Func_Win
		return QueryPerformanceCounter( lpPerformanceCount);
	#else
		LARGE_INTEGER lpPerformanceCount_ = {521891041};//Dummy value
		*lpPerformanceCount = lpPerformanceCount_;
		return true;
	#endif
}

//!WINBOOL WINAPI QueryPerformanceFrequency (LARGE_INTEGER *lpFrequency)
WINBOOL WINAPI sys_QueryPerformanceFrequency(LARGE_INTEGER *lpFrequency){
   	showfunc("QueryPerformanceFrequency( lpFrequency: %p )", lpFrequency);
	#ifdef Func_Win
		return QueryPerformanceFrequency( lpFrequency);
	#else
		LARGE_INTEGER lpFrequency_ = {8221038}; //Dummy value
		*lpFrequency = lpFrequency_;
		return true;
	#endif
}

//!DWORD WINAPI GetTickCount (VOID)
DWORD WINAPI sys_GetTickCount(){
 	showfunc("GetTickCount( )", "");
	#ifdef Func_Win
		return GetTickCount();
	#else
		return 0;
	#endif
}

//!DWORD WINAPI GetCurrentThreadId (VOID)
DWORD WINAPI sys_GetCurrentThreadId(){
 	showfunc("GetCurrentThreadId( )", "");
	#ifdef Func_Win
		return GetCurrentThreadId();
	#else
		return 1;//TODO
	#endif
}

//!DWORD WINAPI GetCurrentThreadId (VOID)
DWORD WINAPI sys_GetCurrentProcessId(){
 	showfunc("GetCurrentProcessId( )", "");
	#ifdef Func_Win
		return GetCurrentProcessId();
	#else
		return 1;//TODO
	#endif
}

 //!VOID WINAPI GetSystemTimeAsFileTime (LPFILETIME lpSystemTimeAsFileTime)
 VOID WINAPI sys_GetSystemTimeAsFileTime(LPFILETIME lpSystemTimeAsFileTime){
	 showfunc("GetSystemTimeAsFileTime( lpSystemTimeAsFileTime: %p )", lpSystemTimeAsFileTime);
	#ifdef Func_Win
		GetSystemTimeAsFileTime(lpSystemTimeAsFileTime);
	#else
	#endif
 }
 
//!LPTOP_LEVEL_EXCEPTION_FILTER WINAPI SetUnhandledExceptionFilter (LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI sys_SetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter){
 	showfunc("SetUnhandledExceptionFilter( lpTopLevelExceptionFilter: %p )", lpTopLevelExceptionFilter);
	#ifdef Func_Win
		return SetUnhandledExceptionFilter(lpTopLevelExceptionFilter);
	#else
		return 0;
	#endif
}
 

//!WINBOOL WINAPI TranslateMessage(CONST MSG *lpMsg)
WINBOOL WINAPI sys_TranslateMessage(CONST MSG *lpMsg){
 	showfunc_opt("TranslateMessage( lpMsg: %p )", lpMsg);
	#ifdef Func_Win
		return TranslateMessage(lpMsg);
	#else
		return true;
	#endif
}

//!LRESULT WINAPI DispatchMessageA(CONST MSG *lpMsg)
//!LRESULT WINAPI DispatchMessageW(CONST MSG *lpMsg)
WINBOOL WINAPI sys_DispatchMessageA(CONST MSG *lpMsg){
 	showfunc_opt("DispatchMessageA( lpMsg: %p )", lpMsg);
	#ifdef Func_Win
		return DispatchMessageA(lpMsg);
	#else
		return true;
	#endif
}
WINBOOL WINAPI sys_DispatchMessageW(CONST MSG *lpMsg){
 	showfunc_opt("DispatchMessageW( lpMsg: %p )", lpMsg);
	#ifdef Func_Win
		return DispatchMessageW(lpMsg);
	#else
		return 0;
	#endif
}

//!WINBOOL WINAPI PeekMessageA(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
//!WINBOOL WINAPI PeekMessageW(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg)
WINBOOL WINAPI sys_PeekMessageA(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg){
 	showfunc_opt("PeekMessageA( lpMsg: %p, hWnd: %p, wMsgFilterMin: %d, wMsgFilterMax: %d, wRemoveMsg: %d )", lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
	#ifdef Func_Win
		return PeekMessageA( lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
	#else
		return 0;
	#endif
}
WINBOOL WINAPI sys_PeekMessageW(LPMSG lpMsg,HWND hWnd,UINT wMsgFilterMin,UINT wMsgFilterMax,UINT wRemoveMsg){
 	showfunc_opt("PeekMessageW( lpMsg: %p, hWnd: %p, wMsgFilterMin: %d, wMsgFilterMax: %d, wRemoveMsg: %d )", lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg );
	#ifdef Func_Win
		return PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg);
	#else
		return 0;
	#endif
}
/*
typedef struct _RTL_CRITICAL_SECTION {
  PRTL_CRITICAL_SECTION_DEBUG DebugInfo;
  LONG LockCount;
  LONG RecursionCount;
  HANDLE OwningThread;
  HANDLE LockSemaphore;
  ULONG_PTR SpinCount;
    } RTL_CRITICAL_SECTION,*PRTL_CRITICAL_SECTION;
*/
//RTL_CRITICAL_SECTION CriticalSection = {(PRTL_CRITICAL_SECTION_DEBUG)1,1,1,(HANDLE)1,(HANDLE)1,(ULONG_PTR)1};
//!VOID WINAPI InitializeCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
int criticalSection_thread_ = 1;
VOID WINAPI sys_InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
 	showfunc_opt("InitializeCriticalSection( lpCriticalSection: %p )", lpCriticalSection);
	#ifdef Func_Win
		InitializeCriticalSection(lpCriticalSection);
	#else
	//*lpCriticalSection = (_RTL_CRITICAL_SECTION*)malloc(sizeof(_RTL_CRITICAL_SECTION));
	lpCriticalSection->DebugInfo = 0;
	lpCriticalSection->LockCount = 1;
	lpCriticalSection->RecursionCount = 1;
	lpCriticalSection->OwningThread = (HANDLE)criticalSection_thread_; //fake thread
	lpCriticalSection->LockSemaphore = (HANDLE)1;
	lpCriticalSection->SpinCount = 1;
	//criticalSection_thread_ ++; //fake to bypass mesa assert
	#endif
}

//!VOID WINAPI EnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
VOID WINAPI sys_EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
 	showfunc_opt("EnterCriticalSection( lpCriticalSection: %p )", lpCriticalSection);
	#ifdef Func_Win
		EnterCriticalSection(lpCriticalSection);
	#else
		lpCriticalSection->OwningThread = (HANDLE)criticalSection_thread_;
	//		criticalSection_thread_ ++; //fake to bypass mesa assert
		//*lpCriticalSection = CriticalSection;
	#endif
}

//!VOID WINAPI TryEnterCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
VOID WINAPI sys_TryEnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
 	showfunc_opt("TryEnterCriticalSection( lpCriticalSection: %p )", lpCriticalSection);
	#ifdef Func_Win
		TryEnterCriticalSection(lpCriticalSection);
	#else
		//*lpCriticalSection = CriticalSection;
	#endif
}

//!VOID WINAPI LeaveCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
 VOID WINAPI sys_LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
 	showfunc_opt("LeaveCriticalSection( lpCriticalSection: %p )", lpCriticalSection);
	#ifdef Func_Win
		LeaveCriticalSection(lpCriticalSection);
	#else
		lpCriticalSection->OwningThread = (HANDLE)0;
	#endif
}

//!VOID WINAPI DeleteCriticalSection (LPCRITICAL_SECTION lpCriticalSection)
 VOID WINAPI sys_DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection){
 	showfunc_opt("DeleteCriticalSection( lpCriticalSection: %p )", lpCriticalSection);
	#ifdef Func_Win
		DeleteCriticalSection(lpCriticalSection);
	#else
	#endif
}

//!UINT WINAPI SetErrosrMode (UINT uMode)
UINT WINAPI sys_SetErrorMode(UINT uMode){
 	showfunc("SetErrorMode( uMode: %p )", uMode);
	#ifdef Func_Win
		return SetErrorMode(uMode);
	#else
		return 0;
	#endif
}

//!WINBOOL WINAPI GetClientRect(HWND hWnd,LPRECT lpRect)
//struct RECT {LONG left; LONG top;LONG right;LONG bottom;}
WINBOOL WINAPI sys_GetClientRect(HWND hWnd,LPRECT lpRect){
 	showfunc("GetClientRect( hWnd: %p, lpRect: %p )", hWnd, lpRect);
	#ifdef Func_Win
		return GetClientRect(hWnd, lpRect);
	#else
		lpRect->left = 0;
		lpRect->top = 0;
		lpRect->right = 800;
		lpRect->bottom = 600;
		return true;
	#endif
}

//!WINBOOL WINAPI GetWindowRect(HWND hWnd,LPRECT lpRect)
WINBOOL WINAPI sys_GetWindowRect(HWND hWnd,LPRECT lpRect){
	showfunc("GetWindowRect( hWnd: %p, lpRect: %p )", hWnd, lpRect);
	#ifdef Func_Win
		return GetWindowRect(hWnd, lpRect);
	#else
		lpRect->left = 0;
		lpRect->top = 0;
		lpRect->right = 800;
		lpRect->bottom = 600;
		return true;
	#endif
}


//!LRESULT WINAPI DefWindowProcA (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//!LRESULT WINAPI DefWindowProcW (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
LRESULT WINAPI sys_DefWindowProcA (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	showfunc("DefWindowProcA( hWnd: %p, Msg: %p, wParam: %p, lParam: %p )", hWnd, Msg, wParam, lParam);
	#ifdef Func_Win
		return DefWindowProcA(hWnd, Msg, wParam, lParam);
	#else
		return 0;
	#endif
}
LRESULT WINAPI sys_DefWindowProcW (HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam){
	showfunc("DefWindowProcW( hWnd: %p, Msg: %p, wParam: %p, lParam: %p )", hWnd, Msg, wParam, lParam);
	#ifdef Func_Win
		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	#else
		return 0;
	#endif
}

//!VOID WINAPI Sleep (DWORD dwMilliseconds)
VOID WINAPI sys_Sleep (DWORD dwMilliseconds){
	showfunc("Sleep( dwMilliseconds: %d )", dwMilliseconds);
	#ifdef Func_Win
		Sleep(dwMilliseconds);
	#else
	#endif
}