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

//!HDC GetDC(HWND hWnd)
int aDC[10] = {};
int aDC_curr = 0;
inline HDC WINAPI sys_GetDC(HWND hWnd){
	showfunc("GetDC( lpModuleName: %p)", hWnd); 
	#ifdef Func_Win
		return GetDC(hWnd);
	#else
		aDC_curr++;
		return (HDC)aDC_curr;
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
