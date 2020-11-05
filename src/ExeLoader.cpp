// Mickael BANVILLE & Sebastien FAVIER
// ExeLoader pour Cpcdos
// Update v1 13/01/2016
// Update v2 19 AVR 2019
// Update v3 10 OCT 2019
// Update v4 30 JAN 2020
// Update v5 12 MAR 2020  
// Update v5.1 20 MAR 2020  

     
#include <cstdlib> 
#include <cstdio> 
#include <cstring>
#include <cstdarg>  // Pour les arguments de fdebug_log
#include <csignal>

#include <memory>
#include <iostream>

#include "win.h"
#include "Lib_GZ/GZ.h"
#ifdef CpcDos
	#include "Lib_GZ/SysUtils/CpcDosHeader.h"
#endif


#include "MemoryModule.h"
#include "ExeLoader.h"

 ManagedAlloc instance_AllocManager = {1024};

void signalHandler(int signum) {
	printf("\n Interrupt signal received: ");
	// cleanup and close up stuff here
	switch (signum) {
	case SIGTERM:
		printf("SIGTERM, termination request, sent to the program ");
		break;
	case SIGSEGV:
		printf("SIGSEGV, invalid memory access (segmentation fault) ");
		break;
	case SIGINT:
		printf("SIGINT, external interrupt, usually initiated by the user ");
		break;
	case SIGILL:
		printf("SIGILL, invalid program image, such as invalid instruction ");
		break;
	case SIGABRT:
		printf("SIGABRT, abnormal termination condition, as is e.g. initiated by std::abort()");
		break;
	case SIGFPE:
		printf("SIGFPE, erroneous arithmetic operation such as divide by zero");
		break;
	default:
		printf("UNKNOW");
		break;
	}
	exit(signum);
}

/*
void segfault_sigaction(int signal, void* si, void *arg)
{
   printf("Caught segfault at address %p\n", si->si_addr);
	exit(0);
}*/

void registerSignal() {
/* //No sigaction on Windows
int *foo = NULL;
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = segfault_sigaction;
	sa.sa_flags   = SA_SIGINFO;
	sigaction(SIGSEGV, NULL, NULL);
*/
	for (int i = 1; i < 32; i++) {
		signal(i, signalHandler);
	}
/*
signal(SIGTERM, signalHandler);  //termination request, sent to the program
signal(SIGSEGV, signalHandler);  //invalid memory access (segmentation fault)
signal(SIGINT, signalHandler);  //external interrupt, usually initiated by the user
signal(SIGILL, signalHandler);  //invalid program image, such as invalid instruction
signal(SIGABRT, signalHandler);  //abnormal termination condition, as is e.g. initiated by std::abort()
signal(10, signalHandler); //SIGBUS
*/
}

char * DLL_LOADED[512] = {0};
void * DLL_HANDLE[512] = {0};
int nTotalDLL = 0;

char* aExeFileData;
long nExeFileSize;

#ifdef CpcDos /* It's Cpcdos */

	gzSp<CpcdosOSx_CPintiCore> oCpc = gzSp<CpcdosOSx_CPintiCore>(new CpcdosOSx_CPintiCore);
	
	void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...)
	{
		// Cette fonction permet d'utiliser le simuler un sprintf()
		va_list arg;
		char BUFFER[1024] = {0};
		
		// Faire une condition si l'instance est en Francais ou non
		
		va_start (arg, format_EN);
			vsprintf (BUFFER, format_FR, arg);
		va_end (arg);
		
		oCpc->debug_log_plus((const char*) BUFFER, 1, 1, alert, 0, 0, 0,  1100, NULL); 	
		
		BUFFER[0] = '\0';
	}
	
		// TODO: Faire une condition si l'instance est en Francais ou non

	gzBool fExeCpcDosLoadFile(const char* _sFullPath)
	{

		nExeFileSize = 0;
		aExeFileData = 0;
		
		_EXE_LOADER_DEBUG(0, "\nExeLoader: Test de l'existence de '%s'\n", "\nExeLoader: Test existence  '%s'\n", _sFullPath);
		gzUInt _nExist = oCpc->File_exist((char*) _sFullPath);
		if(_nExist > 0)
		{
			// Recuperer la taille du fichier
			nExeFileSize =  oCpc->File_size((char*) _sFullPath);
			
			// Afficher sur la console Cpcdos
			_EXE_LOADER_DEBUG(0, "ExeLoader: Lecture de %s (%d octets)...", "ExeLoader: Reading %s (%d bytes)...", _sFullPath, nExeFileSize);
			
			// Recuperer TOUT le contenu
			aExeFileData = (char*) instance_AllocManager.ManagedCalloc(nExeFileSize + 1, sizeof(char));
			// aExeFileData = (char*) calloc(nExeFileSize + 1, sizeof(char));
			
			FILE *fptr;

			if ((fptr = fopen(_sFullPath,"rb")) == NULL){
				_EXE_LOADER_DEBUG(4, "\nExeLoader: Erreur d'ouverture du fichier %s.\n", "\nExeLoader: Error openning file %s.\n", _sFullPath);
				return false;
			}
			
			
			fread(aExeFileData, nExeFileSize, 1, fptr);
			//fclose(fptr);
			
			// oCpc->File_read_all((char*)_sFullPath, (char*)"RB", (char*)aExeFileData);
			
			// Caractere de terminaison
			aExeFileData[nExeFileSize] = '\0';
		} else {
			_EXE_LOADER_DEBUG(4, "\nExeLoader: Fichier non disponible %s.\n", "\nExeLoader: File not avaiable %s.\n", _sFullPath);
			return false;
		}

		_EXE_LOADER_DEBUG(5, "ExeLoader: %s charge!", "ExeLoader:  %s loaded!\n" , _sFullPath);

		return true;
	}

#else /* !!! Not Cpcdos !!! */

	//   #define UNICODE
	//   #define _UNICODE
	//    #include <windows.h>
	
	void _EXE_LOADER_DEBUG(int alert, const char* format_FR, const char* format_EN, ...)
	{
		// Cette fonction permet d'utiliser le simuler un sprintf()
		va_list arg;
		char BUFFER[1024] = {0};
		
		// Faire une condition si l'instance est en Francais ou non
		va_start (arg, format_EN);
			vsprintf (BUFFER, format_EN, arg);
		va_end (arg);
		
		printf("%s\n" , BUFFER); 

	}

	
	/*
	DWORD WINAPI GetModuleFileName(
	_In_opt_ HMODULE hModule,
	_Out_    char*  lpFilename,
	_In_     DWORD   nSize
	);
	*/
	//#define MAX_PATH 255
	gzBool fExeCpcDosLoadFile(const char* _sFullPath)
	{
	
		if(_sFullPath == 0){
			printf("\n Error: No file to load. \n ");
			return false;
		}
	
		//char buffer[MAX_PATH];
		//GetModuleFileName(0, (char*)buffer, MAX_PATH );

		//  gzUTF16 _wcFile(gzStrC(_sFullPath));
		//   FILE*  f = _wfopen((wchar_t*)(gzUInt16*)_wcFile, L"rb+");
		FILE*  f = fopen((char*)(gzUInt8*)_sFullPath, "rb+");
		unsigned char *result;

		if (f != NULL)
		{
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

			gzUInt8* _aData = new gzUInt8[_nSize];
			fread(_aData, 1, _nSize, f);

			nExeFileSize = _nSize;
			aExeFileData = (char*)_aData;
			fclose(f);
			// _oRc->fSetDynamicMemData(_aData, _nSize); //Will be auto free
			// Lib_GZ::Sys::pDebug::fConsole(gzStrL("---File Open!-- ") + _sFullPath);
			return true;
		}else{
			_EXE_LOADER_DEBUG(6, " * Impossible d'ouvrir le fichier: %s", "Error, can't open file: %s", _sFullPath );
			// Lib_GZ::Sys::pDebug::fConsole(gzStrL("Error, can't open file : ") + _sFullPath);
		}
		
		return false;
	}
#endif /* !!! No Cpcdos !!! */

mainFunc2 fFindMainFunction(MemoryModule* _oMem, HMEMORYMODULE handle) {
	mainFunc2 dMain ;
	
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'main_entry()'... ", "research 'main_entry()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "main_entry");
	if(dMain){return dMain;}
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'cpc_main()'... ", "research 'cpc_main()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "cpc_main");
	if(dMain){return dMain;}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Disable standard main because of the CRT mess and Static Initialisation (Application must be compiled without this feature) ///
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ive reactivated for tests
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'main()'... ", "research 'main()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "main");
	if(dMain){return dMain;}
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'WinMain@16()'... ", "research 'WinMain@16()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "WinMain@16");
	if(dMain){return dMain;}
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'WinMainCRTStartup()'... ", "research 'WinMainCRTStartup()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "WinMainCRTStartup");
	if(dMain){return dMain;}
	
	
	
	// S'il s'agit d'une version Windows 16 bits
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'MAIN_LOOP_WINDOWS()'... ", "research 'MAIN_LOOP_WINDOWS()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "MAIN_LOOP_WINDOWS");
	if(dMain){return dMain;}
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'MAIN_LOOP()'... ", "research 'MAIN_LOOP()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "MAIN_LOOP");
	if(dMain){return dMain;}

	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'QBMAIN_WINDOWS()'... ", "research 'QBMAIN_WINDOWS()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "QBMAIN_WINDOWS");
	if(dMain){return dMain;}
	
	_EXE_LOADER_DEBUG(6, " * Recherche du point d'entre 'QBMAIN()'... ", "research 'QBMAIN()' entry point... ");
	dMain = (mainFunc2)_oMem->MemoryGetProcAddress(handle, "QBMAIN");
	if(dMain){return dMain;}
	
	
	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////

// class hndl
// {
	// int idx;
	
	// public:
	// HMEMORYMODULE handle;
	
	
// };

//#include <process.h>

//GDB will automaticly break here (with Cwc compiler)
extern "C" void GDB_Func_Break(){} //raise(SIGTRAP)? void __debugbreak();?
extern "C" void GDB_Func_ExecuteCmds(){} 


/*
bool GDB_Send_RunCmd_AndWait(int _timeout = 1000){ //1000 = 1 seconde

	printf("Cmd(add)[GDB]:Continue\n");
	printf("Cmd(run)[GDB]:(waiting)\n");
	
	#ifdef ImWin
	while(_timeout>0){
		Sleep(1);
		_timeout--;
		if(_timeout == 5){
			return true;
		}
	}
	#endif
	return false;
}*/

void GDB_Send_AddSymbolFile(char* _path, void* _text_adress, int _timeout = 1000){
//add-symbol-file "E:/.../app.exe" 0xXXXXX
	//fflush(stdout);fflush(stderr);//To be sure we receive the cmd
	fprintf(stderr, "Cmd[GDB]:add-symbol-file \"%s\" 0x%p\n", _path, _text_adress);
	fflush(stdout);fflush(stderr);//To be sure we receive the cmd
	GDB_Func_ExecuteCmds();
}


MemoryModule* memory_module = 0;
bool fMainExeLoader(const char* _sPath){

	if(strlen(_sPath) <= 0){
		_EXE_LOADER_DEBUG(5, "Aucun fichier spécifié", "No Input files");
		return false;
	}else{
		_EXE_LOADER_DEBUG(5, "Fichier: %s", "File: %s", _sPath);
	}

//raise(SIGTRAP);


	//setbuf(stdout, NULL);//Just to test
	#ifdef ImWin
		 setbuf(stdout, NULL);//Required to see every printf
		 setbuf(stderr, NULL);//Required to see every printf
		 registerSignal();
	#endif
	
	// Instancier MemoryModule
	std::unique_ptr<MemoryModule> memory_module_instance(new MemoryModule());
	memory_module = memory_module_instance.get();
	

	void *data;
	long filesize;
//	std::unique_ptr<HMEMORYMODULE> handle_ptr{new HMEMORYMODULE};
	
	//HMEMORYMODULE* handle = (HMEMORYMODULE*) handle_ptr.get();
	mainFunc2 dMain ;
 

	for(int index = 0; index < nTotalDLL ; index++)  
	{
		if(index > 512)
			break;
		
		// Verifier si il y a quelque choose
		char* _sName = DLL_LOADED[index];
		if(_sName != NULL)
		{
			// Deja chargee!
			if(strcmp(_sName, _sPath) == 0)
			{
				_EXE_LOADER_DEBUG(5, "\nFichier %s deja charge", "\nFile %s already loaded", (char*) _sPath);
				// return DLL_HANDLE[index];
				return false;
			}
		}
 
	}

	// Charger le fichier en memoire
	if(!fExeCpcDosLoadFile(_sPath)) return false;
	// Recuperer la taille
	filesize = nExeFileSize;
	data = aExeFileData;


	// Charger le fichier
	HMEMORYMODULE* handle = (HMEMORYMODULE*) memory_module_instance->MemoryLoadLibrary(data, filesize);
	DLL_HANDLE[nTotalDLL - 1] = handle;
	 
	// Oups probleme
	if (handle == NULL) {
		_EXE_LOADER_DEBUG(4, "\nImpossible de charger la librairie depuis la memoire\n", "\nUnable to to load library from the memory\n");
		return false;
	}
	
	if(((MEMORYMODULE*)handle)->section_text != 0){
		GDB_Send_AddSymbolFile((char*)_sPath, ((MEMORYMODULE*)handle)->section_text );
	}
	

	#ifdef __cpp_exceptions
	try{
	#endif
		///////////// MAIN //////////////
		char* argument[] = {(char*)"aaaabbbvvv", (char*)"aaaaa"};

		int _nLastChar = 0;
		while( _sPath[_nLastChar] != 0)
		{
			_nLastChar++;
		}
		_nLastChar--;


		// Verifier si le fichier est un DLL ou un EXE
		if( _sPath[ _nLastChar] == 'l' || _sPath[ _nLastChar] == 'L')
		{ /*** DLL ***/

			// Ajouter dans le tableau des DLL deja charge
			DLL_LOADED[nTotalDLL] = (char*) _sPath;
			nTotalDLL ++;
			
		}
		else
		{ /*** EXE ***/

			#ifndef ImWin /* No Cpcdos */
			FUNC_Version dCpcVer = (FUNC_Version)memory_module_instance->MemoryGetProcAddress(handle, "cpc_Set_Version");
			
			if(dCpcVer != NULL)
				dCpcVer(1,0);

			#endif // ImWin
 
 
 
 //GDB_Func_Break();
 
			int boucle = 0;
			

		//	_EXE_LOADER_DEBUG(5, "Lancement[%p]: %s", "Run[%p]: %s", ((PMEMORYMODULE*)handle)->codeBase, _sPath);
			dMain = fFindMainFunction(memory_module_instance.get(), handle);
			
			 
			// Le point d'entre a ete trouve, maintenant on l'execute
			if(dMain != NULL)
			{
				_EXE_LOADER_DEBUG(5, " Execution du point d'entre...\n", "Point entry execution...");
				dMain(1,argument);
				_EXE_LOADER_DEBUG(5, " Execution du point d'entre TERMINE!\n", "Point entry execution...FINISHED!");
			}
			else
			{ 
				_EXE_LOADER_DEBUG(6, "Dernier essai, call Entry\n", "Last try, call Entry.");
				memory_module_instance->MemoryCallEntryPoint(handle);
			}
		} /*** EXE ***/

	#ifdef __cpp_exceptions
	} catch (...) 
	{
		_EXE_LOADER_DEBUG(4, "Exception catched !\n", "Catched exception !");
	}
	#endif
	
	 
	memory_module_instance->MemoryFreeLibrary(handle);
	memory_module_instance->Fin_instance(); 
	// Delete instance
	// delete memory_module_instance.get();
	  
	return true;

}

funcPtr_bool _dFunc_wglSwapBuffers = 0;
funcPtrPtr_int _dFunc_wglChoosePixelFormat = 0;
funcPtrIntPtr_bool _dFunc_wglSetPixelFormat = 0;
funcPtr_int _dFunc_wglGetPixelFormat = 0;
funcPtrIntIntPtr_int _dFunc_wglDescribePixelFormat = 0;


void GetLibraryExportTable(PMEMORYMODULE module){

	unsigned char *codeBase = ((PMEMORYMODULE)module)->codeBase;
	DWORD idx = 0;
	PIMAGE_EXPORT_DIRECTORY exports;
	PIMAGE_DATA_DIRECTORY directory = GET_HEADER_DICTIONARY((PMEMORYMODULE)module, IMAGE_DIRECTORY_ENTRY_EXPORT);
	if (directory->Size == 0) {
		// no export table found
		SetLastError(ERROR_PROC_NOT_FOUND);
		return;
	}

	exports = (PIMAGE_EXPORT_DIRECTORY) (codeBase + directory->VirtualAddress);
	if (exports->NumberOfNames == 0 || exports->NumberOfFunctions == 0) {
		// DLL doesn't export anything
		SetLastError(ERROR_PROC_NOT_FOUND);
		return;
	}

	// search function name in list of exported names
	DWORD i;
	DWORD *nameRef = (DWORD *) (codeBase + exports->AddressOfNames);
	WORD *ordinal = (WORD *) (codeBase + exports->AddressOfNameOrdinals);
	BOOL found = FALSE;
	for (i=0; i < exports->NumberOfNames; i++, nameRef++, ordinal++) {
		LPCSTR funcName = (const char *) (codeBase + (*nameRef));
		_EXE_LOADER_DEBUG(0, "/===: %s", 
							 "/===: %s", funcName);
		
		//// Special Function ///
		if (strcmp("wglChoosePixelFormat", funcName) == 0) {
			FARPROC _dFunc = (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + ((*ordinal)*4))));
			_EXE_LOADER_DEBUG_("FOUND Special function:%s",funcName);
			_dFunc_wglChoosePixelFormat = (funcPtrPtr_int)_dFunc;
		}
		if (strcmp("wglSetPixelFormat", funcName) == 0) {
			FARPROC _dFunc = (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + ((*ordinal)*4))));
			_EXE_LOADER_DEBUG_("FOUND Special function:%s",funcName);
			_dFunc_wglSetPixelFormat = (funcPtrIntPtr_bool)_dFunc;
		}
		if (strcmp("wglGetPixelFormat", funcName) == 0) {
			FARPROC _dFunc = (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + ((*ordinal)*4))));
			_EXE_LOADER_DEBUG_("FOUND Special function:%s",funcName);
			_dFunc_wglGetPixelFormat = (funcPtr_int)_dFunc;
		}
		if (strcmp("wglDescribePixelFormat", funcName) == 0) {
			FARPROC _dFunc = (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + ((*ordinal)*4))));
			_EXE_LOADER_DEBUG_("FOUND Special function:%s",funcName);
			_dFunc_wglDescribePixelFormat = (funcPtrIntIntPtr_int)_dFunc;
		}
		if (strcmp("wglSwapBuffers", funcName) == 0) {
			FARPROC _dFunc = (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + ((*ordinal)*4))));
			_EXE_LOADER_DEBUG_("FOUND Special function:%s",funcName);
			_dFunc_wglSwapBuffers = (funcPtr_bool)_dFunc;
		}
		
		//////////////////////////
	}

/*
	if (idx > exports->NumberOfFunctions) {
		// name <-> ordinal number don't match
		SetLastError(ERROR_PROC_NOT_FOUND);
		return;
	}
*/
	// AddressOfFunctions contains the RVAs to the "real" functions
	//return (FARPROC)(LPVOID)(codeBase + (*(DWORD *) (codeBase + exports->AddressOfFunctions + (idx*4))));
	
}


//Max laoded dll: 50
HMEMORYMODULE  aLibList[50] = {};
int aLibList_size = 0;
bool aLibList_add(HMEMORYMODULE _handle){
	if(aLibList_size < 50){
		aLibList[aLibList_size] = _handle;
		aLibList_size++;
		return true;
	}
	return false;
}
bool is_in_aLibList(HMEMORYMODULE _handle){
	for(int i =0; i < aLibList_size; i++){
		if(_handle == aLibList[i]){
			return true;
		}
	}
	return false;
}


HMEMORYMODULE AddLibrary(const char* _sPath) {
	_EXE_LOADER_DEBUG_("///===============================================================================================================///","");
	_EXE_LOADER_DEBUG(0, "///========= AddLibrary: %s", 
						 "///========= AddLibrary: %s", _sPath);
	_EXE_LOADER_DEBUG_("///===============================================================================================================///","");
	// Charger le fichier en memoire
	if(!fExeCpcDosLoadFile(_sPath)) return 0;
	long filesize = nExeFileSize;
	void* data = aExeFileData;
	
	HMEMORYMODULE handle = (HMEMORYMODULE)memory_module->MemoryLoadLibrary(data, filesize);
	aLibList_add(handle);

	
	_EXE_LOADER_DEBUG_("///===============================================================================================================///","");
	if(handle == 0){
		_EXE_LOADER_DEBUG(0, "///====== ERREUR: Chargement de la DLL Impossible %s", 
						 "///====== ERROR: Unable to load DLL: %s",  _sPath);
		_EXE_LOADER_DEBUG_("///===============================================================================================================///","");
		return 0;
	}else{
		_EXE_LOADER_DEBUG(0, "///====== Loaded: %s", 
							 "///====== Loaded: %s",  _sPath);
					 
		_EXE_LOADER_DEBUG(0, "///====== Export function table:", 
							 "///====== Export function table:",  "");				 
		GetLibraryExportTable((PMEMORYMODULE)handle);
		
	}
	_EXE_LOADER_DEBUG_("///===============================================================================================================///","");

	PMEMORYMODULE module = (PMEMORYMODULE)handle;
	fprintf(stdout, "Adresse %p \n", module->dllEntry);

	if (module == NULL || !module->isDLL || module->dllEntry == NULL || !module->isRelocated) {
		fprintf(stdout, "ARF.....\n");
		return 0;
	}
	
	if(((MEMORYMODULE*)handle)->section_text != 0){
		GDB_Send_AddSymbolFile((char*)_sPath, ((MEMORYMODULE*)handle)->section_text );
	}

	fprintf(stdout, "EXECUTION DllMain..... %p \n", module->dllEntry);
	//!BOOL WINAPI DllMain(HINSTANCE hinstDLL,/*handle to DLL module*/DWORD fdwReason,/*reason for calling function*/LPVOID lpReserved ) /*reserved*/
	void* _lpReserved = 0;
	module->dllEntry((HINSTANCE)module, DLL_PROCESS_ATTACH, _lpReserved);
	
	
	return handle;
}


bool fStartExeLoader(const char* _sPath) {
	
	// Lancer une instance du ManagedAlloc
	instance_AllocManager.ManagedAlloc_(1024, (const char*) __FILE__);
	
	bool resultat = fMainExeLoader(_sPath);
	
	// Tout nettoyer!
	instance_AllocManager.ManagedAlloc_clean();
	
	return resultat;
	
	
	// MemoryFreeLibrary(handle);
}

#ifdef ImWin
int main(int argc, char* argv[]) {
	printf("#\nMainCalled!! %d, %s", argc, argv[0]);

	fMainExeLoader(argv[1]);  // argv[0] is path
	printf("\n -- END -- \n");
	system("Pause");

	// MemoryFreeLibrary(handle);
	return 0;
}
#endif
