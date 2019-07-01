
#include <iostream>
#include <stdio.h>
char * DLL_LOADED[512] = {0};
void * DLL_HANDLE[512] = {0};
int nTotalDLL = 0;

typedef int (*addNumberProc)(int, int);
typedef void (*testFunc)();
typedef int (*mainFunc)();
typedef int (*mainFunc2)(int argc, char* argv[]);
typedef void (*FUNC_Version)(int _nMajor, int _nMinor);


#include "Lib_GZ/GZ.h"


    gzUInt8* aExeFileData;
    gzUIntX nExeFileSize;

#ifdef CpcDos

 //   #include "windows.h"
  #include "win.h"




    #include "Lib_GZ/SysUtils/CpcDosHeader.h"

    gzSp<CpcdosOSx_CPintiCore> oCpc = gzSp<CpcdosOSx_CPintiCore>(new CpcdosOSx_CPintiCore);


    gzBool fExeCpcDosLoadFile(const char* _sFullPath){

        printf("\nOk fLoadFile: %s" , _sFullPath);
            nExeFileSize = 0;
            aExeFileData = 0;
            gzUInt _nExist = oCpc->File_exist((char*) _sFullPath);
            if(_nExist > 0){
                printf("\nile Exist: %s" , _sFullPath);

               nExeFileSize =  oCpc->File_size((char*) _sFullPath);

               aExeFileData = new gzUInt8[nExeFileSize + 1];
                // Recuperer TOUT le contenu
                oCpc->File_read_all((char*)_sFullPath, (char*)"RB", (char*)aExeFileData);
                aExeFileData[nExeFileSize] = 0;

            }else{
                   printf("\nFile not found: %s" , _sFullPath);
            //	Debug.fError("File not found: " + _sFullPath);
                return false;
            }
        printf("\nLaoded: %s" , _sFullPath);

        return true;
    }
#else

 //   #define UNICODE
 //   #define _UNICODE
//    #include <windows.h>
     #include "win.h"
/*
DWORD WINAPI GetModuleFileName(
  _In_opt_ HMODULE hModule,
  _Out_    char*  lpFilename,
  _In_     DWORD   nSize
);
*/
#define MAX_PATH 255
   gzBool fExeCpcDosLoadFile(const char* _sFullPath){

//char buffer[MAX_PATH];
//GetModuleFileName(0, (char*)buffer, MAX_PATH );

       //  gzUTF16 _wcFile(gzStrC(_sFullPath));
      //   FILE*  f = _wfopen((wchar_t*)(gzUInt16*)_wcFile, L"rb+");
         FILE*  f = fopen((char*)(gzUInt8*)_sFullPath, "rb+");
         unsigned char *result;

        if (f != NULL){


            WIN32_FILE_ATTRIBUTE_DATA fa;
/*
            if (!GetFileAttributesExW((LPCWSTR)(gzUInt16*)_wcFile, GetFileExInfoStandard, &fa)){
            // error handling
            }*/
/*
            if (!GetFileAttributesEx((LPCSTR)(gzUInt8*)_sFullPath, GetFileExInfoStandard, &fa)){
            // error handling
            }*/
         //  int  size = ftell(f);
  // obtain file size:
   long lSize;
  fseek (f , 0 , SEEK_END);
  lSize = ftell (f);
  rewind (f);
           // gzUIntX _nSize =   ((gzUInt64)fa.nFileSizeHigh << 32) | fa.nFileSizeLow;
            gzUIntX _nSize = lSize;
            	printf("\n !!!nEx_nSize :%ld \n ", _nSize);

            gzUInt8* _aData = new gzUInt8[_nSize];
            fread(_aData, 1, _nSize, f);


            nExeFileSize = _nSize;
            aExeFileData = _aData;


        //    _oRc->fSetDynamicMemData(_aData, _nSize); //Will be auto free

//            Lib_GZ::Sys::pDebug::fConsole(gzStrL("---File Open!-- ") + _sFullPath);
             return true;

        }else{
//
     //       Lib_GZ::Sys::pDebug::fConsole(gzStrL("Error, can't open file : ") + _sFullPath);
        }

         fclose(f);
         return false;
    }

#endif




#include "MemoryModule.h"
using namespace std;

HMEMORYMODULE fMainExeLoader(const char* _sPath = "");

#define Func(_func) (void*)(&_func)




extern "C" bool fStartExeLoader(const char* _sPath){
    fMainExeLoader(_sPath);

  //  MemoryFreeLibrary(handle);

#ifdef ImWin
system("Pause");
#endif // ImWin

    return false;
}



#ifdef ImWin
int main(int argc, char* argv[]) {
      printf("#\nMainCalled!! %d, %s", argc, argv[0]);

    fMainExeLoader(argv[1]); //argv[0] is path

    system("Pause");

//MemoryFreeLibrary(handle);

    return false;
}
#endif


/*
extern "C" void DbgCrash(gzPtr* _pStackArray, gzUInt _nStackIndex){

    int _nIndex = _nStackIndex;
    printf("\n!!!Crashed!!!!!" );
    printf("\n\n --- Call Stack -----" );

  //  printf("\n on: %d", _nStackIndex );
    while(_nIndex > 0){
        gzFuncStack* _pStack = (gzFuncStack*)_pStackArray[_nIndex];
        printf("\n name: %s", _pStack->cName );
        _nIndex--;
    }

    printf("\n ------------------ ");
    printf("\n");
    throw 42;
}
*/





#define DEREF_32( name )*(DWORD *)(name)
#define BLOCKSIZE 100

void fix_relocations(IMAGE_BASE_RELOCATION *base_reloc,DWORD dir_size,	DWORD new_imgbase, DWORD old_imgbase);



HMEMORYMODULE fMainExeLoader(const char* _sPath){





////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////






  void *data;
    long filesize;
    HMEMORYMODULE handle;
    addNumberProc addNumber;
    testFunc dTest;
     mainFunc2 dMain ;
    HMEMORYRSRC resourceInfo;
    DWORD resourceSize;
    LPVOID resourceData;
    TCHAR buffer[100];




   //  printf("MemoryLoadLibrary.\n");


    for(int index = 0; index < nTotalDLL ; index++)   {
        if(index > 512){
            break;
        }
        // Verifier si il y a quelque choose
        char* _sName = DLL_LOADED[index];
        if(_sName != NULL) {
            // Deja chargee!
            if(strcmp(_sName, _sPath) == 0)   {
                printf("\n File '%s' already loaded .!", (char*) _sPath);
                return DLL_HANDLE[index];
            }
         }

     }





    printf("\n----------- New (Recursive)  Arg: %s ------------------------\n ", _sPath );

   //  printf("\n%s\n",_sPath );

	fExeCpcDosLoadFile(_sPath);
	//fExeCpcDosLoadFile("Test.exe");
	//fExeCpcDosLoadFile("Rar.exe");

	//oMainProg.fLoadFileMem((const char*)aExeFileData, nExeFileSize);
	printf("\n nExeFileSize :%d \n ", nExeFileSize);

    filesize = nExeFileSize;
    data = aExeFileData;




    DLL_LOADED[nTotalDLL] = (char*) _sPath;
     nTotalDLL ++;

    handle = MemoryLoadLibrary(data, filesize);
    DLL_HANDLE[nTotalDLL - 1] = handle;
    if (handle == NULL)   {
        printf(("\nCan't load library from memory.\n"));
        return handle;
    }






try{

//Direct Call Temp
  // MemoryCallEntryPoint(handle);

///////////// MAIN //////////////
char* argument[] = {(char*)"aaaabbbvvv", (char*)"aaaaa"};

int _nLastChar = 0;
while( _sPath[_nLastChar] != 0){
   _nLastChar++;
}
_nLastChar--;



if( _sPath[ _nLastChar] == 'l' || _sPath[ _nLastChar] == 'L'){
   // printf("\nIm a DLL!!!");


}else{
   // printf("\nIm a EXE!!!");
 //  system("pause");


    #ifndef ImWin
        FUNC_Version dCpcVer = (FUNC_Version)MemoryGetProcAddress(handle, "cpc_Set_Version");
        if(dCpcVer != NULL){
            dCpcVer(1,0);
        }
    #endif // ImWin



    dMain = (mainFunc2)MemoryGetProcAddress(handle, "main");

    if(dMain != NULL){
        printf("Found main\n");
        dMain(1,argument);
    }else{
        printf("Not Found main\n");
        dMain = (mainFunc2)MemoryGetProcAddress(handle, "WinMain@16");
        if(dMain != NULL){
            printf("Found WinMain\n");
            dMain(1,argument);
        }else{
          printf("Not Found WinMain\n");

         printf("Call Entry:");   //Last try
          MemoryCallEntryPoint(handle);
        }
    }
    //////////////////////////////////////////////////

}




} catch (...) {

    std::cout << "catched exception" << std::endl;
}




     printf("\n-------------------- End  : %s ------------------------\n ", _sPath );

    return handle;


 //   printf(("From memory: %d\n"), addNumber(1, 2));



 /*
    resourceInfo = MemoryFindResource(handle, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
    printf(("MemoryFindResource returned 0x%p\n"), resourceInfo);

    resourceSize = MemorySizeofResource(handle, resourceInfo);
    resourceData = MemoryLoadResource(handle, resourceInfo);
    printf(("Memory resource data: %ld bytes at 0x%p\n"), resourceSize, resourceData);


    MemoryLoadString(handle, 1, buffer, sizeof(buffer));
    printf(("String1: %s\n"), buffer);

    MemoryLoadString(handle, 20, buffer, sizeof(buffer));
    printf(("String2: %s\n"), buffer);
*/






//exit:


// free(data);





/////////////////////

    printf("\n Loaded \n ");
    printf("**---- ExeLoader END -----**");
}







/*


//void fix_relocations
 // Function for rebasing a program to a new image base
 // IMAGE_BASE_RELOCATION *base_reloc - The relocation section of the image to be used
// DWORD relocation_size - Size of the relocation section
 // DWORD new_imgbase - Location in memory of the new image base
//DWORD old_imgbase - Location in memory of the old image base

void fix_relocations(IMAGE_BASE_RELOCATION *base_reloc, DWORD relocation_size, DWORD new_imgbase, DWORD old_imgbase)
{
    IMAGE_BASE_RELOCATION *cur_reloc = base_reloc, *reloc_end;

	//Calculate the difference between the old image base and the new
    DWORD delta = new_imgbase - old_imgbase;
    printf("\nDelata Realoc: %p ", delta);

	//Calculate the end of the relocation section
    reloc_end = PIMAGE_BASE_RELOCATION((char *)base_reloc + relocation_size);

	//Loop through the IMAGE_BASE_RELOCATION structures
    while (cur_reloc < reloc_end && cur_reloc->VirtualAddress) {

		//Determine the number of relocations in this IMAGE_BASE_RELOCATION structure
        DWORD count = (cur_reloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

		//Grab the current relocation entry. The plus one is to account for the loop below
        WORD *cur_entry = (WORD *)(cur_reloc + 1);

		//Grab the address in new memory where the relocation will occur
        void *page_va = (void *)((char *)new_imgbase + cur_reloc->VirtualAddress);

		//Loop through each of the relocations in the current IMAGE_BASE_RELOCATION structure
        while (count--) {

            // is valid x86 relocation?
            if (*cur_entry >> 12 == IMAGE_REL_BASED_HIGHLOW)
				//Add the delta. The 0x0fff ands out the type of relocation
                *(DWORD *)((char *)page_va + (*cur_entry & 0x0fff)) += delta;

			//Move to the next entry
            cur_entry++;

        }

        // advance to the next one
        cur_reloc = (IMAGE_BASE_RELOCATION *)((char *)cur_reloc + cur_reloc->SizeOfBlock);

    }

}

*/






