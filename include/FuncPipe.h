/* @autor Maeiky
*
* FuncPipe will remap a system function to a same signature custom function to see what is called,
* and then we choose what to do with it.
*
* Warning: Windows function are __stdcall instead of __cdecl. 
* __stdcall remapped function must have the EXACT same paramters and must be specified as __stdcall
*  If not, your app will crash when the fucntion return. 
*/

#include "win.h"

#define showfunc(name) printf("\nCall:"#name);
#if ImWin
#define Func_Win
#endif


//ATOM RegisterClassW(const WNDCLASSW *lpWndClass);
inline unsigned short __stdcall  pipe_RegisterClassW(void* value){
	showfunc("RegisterClassW");
	#ifdef Func_Win
	return RegisterClassW((WNDCLASSW*)value);
	#else
	return 0;
	#endif
}



