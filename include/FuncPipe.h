/* @autor Maeiky
*
* FuncPipe will remap a system function to a same signature custom function to see what is called,
* and then we choose what to do with it.
*
* Warning: Windows WINAPI function are __stdcall instead of __cdecl. 
* __stdcall remapped function must have the EXACT same paramters and must be specified as __stdcall
*  If not, your app will crash when the fucntion return. 
*/

#include "_Config.h"
#include "win.h"

extern HMEMORYMODULE AddLibray(const char* _sPath); //ExeLoader.cpp
extern DWORD My_GetLastError();

#define showfunc_unimplt(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de Fonction non implémenté: " name, "\n-->Call not implemented func: " name , __VA_ARGS__);
#define showfunc(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de: " name, "\n-->Call: " name , __VA_ARGS__);
#define showfunc_ret(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Retour: " name, "\n-->Return: " name , __VA_ARGS__);
#if ImWin
#define Func_Win
#endif
#define Func_Win

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
	#ifdef Func_Win
		#ifdef USE_Windows_LoadLibrary
			HMODULE _ret = LoadLibraryA(lpLibFileName);
			if(!_ret){My_GetLastError();}return _ret;
		#endif
	#endif
	return (HMODULE)AddLibray(lpLibFileName);
}

//!BOOL SetPixelFormat(HDC hdc, int format, const PIXELFORMATDESCRIPTOR *ppfd)
inline BOOL WINAPI pipe_SetPixelFormat(void* hdc, int format, void* ppfd){
	showfunc("SetPixelFormat( hdc: %p, format: %d, ppfd: %p )", hdc, format, ppfd);
	#ifdef Func_Win
	return SetPixelFormat((HDC)hdc, format, (PIXELFORMATDESCRIPTOR*)ppfd);
	#else
	return true;
	#endif
}

//!int ChoosePixelFormat( HDC hdc, const PIXELFORMATDESCRIPTOR *ppfd)
inline int WINAPI pipe_ChoosePixelFormat(void* hdc, void* ppfd){
	showfunc("ChoosePixelFormat( hdc: %p, ppfd: %p )", hdc, ppfd);
	#ifdef Func_Win
	int _ret = ChoosePixelFormat((HDC)hdc, (PIXELFORMATDESCRIPTOR*)ppfd);
	showfunc_ret("ChoosePixelFormat[ int: %d ]", _ret);return _ret;
	#else
	return 0;
	#endif
}


//!void __cdecl _lock(int locknum)
inline void  pipe_lock(int locknum){
	showfunc("_lock( locknum: %d )", locknum);
	#ifdef Func_Win
	//_lock(locknum);
	#else
	#endif
}

//!void __cdecl _unlock(int locknum)
inline void  pipe_unlock(int locknum){
	showfunc("_unlock( locknum: %d )", locknum);
	#ifdef Func_Win
	//_unlock(locknum);
	#else
	#endif
}

//!void __cdecl _initterm(PVFV *,PVFV *);
inline void pipe_initterm(void* a,void* b){
	showfunc("_initterm( a: %p, b: %p )", a,b);
	#ifdef Func_Win
	//_initterm(a, b);
	#else
	#endif
}

//!HANDLE My_CreateSemaphore(_In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,_In_ LONG lInitialCount, _In_ LONG lMaximumCount, _In_opt_ LPCTSTR lpName)
HANDLE  WINAPI  pipe_CreateSemaphore( //Must have __stdcall
 _In_opt_ LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
  _In_     LONG                  lInitialCount,
  _In_     LONG                  lMaximumCount,
  _In_opt_ LPCTSTR               lpName
)
{
	showfunc_unimplt("_CreateSemaphore(  )","");
	return 0;
}




/*
//!BOOL wglMakeCurrent(HDC,HGLRC)
BOOL pipe_wglMakeCurrent(void* hdc, HGLRC hglrc) {
	showfunc("ChoosePixelFormat( hdc: %p, hglrc: %p )", hdc, hglrc);
	#ifdef Func_Win
	return wglMakeCurrent((HDC)hdc, hglrc);
	#else
	return true;
	#endif
}

*/





