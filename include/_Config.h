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
* Here you can change the behavior of ExeLoader 
*
*/

#ifndef EXELOADER_Config_H
#define EXELOADER_Config_H

#if ImWin
#define Func_Win
#endif
#define Func_Win //Temp

////////////////////////////////////////////////////
/////////////  CUSTOM SETTINGS  ////////////////////
////////////////////////////////////////////////////

//Use the Windows Stanard LoadLibraryA or undef to use the ExeLaoder AddLibrary similar to loading .exe from memory
//#define USE_Windows_LoadLibrary

//Some function are irrelevant and annoying, but sometime we need it for debugging purpose
//#define Pipe_Show_AllFunc




////////////////////////////////////////////////////
////////////////////////////////////////////////////
////////////////////////////////////////////////////

#ifndef Func_Win
#undef USE_Windows_LoadLibrary
#endif

extern void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...);
//#define _EXE_LOADER_DEBUG_S(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg);
#define _EXE_LOADER_DEBUG_(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg,  __VA_ARGS__);


typedef bool (*funcPtr_bool)(void*);
typedef int  (*funcPtr_int)(void*);
typedef int  (*funcPtrPtr_int)(void*,void*);
typedef bool (*funcPtrIntPtr_bool)(void*,int,void*);
typedef int  (*funcPtrIntIntPtr_int)(void*,int,int,void*);


#endif
