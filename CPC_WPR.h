/*
	******************* ENTETE *******************

             ===== ========================== =====
    ============= ******** Wrapper ********* =============
 ================  Cpcdos OSx & CPinti Core  ================
    ============= ************************** =============
            ===== ========================== =====

			Auteur Sebastien FAVIER
	  Contributeur Vous!

	  Copyright (c) microsf01
	  http://cpcdos.fr.nf/

 Condition :
	Ce fichier réservé uniquement aux adhérants du developpement C/C++
	du noyau. Pour toutes questions : sebastien.ordinateur@hotmail.fr

 Description :
	Header/Wrapper du noyau Cpcdos & CPinti Core pour la programmation
	C/C++ de modules externe. Ceci va permettre aux adhérants de créer
	des fonctions native pour cpcdos. Il s'agit d'un header qui
	contient la liste des fonctions de Cpcdos dont le developpeur aura
	besoin pour developper ses fonction afin qu'il soit nativement
	dependant des fonctions du noyau sans passer par le CpcdosC+.

		Creation le 25 avril 2016

		Mise a jour le 10 Aout 2016

*/

	extern bool bOnCpcDos;
	extern int nCpcVerMajor;
	extern int nCpcVerMinor;
	extern "C" void 			cpc_Set_Version				(int _nCpcVerMajor, int _nCpcVerMinor);

#ifdef InCpcDosCore

	extern "C" int 			cpc_Creer_Contexte				(int TAILLEX, int TAILLEYn);
	extern "C" void* 		cpc_Obtenir_Zone_Contexte		(int ID);
	extern "C" int 			cpc_Blitter						(int ID);
	extern "C" void 		cpc_CCP_Exec_Commande			(char* COMMANDE, int ID);
	extern "C" int 			cpc_CCP_Exec_Thread_cpc			(char* chemin, int Thread_Priorite);

	extern "C" char*		cpc_Exec_en_cours				(int ARG);


	extern "C" void			cpc_cpinti_debug_cpinticore		(char* texte, int alerte);
	extern "C" int			cpc_cpinti_Fichier_Existe		(char* chemin);
	extern "C" unsigned int	cpc_cpinti_Taille_Fichier		(char* chemin);
	extern "C" int 			cpc_cpinti_Lire_Fichier_complet	(char* Chemin, char* Mode, char *retour_str);

	extern "C" double 		cpc_cpinti_Obtenir_Temps_ms		(double Temps_av);

#else

	//Dynmaic loaded CpcDos (like external .exe)
	#ifndef STDCALL
		#define STDCALL __stdcall
	#endif

	#ifndef FuncTableStructure
		typedef void* (*FUNC_)();
		typedef struct {
			const char* sFuncName;
			FUNC_ dFunc;
		} FuncTable;
	#endif

	extern "C" FUNC_ STDCALL GetProcAddress(void* hModule, char* lpProcName); //*Wrap over standard windows function (from dll on windows)*


	typedef 	int 			(*FUNC_cpc_Creer_Contexte)				(int TAILLEX, int TAILLEYn); 			extern FUNC_cpc_Creer_Contexte cpc_Creer_Contexte;
	typedef 	void* 			(*FUNC_cpc_Obtenir_Zone_Contexte)		(int ID); 								extern FUNC_cpc_Obtenir_Zone_Contexte cpc_Obtenir_Zone_Contexte;
	typedef 	int 			(*FUNC_cpc_Blitter)						(int ID); 								extern FUNC_cpc_Blitter cpc_Blitter;
	typedef 	void 			(*FUNC_cpc_CCP_Exec_Commande)			(char* COMMANDE, int ID); 				extern FUNC_cpc_CCP_Exec_Commande cpc_CCP_Exec_Commande;
	typedef 	int 			(*FUNC_cpc_CCP_Exec_Thread_cpc)			(char* chemin, int Thread_Priorite); 	extern FUNC_cpc_CCP_Exec_Thread_cpc cpc_CCP_Exec_Thread_cpc;
	typedef 	char*			(*FUNC_cpc_Exec_en_cours)				(int ARG);								extern FUNC_cpc_Exec_en_cours cpc_Exec_en_cours;
	
	typedef		void			(*FUNC_cpc_cpinti_debug_cpinticore)		(char* texte, int alerte);				extern FUNC_cpc_cpinti_debug_cpinticore cpc_cpinti_debug_cpinticore;
	typedef 	int				(*FUNC_cpc_cpinti_Fichier_Existe)		(char* chemin); 						extern FUNC_cpc_cpinti_Fichier_Existe cpc_cpinti_Fichier_Existe;
	typedef 	unsigned int	(*FUNC_cpc_cpinti_Taille_Fichier)		(char* chemin); 						extern FUNC_cpc_cpinti_Taille_Fichier cpc_cpinti_Taille_Fichier;
	typedef 	int 			(*FUNC_cpc_cpinti_Lire_Fichier_complet)	(char* Chemin, char* Mode, char *retour_str); extern FUNC_cpc_cpinti_Lire_Fichier_complet cpc_cpinti_Lire_Fichier_complet;

#endif


#ifndef DEFINITION_WRAPPER
	#define DEFINITION_WRAPPER

	class CpcdosOSx_CPintiCore
	{
		public:
		// ************ For GZE 3D Graphic engine ************
		int Create_Context			(int TAILLEX, int TAILLEYn);				// Creer un contexte de donnees
		void* Init_Get_Context_PTR	(int ID);									// Initialiser et obtenir le pointeur du contexte
		void Blitting				(int ID);									// Blitter le buffer
		
		
		// ***************** CpcdosC+ Engine *****************
		void Shell_CCP				(char* COMMAND, int LEVEL);					// Executer une commande CpcdosC+<
		int Shell_FILE				(char* FILE, int THREAD_PRIORITY); 			// Executer un fichier CpcdosC+ multithreadé ou pas
		
		// ******************** Cpcdos OSx ********************
		char* Get_Path(int ARG);												// Obtenir le chemin d'acces en cours
		
		// ******************* CPinti Core *******************
		void			debug_log(const char* text, int alert);
		int			 	File_exist(char* path);									// Renvoie 1 OK 0 FAUX
		unsigned int 	File_size(char* path);									// Obtenir la taille d'un fichier
		int 			File_read_all(char* path, char* mode, char *data);		// Lire TOUT le contenu d'un fichier
		
		// ******************* CPinti Core Fonctions utiles *******************
		double 			get_time_ms	(double Temps_av);	
	};

	//////////////////// For GZE 3D Graphic engine ////////////////////
	inline int CpcdosOSx_CPintiCore::Create_Context(int TAILLEX, int TAILLEYn)
	{
		return cpc_Creer_Contexte(TAILLEX, TAILLEYn);
		
	}
	
	inline void* CpcdosOSx_CPintiCore::Init_Get_Context_PTR(int ID)
	{
		return cpc_Obtenir_Zone_Contexte(ID);
	}
	
	inline void CpcdosOSx_CPintiCore::Blitting(int ID)
	{
		cpc_Blitter(ID);
	}
	
	////////////////////// For CpcdosC+ //////////////////////
	inline void CpcdosOSx_CPintiCore::Shell_CCP(char* COMMAND, int LEVEL)
	{
		cpc_CCP_Exec_Commande(COMMAND, LEVEL);
	}
	
	inline int CpcdosOSx_CPintiCore::Shell_FILE(char* FILE, int Thread_Priority)
	{
		return cpc_CCP_Exec_Thread_cpc(FILE, Thread_Priority);
	}
	
	
	//////////////////// For Cpcdos OSx ////////////////////
	inline char* CpcdosOSx_CPintiCore::Get_Path(int ARG)
	{
		return (char*) cpc_Exec_en_cours(ARG);
	}
	
	//////////////////// For CPinti Core ////////////////////	
	
	inline void CpcdosOSx_CPintiCore::debug_log(const char* text, int alert)
	{
		cpc_cpinti_debug_cpinticore((char*) text, alert);
	}
	
	inline int CpcdosOSx_CPintiCore::File_exist(char* path)
	{
		return cpc_cpinti_Fichier_Existe(path);
	}
	
	inline unsigned int CpcdosOSx_CPintiCore::File_size(char* path)
	{
		return cpc_cpinti_Taille_Fichier(path);
	}
	
	inline int CpcdosOSx_CPintiCore::File_read_all(char* path, char* mode, char* data)
	{
		return (int) cpc_cpinti_Lire_Fichier_complet(path, mode, data);
	}
	
	inline double CpcdosOSx_CPintiCore::get_time_ms(double Temps_av)
	{
		return cpc_cpinti_Obtenir_Temps_ms(Temps_av);
	}

	//////////////////// For  ////////////////////
	//////////////////// For  ////////////////////
	//////////////////// For  ////////////////////
	//////////////////// For  ////////////////////

#endif

