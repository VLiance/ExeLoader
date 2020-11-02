#ifndef EXELOADER_Config_H
#define EXELOADER_Config_H

//Use the Windows Stanard LoadLibraryA or undef to use the ExeLaoder AddLibray similar to loading .exe from memory
#define USE_Windows_LoadLibrary



extern void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...);
//#define _EXE_LOADER_DEBUG_S(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg);
#define _EXE_LOADER_DEBUG_(_msg, ...)  _EXE_LOADER_DEBUG(0, _msg, _msg,  __VA_ARGS__);

#endif
