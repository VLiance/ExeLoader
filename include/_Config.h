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
