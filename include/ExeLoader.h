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
 *
 */
#ifndef EXELOADER_Exeloader_H
#define EXELOADER_Exeloader_H

#include "_Config.h"

#include <cstdlib> 
#include <cstdio> 
#include <cstring>
#include <cstdarg>
#include <csignal>
#include <memory>


#ifdef CpcDos
	#include "Lib_GZ/SysUtils/CpcDosHeader.h"
#endif

extern void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...);
//#define _EXE_LOADER_DEBUG_S(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg);
#define _EXE_LOADER_DEBUG_(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg,  __VA_ARGS__);



#include "FuncPrototype/win.h"
#include "MemoryModule.h"


#ifndef STDCALL
	#define STDCALL __stdcall
#endif

#ifndef FuncTableStructure
	#define FuncTableStructure
	typedef void* (*FUNC_)();
	typedef struct {
		const char* sFuncName;
		FUNC_* dFunc;
	} FuncTable;
	#undef PFUNC_
	#define PFUNC_ FUNC_*
#endif

#define Func(_func) (void*)(&_func)
#define DEREF_32(name) *(DWORD *)(name)
#define BLOCKSIZE 100


extern HMEMORYMODULE AddLibrary(const char* _sPath); //ExeLoader.cpp
extern DWORD My_GetLastError();


extern "C" bool fStartExeLoader(const char* Source_File);



/// Main Func Ptr ///
typedef int (*addNumberProc)(int, int);
typedef void (*testFunc)();
typedef int (*mainFunc)();
typedef int (*mainFunc2)(int argc, char* argv[]);
typedef void (*FUNC_Version)(int _nMajor, int _nMinor);

/// Generic Func Ptr ///
typedef bool (*funcPtr_bool)(void*);
typedef int  (*funcPtr_int)(void*);
typedef int  (*funcPtrPtr_int)(void*,void*);
typedef bool (*funcPtrIntPtr_bool)(void*,int,void*);
typedef int  (*funcPtrIntIntPtr_int)(void*,int,int,void*);

/// Log ///
#define showfunc_unimplt(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de Fonction non implémenté: " name, "\n-->Call not implemented func: " name , __VA_ARGS__);
#define showfunc(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Appel de: " name, "\n-->Call: " name , __VA_ARGS__);
#define showfunc_ret(name, ...) _EXE_LOADER_DEBUG(0, "\n-->Retour: " name, "\n-->Return: " name , __VA_ARGS__);


#ifdef Show_AllFuncTable
#define showfunc_opt showfunc
#else
#define showfunc_opt
#endif

#ifndef Show_FuncTable
#undef showfunc
#define showfunc
#undef showfunc_ret
#define showfunc_ret
#undef showfunc_unimplt
#define showfunc_unimplt
#undef showfunc_opt
#define showfunc_opt
#endif


#endif //EXELOADER_Exeloader_H