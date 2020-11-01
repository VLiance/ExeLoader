/* @autor Maeiky
*
* FuncPipe will remap a system function to a same signature custom function to see what is called,
* and then we choose what to do with it.
*
* Warning: Windows WINAPI function are __stdcall instead of __cdecl. 
* __stdcall remapped function must have the EXACT same paramters and must be specified as __stdcall
*  If not, your app will crash when the fucntion return. 
*/

#include "win.h"

extern HMEMORYMODULE AddLibray(const char* _sPath); //ExeLoader.cpp
extern DWORD My_GetLastError();
extern void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...);
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
	return (HMODULE)AddLibray(lpLibFileName);
	/*
	HMODULE _ret = LoadLibraryA(lpLibFileName);
	if(!_ret){My_GetLastError();}return _ret;
	*/
	#else
	return 0;
	#endif
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





