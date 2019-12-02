#ifndef EXELOADER_H
# define EXELOADER_H 1

#define Func(_func) (void*)(&_func)
#define DEREF_32(name) *(DWORD *)(name)
#define BLOCKSIZE 100

typedef int (*addNumberProc)(int, int);
typedef void (*testFunc)();
typedef int (*mainFunc)();
typedef int (*mainFunc2)(int argc, char* argv[]);
typedef void (*FUNC_Version)(int _nMajor, int _nMinor);

extern "C" bool fStartExeLoader(const char* Source_File);
HMEMORYMODULE fMainExeLoader(const char* _sPath = "");
void fix_relocations(IMAGE_BASE_RELOCATION *base_reloc, DWORD dir_size, DWORD new_imgbase, DWORD old_imgbase);

#endif /* !EXELOADER_H */