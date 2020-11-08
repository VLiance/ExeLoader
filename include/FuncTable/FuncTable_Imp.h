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
* FuncTable_Impl is the ExeLoader custom implementation. 
* Main goal is to be portable which does not relying on system functions
* 
*/

//!HMODULE LoadLibraryA(LPCSTR lpLibFileName)
inline HMODULE WINAPI imp_LoadLibraryA(LPCSTR lpLibFileName){
	showfunc("LoadLibraryA( lpLibFileName: %s )", lpLibFileName);
	#ifdef USE_Windows_LoadLibrary
		HMODULE _ret = LoadLibraryA(lpLibFileName);
		if(!_ret){My_GetLastError();}return _ret;
	#else
		return (HMODULE)AddLibrary(lpLibFileName);
	#endif
}

//!FARPROC GetProcAddress(HMODULE hModule,LPCSTR  lpProcName)
FARPROC WINAPI  imp_GetProcAddress(  HMODULE hModule, LPCSTR  lpProcName){

	char* _sDllName = (char*)"unknow";
	bool bOurLib = is_in_aLibList((HMEMORYMODULE)hModule);

	FARPROC func = 0;
	if(bOurLib){
		PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)hModule, IMAGE_DIRECTORY_ENTRY_EXPORT);
		if(directory != 0){
			if ( directory->Size == 0) {
				 _EXE_LOADER_DEBUG(0, "no export table found", "no export table found" );
			}
			PIMAGE_EXPORT_DIRECTORY exports = (PIMAGE_EXPORT_DIRECTORY) ( ((MEMORYMODULE*)hModule)->codeBase + directory->VirtualAddress);
			_sDllName =  (char*) ( ((MEMORYMODULE*)hModule)->codeBase + exports->Name);
		}
		func =  memory_module->MemoryGetProcAddress((HMEMORYMODULE)hModule, lpProcName);
	}
	if(func != 0){
		_EXE_LOADER_DEBUG(0, "GetLibAddress[%s] --> %s() ...", "GetLibAddress[%s] --> %s() ...", _sDllName, lpProcName);
		return func;
	}else{
		_EXE_LOADER_DEBUG(0, "GetTableAddress[%s] --> %s() ...", "GetTableAddress[%s] --> %s() ...", _sDllName, lpProcName);
		return MyMemoryDefaultGetProcAddress(0, lpProcName, 0); //Look in our function table
	}
}

//!void __cdecl _initterm(PVFV *,PVFV *);
typedef void (__cdecl *_PVFV)();
inline void imp_initterm(_PVFV* ppfn,_PVFV* end){
	showfunc("_initterm( ppfn: %p, end: %p )", ppfn,end);
	do {
        if (_PVFV pfn = *++ppfn){
            pfn();
        }
    } while (ppfn < end);
}

//!void __cdecl _initterm(PVFV *,PVFV *);
typedef int  (__cdecl *_PIFV)();
inline int imp_initterm_e(_PIFV* ppfn,_PIFV* end){
	showfunc("_initterm_e( ppfn: %p, end: %p )", ppfn,end);
	do {
        if (_PIFV pfn = *++ppfn){
            if (int err = pfn()) return err;
        }
    } while (ppfn < end);
    return 0;
}

//!void __cdecl _lock(int locknum)
inline void  imp_lock(int locknum){
	showfunc_opt("_lock( locknum: %d )", locknum);
	//_lock(locknum);
}

//!void __cdecl _unlock(int locknum)
inline void  imp_unlock(int locknum){
	showfunc_opt("_unlock( locknum: %d )", locknum);
	//_unlock(locknum);
}

//!int _vscprintf(const char *format,va_list argptr)
inline int imp_vscprintf(const char *format,va_list argptr){
	showfunc_opt("_vscprintf( )");
    int retval = 0; 
    va_list argcopy;
    va_copy(argcopy, argptr); 
    retval = vsnprintf(NULL, 0, format, argcopy); 
    va_end(argcopy); 
    return retval;
 }

//! void * _aligned_malloc(size_t size,size_t alignment)
inline void* imp_aligned_malloc(size_t size,size_t alignment){
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
inline void imp_aligned_free(void *memblock){
	showfunc_opt("aligned_free( memblock: %p )", memblock);
	if(memblock != 0){
		free(((void**)memblock)[-1]);
	}
}
//! void * _aligned_realloc(void *memblock,size_t size,size_t alignment);
inline void* imp_aligned_realloc(void *memblock,size_t size,size_t alignment){
	showfunc_opt("aligned_realloc( size: %d, alignment: %d )", size,alignment);
	imp_aligned_free(memblock);
	return imp_aligned_malloc(size, alignment);
}


//!char *_strdup(const char *strSource)
inline char* imp_strdup(const char *strSource){
	showfunc_opt("_strdup( strSource: %s )", strSource);
	size_t size = strlen(strSource) + 1;
	char* str = (char*)malloc(size);
	if (str) {memcpy(str, strSource, size);}
	return str;
}


//!char * strncpy( char * destination, const char * source, size_t num )
inline char* imp_strncpy( char * destination, const char * source, size_t num ){
	showfunc_opt("strncpy( destination: %p, source: %p, num: %d )", destination, source, num);
	size_t i = 0;
	while(i++ != num && (*destination++ = *source++));
	return destination;
}

//!int isspace ( int c )
inline int imp_isspace( int c ){
	showfunc_opt("isspace( c %d )", c);
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'; // || whatever other char you consider space
}

//!int isupper ( int c )
inline int  imp_isupper( int c ){
	showfunc_opt("isupper( c %d )", c);
	return (c >= 'A' && c <= 'Z');
}

//!int islower ( int c )
inline int  imp_islower( int c ){
	showfunc_opt("islower( c %d )", c);
	return (c >= 'a' && c <= 'z');
}

//!int abs(int x)
inline int imp_abs(int x){
	showfunc_opt("abs( x %d )", x);
	if(x < 0){return x*-1;}return x;
}

//==== TLS === //

void** aTlsNewMem = 0;
//!DWORD TlsAlloc()
DWORD WINAPI imp_TlsAlloc(void){
   	showfunc_opt("TlsAlloc( )", "");
	static int _nIndex = 0;
	static int _nMax = 0;
	if(_nIndex >= _nMax){ //Realloc
		int _nNewSize = _nMax*2 + 5; 
		
		 void** _aNewMem = (void**)calloc(_nNewSize, sizeof(void*));
		 memcpy(_aNewMem, aTlsNewMem, _nMax*sizeof(void*) );
		 free(aTlsNewMem);
		 aTlsNewMem = _aNewMem;
		 
		 _nMax = _nNewSize;
		_EXE_LOADER_DEBUG(3,"TlsAlloc() : [ReAlloc] size: %d\n","TlsAlloc() : [ReAlloc] size: %d\n", _nNewSize);
	}
	
	_EXE_LOADER_DEBUG(3,"TlsAlloc() : %d\n","TlsAlloc() : %d\n", _nIndex);
	_nIndex++;
	return _nIndex-1;
}

//!BOOL TlsSetValue(DWORD  dwTlsIndex,LPVOID lpTlsValue)
BOOL  WINAPI imp_TlsSetValue(DWORD dwTlsIndex, _In_opt_ LPVOID lpTlsValue){
	showfunc_opt("TlsSetValue( dwTlsIndex: %d, lpTlsValue: %p )", dwTlsIndex, lpTlsValue);
   if(lpTlsValue != 0){
		_EXE_LOADER_DEBUG(3,"TlsSetValue() : %d [0x%p] value : %d\n","TlsSetValue() : %d [0x%p] value : %d\n", dwTlsIndex, lpTlsValue, *(int*)lpTlsValue );
   }else{
		_EXE_LOADER_DEBUG(3,"TlsSetValue() : %d [0x%p]\n"		    ,"TlsSetValue() : %d [0x%p]\n",			   dwTlsIndex, lpTlsValue);
   }
   aTlsNewMem[dwTlsIndex] = lpTlsValue;
   return true;
}

//!LPVOID TlsGetValue(DWORD dwTlsIndex)
LPVOID WINAPI imp_TlsGetValue(DWORD dwTlsIndex){
	showfunc_opt("TlsGetValue( dwTlsIndex: %d )", dwTlsIndex);
   if(aTlsNewMem[dwTlsIndex] != 0){
		_EXE_LOADER_DEBUG(3,"TlsGetValue() : %d [0x%p] value : %d\n","TlsGetValue() : %d [0x%p] value : %d\n", dwTlsIndex, aTlsNewMem[dwTlsIndex], *(int*)aTlsNewMem[dwTlsIndex]);
   }else{
		_EXE_LOADER_DEBUG(3,"TlsGetValue() : %d [0x%p]\n"		     ,"TlsGetValue() : %d [0x%p]\n",		   dwTlsIndex, aTlsNewMem[dwTlsIndex]);
   }
   return aTlsNewMem[dwTlsIndex];
}

//!BOOL TlsFree(DWORD dwTlsIndex)
BOOL WINAPI imp_TlsFree(DWORD dwTlsIndex){
	showfunc_opt("TlsFree( dwTlsIndex: %d )", dwTlsIndex);
	_EXE_LOADER_DEBUG(3,"TlsFree() : %d [0x%p] value : %d\n","TlsFree() : %d [0x%p] value: %d\n", dwTlsIndex, aTlsNewMem[dwTlsIndex], *(int*)aTlsNewMem[dwTlsIndex]);
	aTlsNewMem[dwTlsIndex] = 0;
	return true;
}

//============ //

