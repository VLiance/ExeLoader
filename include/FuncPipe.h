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

extern HMEMORYMODULE AddLibrary(const char* _sPath); //ExeLoader.cpp
extern DWORD My_GetLastError();

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



/*
inline int wglChoosePixelFormat(HDC hdc, CONST PIXELFORMATDESCRIPTOR *ppfd ){
printf("\n wglChoosePixelFormat!\n");
   if (ppfd->nSize != sizeof( PIXELFORMATDESCRIPTOR ) || ppfd->nVersion != 1)
      return 0;
   if (ppfd->iPixelType != PFD_TYPE_RGBA)
      return 0;
   if (!(ppfd->dwFlags & PFD_DRAW_TO_WINDOW))
      return 0;
   if (!(ppfd->dwFlags & PFD_SUPPORT_OPENGL))
      return 0;
   if (ppfd->dwFlags & PFD_DRAW_TO_BITMAP)
      return 0;
   if (!(ppfd->dwFlags & PFD_STEREO_DONTCARE) && (ppfd->dwFlags & PFD_STEREO))
      return 0;

printf("\n end!\n");
   return stw_pixelformat_choose( hdc, ppfd );
   
}

_sapp.wgl.ChoosePixelFormat = (PFN_wglChoosePixelFormat) GetProcAddress(_sapp.wgl.opengl32, "wglChoosePixelFormat");
SOKOL_ASSERT(_sapp.wgl.ChoosePixelFormat);
*/
/*
O> GetProcAddress[unknow] --> wglDeleteContext() ...
O> Warning, :  ---------   wglDeleteContext 
O> GetProcAddress[unknow] --> wglGetProcAddress() ...
O> Warning, :  ---------   wglGetProcAddress 
O> GetProcAddress[unknow] --> wglGetCurrentDC() ...
O> Warning, :  ---------   wglGetCurrentDC 
O> GetProcAddress[unknow] --> wglMakeCurrent() ...
O> Warning, :  ---------   wglMakeCurrent 
O> GetProcAddress[unknow] --> wglChoosePixelFormat() ...
O> Warning, :  ---------   wglChoosePixelFormat 
*/



extern funcPtrPtr_int _dFunc_wglChoosePixelFormat;
//!int ChoosePixelFormat( HDC hdc, const PIXELFORMATDESCRIPTOR *ppfd)
inline int WINAPI pipe_ChoosePixelFormat(void* hdc, void* ppfd){
	showfunc("ChoosePixelFormat( hdc: %p, ppfd: %p )", hdc, ppfd);
	if(_dFunc_wglChoosePixelFormat != 0){
//		return _dFunc_wglChoosePixelFormat(hdc, ppfd);
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
	return (char*)malloc(strlen(strSource) + 1);
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





