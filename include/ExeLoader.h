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
 */

#ifndef EXELOADER_H
#define EXELOADER_H 1

#define Func(_func) (void*)(&_func)
#define DEREF_32(name) *(DWORD *)(name)
#define BLOCKSIZE 100

typedef int (*addNumberProc)(int, int);
typedef void (*testFunc)();
typedef int (*mainFunc)();
typedef int (*mainFunc2)(int argc, char* argv[]);
typedef void (*FUNC_Version)(int _nMajor, int _nMinor);

extern "C" bool fStartExeLoader(const char* Source_File);

#endif /* !EXELOADER_H */