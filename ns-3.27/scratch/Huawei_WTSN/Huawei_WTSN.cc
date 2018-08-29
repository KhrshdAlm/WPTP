//============================================================================
// Name        : Huawei_WTSN.cpp
// Author      : Dennis Krummacker
// Version     :
// Copyright   : 
// Description : C++, Ansi-style
// Description : ns-3 Simulator. Wireless TSN.
// Description : Program Initializer for basic Framework including "DenKr_essentials"
//============================================================================




#define NO__HUAWEI_WTSN_CPP__FUNCTIONS


//////////////////////////////////////////////////////////////
//----------------------------------------------------------//
// NOTICE for Developers:
//    Consider the Folder
//       -> "global/"
//    Especially the Files
//       -> "global_settings.h"
//       -> "global_variables.c"  &  "global_variables.h"
//----------------------------------------------------------//
//////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////
//==================================================================================================//
//--------------------------------------------------------------------------------------------------//
//--------  Preamble, Inclusions  ------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//==================================================================================================//
//---  The very first intercepted mixed 'Plain C' & 'C++' Part  ------------------------------------//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
// At first include this ----------------------------------------------------------
//---------------------------------------------------------------------------------
extern "C"{
	#include "./global/global_settings.h"
	#include "./global/global_variables.h"
}
// Need this for e.g. some really fundamental Program Basics (if any)  ------------
//---------------------------------------------------------------------------------
#include "Huawei_WTSN_base.hpp"
// Then better start with this  ---------------------------------------------------
//---------------------------------------------------------------------------------
#include "Huawei_WTSN.hpp"
//==================================================================================================//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" {////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//==================================================================================================//
	//---  The 'Plain C' Part  -------------------------------------------------------------------------//
	//==================================================================================================//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------------------------------//
	//Just to nicely keep order:  -----------------------------------------------------
	//   First include the System / Third-Party Headers  ------------------------------
	//   Format: Use <NAME> for them  -------------------------------------------------
	//---------------------------------------------------------------------------------
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	// . . . .  System . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//#include <linux/if.h>
//	#include <net/if.h>
//	#include <sys/stat.h>
//	#include <fcntl.h>
//	#include <limits.h>
//	#include <stdbool.h>
	#include <errno.h>
	#include <stdio.h>
//	#include <stdlib.h>
//	#include <time.h>
//	#include <ctype.h>
//	#include <string.h>
//	#include <linux/types.h>
//	#include <sys/types.h>
//	#include <stdint.h>
//	#include <unistd.h>
//	#include <signal.h>
//	#include <malloc.h>
	//#include <pthread.h>
	//#include <semaphore.h>
	//#include <ncurses.h>
	//#include <pcap.h>
	//#include <arpa/inet.h>
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	// . . . .  Third Party  . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	#include "ns3/core-module.h"
	//#include "nl80211.h"
	//==================================================================================================//
	//==================================================================================================//
	//==================================================================================================//
	//Then include own Headers  -------------------------------------------------------
	//   Format: Use "NAME" for them  -------------------------------------------------
	//---------------------------------------------------------------------------------
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	// . . . .  DenKr_essentials . . . . . . . . . . . . . . . . . . . . . . . . . . .
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef NO__DENKR_ESSENTIALS // See "global/global_settings.h"
		#include "DenKr_essentials/DenKr_errno.h"
		#include "DenKr_essentials/PreC/DenKr_PreC.h"
		#include "DenKr_essentials/auxiliary.h"
		#ifdef DENKR_ESSENTIALS_AUXILIARY_H
			//#include "DenKr_essentials/Program_Files/control_cfg_file.h"
			//#include "DenKr_essentials/multi_threading.h"
			//#include "DenKr_essentials/plugins/DL_Libs.h"
		#endif
	#endif
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	// . . . .  Own Program proprietary  . . . . . . . . . . . . . . . . . . . . . . .
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//	#include "auxiliary/support_main.h"
	#ifdef DENKR_ESSENTIALS_PROGRAM_FILES_CONTROL_CFG_FILE_H
		#include "files/config/Huawei_WTSN_cfg_files.h"
	#endif
	#ifdef DENKR_ESSENTIALS__PLUGINS__DL_LIBS_H
		#include "plugins/Huawei_WTSN_plugins.h"
		#include "plugins/export/plugins_export.h"
	#endif
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
		#include "Huawei_WTSN_threads.h"
	#endif
	#if defined(DENKR_ESSENTIALS_MULTI_THREADING_H) && defined(DENKR_ESSENTIALS__GUI__GTKP__DENKR_GUI_GTK_MAIN__H) && defined(DENKR_ESSENTIALS__GUI__GTKP__GUI_FUNCTIONS__H)
		#include "GUI/GUI_thread.h"
	#endif
	//==================================================================================================//
	//==================================================================================================//
	//==================================================================================================//
	// To be honest, this below here isn't really of profitable benefit... ;oP  -------
	//---------------------------------------------------------------------------------
	#ifndef NO__DENKR_ESSENTIALS // See "global/global_settings.h"
//		#include "secret/DenKrement_funny.h"
	#endif
	//==================================================================================================//
	//==================================================================================================//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//--------------------------------------------------------------------------------------------------//
	//==================================================================================================//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
}///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//==================================================================================================//
//---  The 'C++' Part  -----------------------------------------------------------------------------//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//Just to nicely keep order:  -----------------------------------------------------
//   First include the System / Third-Party Headers  ------------------------------
//   Format: Use <NAME> for them  -------------------------------------------------
//---------------------------------------------------------------------------------
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . .  System . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include <iostream>
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . .  Third Party  . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#include <>
//==================================================================================================//
//==================================================================================================//
//==================================================================================================//
//Then include own Headers  -------------------------------------------------------
//   Format: Use "NAME" for them  -------------------------------------------------
//---------------------------------------------------------------------------------
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . .  DenKr_essentials . . . . . . . . . . . . . . . . . . . . . . . . . . .
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifndef NO__DENKR_ESSENTIALS // See "global/global_settings.h"
	//#include "DenKr_essentials/auxiliary.hpp"
#endif
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . .  Own Program proprietary  . . . . . . . . . . . . . . . . . . . . . . .
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#include "ns3_Simulator.hpp"
#include "ns3_Simulator_modules.hpp"
//==================================================================================================//
//==================================================================================================//
//==================================================================================================//
// Namespaces  --------------------------------------------------------------------
//---------------------------------------------------------------------------------
using namespace std;
//using namespace ns3;
//==================================================================================================//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////








//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  First Basic Setup  ----------------------------------------------------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
extern int DenKr_essentials_Init(int argc, char **argv);








//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  Additional Global Variables form this Files (try to left them few)  ---------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
#if (defined(NO__DENKR_ESSENTIALS__FUNCTIONS) || defined(NO__SUBFOLDERS)) && !defined(DENKR_ESSENTIALS_AUXILIARY_OVERRIDE)
	char ansi_escape_use=1;
#endif







//int main(int argc, char *argv[]){
int main(int argc, char **argv)
{
//	system("reset");
//	system("clear");
	int err=0;
//	int i=0;


	// Set Signal Handler for the User-desired Exit Ctrl-C
//	signal(SIGINT, sig_handler_ctrl_c);





    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Initialization of the DenKr_essentials-Library  --------------------------------------------//
    //----  Enables some Paths, Checks System-Endianess, Terminal ANSI-Escape  -------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	#if defined(DENKR_ESSENTIALS_AUXILIARY_H) && !defined(NO__DENKR_ESSENTIALS__FUNCTIONS) || defined(DENKR_ESSENTIALS_AUXILIARY_OVERRIDE)
		DenKr_essentials_Init(argc, argv);
		printfc(yellow,"Program Name:\n");printf("%s\n",ProgramName);
		printfc(yellow,"Program Path (executable):\n");printf("%s\n",ProgPath);
		printfc(yellow,"Storage Files Path (cfg, log, etc.):\n");printf("%s\n",StorageFilesPath);
		printf("\n");
		std::cout << std::flush;
	#endif





    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Start of the Command-Line Multiplexing  ----------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	//For now just to intercept some stuff "not really for valuable use", but just additional funny stuff
	//Maybe later on can be extended and the other Code will be integrated; certainly not.
	#ifdef SECRET_DENKREMENT_FUNNY_H
		int argc_proc=argc-1; char **argv_proc=argv+1;
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		if(argc_proc>0){
			if(strcmp(*argv_proc, "funny") == 0){
				// #include "secret/DenKrement_funny.h"
				err=Cmd_Line_Mux_funny(--argc_proc,++argv_proc);
				exit(err);
			}else{}
		}else{}
	#endif





    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Config-Files  ------------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
    //----  Initializes the global char [][] cfg_files_v_gl,
    //----       Checks for the Presence of Config-Files,
    //----       Eventually creates Default-Config-Files, if one doesn't already exist
    //--------------------------------------------------------------------------------------------------//
	#ifdef HUAWEI_WTSN__FILES__CONFIG__HUAWEI_WTSN_CFG_FILES_H
		// Creates cfg_files_v, usable like argv, with 'HUAWEI_WTSN_CFG_FILES_QUANTITY' as max Value
		#define cfg_files_v ((char **)cfg_files_v_)
		create_cfg_files_argv
		//--------------------------------------------------------------------------------------------------//
		// Presence Checking and eventually Default-Creation
		cfg_files_init_and_default(cfg_files_v);
	#endif





    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  ShMem, Inter-Thread, Thread-Handling, Synchronization  -------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	//----------------------------------------------------------------------------------------------
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
		// Initialize Shared-Memory, Thread-Management/Synchronization-Structures, Semaphores
		#define shmem_recv_start_self ShMem_recv_start(&(shmem_headers[DENKREMENT_THREAD__MAIN]))
		#define shmem_recv_finish_self ShMem_recv_finish(&(shmem_headers[DENKREMENT_THREAD__MAIN]))
		#define shmem_self (shmem_headers[DENKREMENT_THREAD__MAIN])
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		ThreadManager* thrall;
		DenKr_ThreadManager_init(&thrall,DENKREMENT_THREADS__MAX);
		struct ShMemHeader shmem_headers[DENKREMENT_THREADS__SHMEM_MAX];
		ShMem_Array_Sem_init(shmem_headers,DENKREMENT_THREADS__SHMEM_MAX);
	#endif





    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Dynamically Loaded Libraries (DL Lib, DLL, DSO [Dynamic Shared Object], '.so')  ------------//
    //----  Plugins  -----------------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	//----------------------------------------------------------------------------------------------
	#ifdef DENKR_ESSENTIALS__PLUGINS__DL_LIBS_H
		// Initialize Plugin-Manager, Plugin-Handle-Manager
		// Load and Register Plugins
		PluginManager* plugman;
		PluginManager_init(&plugman);
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		DL_Libs_plugins_discover_folder(plugman,ProgPath,ProgPath_length,DENKREMENT_PLUGINS_SUBFOLDER,sizeof(DENKREMENT_PLUGINS_SUBFOLDER)-1);
	#endif







	//=======================================================================================================================================
	//=======================================================================================================================================
	//=======================================================================================================================================
	//----------  Temporary Testing Section  --------------------------------------------------------------------------------------==========
	//=======================================================================================================================================
	//=======================================================================================================================================
	#ifdef DEBUG //----------------------------------------------------------------------------------------------------------------==========
	#ifndef RELEASE_VERSION //Just to get sure... ;o) that nothing got forgotten within the final Build  --------------------------==========
	//=======================================================================================================================================







	//exit(1);

	//=======================================================================================================================================
	#endif //----------------------------------------------------------------------------------------------------------------------==========
	#endif //----------------------------------------------------------------------------------------------------------------------==========
	//=======================================================================================================================================
	//=======================================================================================================================================
	//----------  End  -  Temporary Testing Section  ------------------------------------------------------------------------------==========
	//=======================================================================================================================================
	//=======================================================================================================================================
	//=======================================================================================================================================







    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Threads  -----------------------------------------------------------------------------------//
    //----  Start, Init, Get-Ready, Synchronized Start-of-Work  ----------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	//--------------------------------------------------------------------------------------------------
	// - - - - - Start Threads - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
		//  Graphical User Interface (GUI)  ------------------------------------------------------------//
		//  Does all the Stuff inside the Thread: Start, Load Layout-File, Build, Run Main-Loop  -------//
		GUI_START_THREAD
		//RYU Controller Connect
//		SDN_CTRL_COM_START_THREAD
		//Link Monitoring, based on SDN Methods (imported from Plugin)
		SDN_LINK_MONITORING_START_THREAD
		//Network Topology Discovery, using SDN (imported from Plugin)
		NETWORK_TOPOLOGY_DISCOVERY_START_THREAD
		// - - - - - Get Everyone Ready - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//A "Wait for Ready"-Routine
		//When all Threads are implemented, it should look like:
		//  Start all Threads sequentially
		//  Wait inside a ShMem Message Queue, like common inside the Threads.
		//     Maybe use temporary state-variables
		//  Every Thread sends a "Ready" Message after Initialization
		//     These could be cascaded: SDN-Ctrl-Thread waits for this Message from the Listen-Thread
		//     Main only waits for Ready-Message from the Sending-Thread
		//  Main first continues after one Ready-Message from every Thread was received.
		//It is done a bit more complex than just with a counter to avoid a failure like
		// multiple Ready-Msgs from the same Thread.
		//
		//Something familiar is done inside the Threads as well. After the Main received a Ready from every Thread
		// it sends a Ready to every Thread.
		//Every Threads waits for this Ready from the main, after it sent its own Ready.
		{
			//An Array, that contains the Thread-IDs of the spawned Threads. This becomes passed as a Pointer, to make it generic against dynamically loaded plugins
			//   You can set this Array manually
//			DenKr_essentials_ThreadID spawned_threads[]={DENKREMENT_THREAD__GUI,DENKREMENT_THREAD__SDN_CTRL_COM,DENKREMENT_THREAD__SDN_CTRL_COM_LISTEN};
			//   Or use the provided function. This uses the Data-Structures also provide by DenKr_essentials
			//      that helps you Tracking, which Threads are actually running (Basic, Predefined, Generic)
			DenKr_Threads_Main_Ready_Init(thrall, shmem_headers, DENKREMENT_THREAD__MAIN);
		}
	#endif
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -







    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  ShMem-Communication  -----------------------------------------------------------------------//
    //----  Consider this as the 'Main-Operation' of the Main-Thread  ----------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	//----------------------------------------------------------------------------------------------
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
//		while(1){
//			shmem_recv_start_self;
//
//			switch(shmem_self.type){
//			case SHMEM_MSG_TYPE__CANCEL_THREAD:
//				if(FLAG_CHECK(shmem_self.flags,SHMEM_MSG_FLAG__SRC_VALID)){
//					switch(shmem_self.src){
//					case DENKREMENT_THREAD__GUI:
//						shmem_recv_finish_self;
//						printfc(green,"KEY-Event:");printf(" Application-Termination initiated.\n");
//						goto ProgramTermination;
//						break;
//					default:
//						shmem_recv_finish_self;
//						printfc(red,"BUG:");printf(" CANCEL_THREAD-Msg to 'Main' from other src than GUI-Thread. Ignoring...\n");
//						break;
//					}
//				}else{
//					shmem_recv_finish_self;
//					printfc(red,"BUG:");printf(" CANCEL_THREAD-Msg to 'Main' from unspecified source. Ignoring...\n");
//				}
//				break;
//			default: // Programming Error
//				shmem_recv_finish_self;
//				printfc(red,"ERROR:");
//				printf(" Program-Bug encountered. Check the ShMem-Comm-Section of the Main-Thread.\n");
//				printf("\tReceived ShMem-Type: %d\n",shmem_self.type);
//				exit(MAIN_ERR_BUG);
//				break;
//			}
//		}
	#endif








    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  ns-3 Simulation Module  --------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	ns3_Simulator__main(argc, argv);
	//----------------------------------------------------------------------------------------------------








	//	ProgramTermination:
    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Program-Termination Sequence  --------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
	//Instruct the appropriate Threads to stop working and terminate
	//   Tell the SDN-Ctrl Communication Sending Thread to Terminate (and to handle the Termination of its related Listen-Thread
	ShMem_send_start(&(shmem_headers[DENKREMENT_THREAD__SDN_CTRL_COM]), 0, SHMEM_MSG_TYPE__CANCEL_THREAD);
	//-------- No content.
	ShMem_send_finish(&(shmem_headers[DENKREMENT_THREAD__SDN_CTRL_COM]));
	#endif
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	//Decided to NOT do it like this:
	// Cause that would be a bit of a waste. Main doesn't have to wait for every Thread,
	// because some Threads already wait for others.
	// So does for example the SDN-Ctrl-Communication Thread (Responsible for Sending and Connecting)
	// wait for the Receiving/Listening Thread (also spawned by it).
	//	for(i=0;i<DENKREMENT_THREADS_MAX;i++){
	//		pthread_join(allThreads[0],NULL);
	//	}
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
	//Join, i.e. wait for the applicable Threads to be finished.
	//   Join the SDN-Ctrl Com Thread
	pthread_join((thrall->allThreads)[DENKREMENT_THREAD__GUI],NULL);
	pthread_join((thrall->allThreads)[DENKREMENT_THREAD__SDN_CTRL_COM],NULL);
	pthread_join((thrall->allThreads)[DENKREMENT_THREAD__SDN_LINK_MONITORING],NULL);
	pthread_join((thrall->allThreads)[DENKREMENT_THREAD__NETWORK_TOPOLOGY_DISCOVERY],NULL);
	#endif
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	#ifndef NO__DENKR_ESSENTIALS // See "global/global_settings.h"
//		printf("\nHuawei_WTSN (DenKr_essentials-Part): Exiting with Error-Code: %d\n",err);
	#endif
	#ifdef DENKR_ESSENTIALS_MULTI_THREADING_H
		pthread_exit(NULL);
		#undef shmem_recv_start_self
		#undef shmem_recv_finish_self
		#undef shmem_self
	#endif

	#if defined(HUAWEI_WTSN__FILES__CONFIG__HUAWEI_WTSN_CFG_FILES_H) && defined(cfg_files_v)
		#undef cfg_files_v
	#endif



    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Final Termination Sequence  ----------------------------------------------------------------//
    //----    Totally and absolutely final...  ---------------------------------------------------------//
    //----  Program proprietary, independent from DenKr_essentials and all this... ;oP  ----------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	return err;
}









#undef NO__HUAWEI_WTSN_CPP__FUNCTIONS
