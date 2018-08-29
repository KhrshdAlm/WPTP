//============================================================================
// Name        : wifi_sim_PERR.cc
// Author      : Dennis Krummacker
// Version     :
// Copyright   : 
// Description : C++, Ansi-style
// Description : ns-3, WLAN, PERR Simulation
//============================================================================




#define NO__WIFI_SIM_PERR_CC__FUNCTIONS


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
	#include <limits.h>
	#include <stdbool.h>
	#include <errno.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <ctype.h>
	#include <string.h>
//	#include <linux/types.h>
//	#include <sys/types.h>
//	#include <stdint.h>
//	#include <unistd.h>
	//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	// . . . .  Third Party  . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		//- . - . - Do not use the ns-3 Headers here, they are C++ Headers - . - . -//
		//#include "ns3/core-module.h"
		//#include "ns3/network-module.h"
		//#include "ns3/internet-module.h"
		//#include "ns3/point-to-point-module.h"
		//#include "ns3/applications-module.h"
	//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
//	#include "Huawei_WTSN_.h"
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
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
//#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
//#include "ns3/csma-module.h"
//Tracing:
#include "ns3/object.h"
#include "ns3/uinteger.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"
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
//#include "wifi_sim_PERR_pcapMonitor.h"
#include "files/config/Huawei_WTSN_cfg_files.h"
//==================================================================================================//
//==================================================================================================//
//==================================================================================================//
// Namespaces  --------------------------------------------------------------------
//---------------------------------------------------------------------------------
using namespace std;
using namespace ns3;
//==================================================================================================//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////





//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  Notices, Informations, Further Reading  -------------------------------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
//
//  https://www.nsnam.org/docs/models/html/spectrum.html
//
//##########========================================================================================================================#####
//#######################################################################################################################################
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  First Basic Setup  ----------------------------------------------------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
NS_LOG_COMPONENT_DEFINE ("Huawei_WTSN - ns-3, WiFi, PERR-Simulation");
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// ------ NOTE: The ns-3 pcap Trace-Files ("<FileName>.pcap") are found after Simulation in the ns-3.27 root-Folder ---------------------
#define NS3_PCAP_TRACENAME_TEMPLATE        "Huawei_WTSN_PERRSim_" NS3_PCAP_TRACENAME_PARAM1 NS3_PCAP_TRACENAME_SNR
#define NS3_PCAP_TRACENAME_BASE_STRLEN     20
#define NS3_PCAP_TRACENAME_PARAM1_NUM      "%03d"
#define NS3_PCAP_TRACENAME_PARAM1          "[dist#" NS3_PCAP_TRACENAME_PARAM1_NUM "#m]"
#define NS3_PCAP_TRACENAME_PARAM1_STRLEN   12
#define NS3_PCAP_TRACENAME_SNR_NUM         "%03d"
#define NS3_PCAP_TRACENAME_SNR             "[SNR#" NS3_PCAP_TRACENAME_SNR_NUM "#dB]"
#define NS3_PCAP_TRACENAME_SNR_STRLEN      12
#define NS3_PCAP_TRACENAME_SIZEOF          NS3_PCAP_TRACENAME_BASE_STRLEN+NS3_PCAP_TRACENAME_PARAM1_STRLEN+NS3_PCAP_TRACENAME_SNR_STRLEN+1
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//#define NS3_ASCII_TRACENAME_TEMPLATE        "Huawei_WTSN_PERRSim_" NS3_PCAP_TRACENAME_PARAM1 NS3_PCAP_TRACENAME_SNR
//#define NS3_ASCII_TRACENAME_BASE_STRLEN     20
//#define NS3_ASCII_TRACENAME_PARAM1_NUM      "%03d"
//#define NS3_ASCII_TRACENAME_PARAM1          "<dist#" NS3_PCAP_TRACENAME_PARAM1_NUM "#m>"
//#define NS3_ASCII_TRACENAME_PARAM1_STRLEN   12
//#define NS3_ASCII_TRACENAME_SNR_NUM         "%03d"
//#define NS3_ASCII_TRACENAME_SNR             "<SNR#" NS3_PCAP_TRACENAME_SNR_NUM "#dB>"
//#define NS3_ASCII_TRACENAME_SNR_STRLEN      12
//#define NS3_ASCII_TRACENAME_SIZEOF          NS3_PCAP_TRACENAME_BASE_STRLEN+NS3_PCAP_TRACENAME_PARAM1_STRLEN+NS3_PCAP_TRACENAME_SNR_STRLEN+1











//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  Function-Set Signature Creation &  ------------------------------------------------------------------------------=====#####
//##########  Prototype Declaration  ------------------------------------------------------------------------------------------=====#####
//##########     (actual Function-Definition is found below Main-Function)  ---------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
//--------------------------------------------------------------------------------------------------//
//---------   (Assume this first sub-section as something like a)   --------------------------------//
//----------   ***  [FileName]__function_creator_funcname.h  ***   ---------------------------------//
//--------------------------------------------------------------------------------------------------//
//---------------  You wonder "why all this complex Definition-Stuff?". That's why:  ---------------//
//---------------    With this you have only "one and only Place for the Func-Signature Definition.
//---------------    From that you can't come into trouble with Signature-Conflicts, when changing the Name or Parameters and stuff like that.
//---------------  Notation (Example):
//---------------    Parameters: int argc, char* vect, uint64_t* res
//---------------    Funcname: sim_iter_trans
//---------------    Signature: sim_iter_trans(int argc, char* vect, uint64_t* res)
//---------------    Prototype: int sim_iter_trans(int argc, char* vect, uint64_t* res)
//--------------------------------------------------------------------------------------------------//
#define wifi_sim_PERR__coreSim__FUNCNAME     wifi_sim_PERR__coreSim
#define wifi_sim_PERR__coreSim__PAREMETERS   char* tracename, int num_packs, uint32_t nWifi, int distance, int tx_pow, WifiPhyHelper* phy_point
#define wifi_sim_PERR__coreSim__SIGNATURE    wifi_sim_PERR__coreSim__FUNCNAME (wifi_sim_PERR__coreSim__PAREMETERS)
#define wifi_sim_PERR__coreSim__PROTOTYPE    static int wifi_sim_PERR__coreSim__SIGNATURE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define wifi_sim_PERR__coreSim_channel_wrap__FUNCNAME     wifi_sim_PERR__coreSim_channel_wrap
#define wifi_sim_PERR__coreSim_channel_wrap__PAREMETERS   EXCLUDE_LAST_ENTRY_FROM_LIST(wifi_sim_PERR__coreSim__PAREMETERS)
#define wifi_sim_PERR__coreSim_channel_wrap__SIGNATURE    wifi_sim_PERR__coreSim_channel_wrap__FUNCNAME (wifi_sim_PERR__coreSim_channel_wrap__PAREMETERS)
#define wifi_sim_PERR__coreSim_channel_wrap__PROTOTYPE    static int wifi_sim_PERR__coreSim_channel_wrap__SIGNATURE
//--------------------------------------------------------------------------------------------------//
//---------   (Declaration of the Function Prototypes)   -------------------------------------------//
//--------------------------------------------------------------------------------------------------//
wifi_sim_PERR__coreSim__PROTOTYPE;
wifi_sim_PERR__coreSim_channel_wrap__PROTOTYPE;











//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  Main Function  --------------------------------------------------------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
int
wifi_sim_PERR__main (int argc, char **argv)
{
	int err=0;



    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  ns-3 Setup  --------------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
//	NS_LOG_UNCOND ("\n--> WLAN PERR-Simulation started.");
	std::cout << std::flush;
	printfc(cyan,"--> WLAN PERR-Simulation started.\n");
	std::cout << std::flush;
	//
	// Create the Full-Folder-Paths in use for the Simulation Results
	{
		{
			int SimResFolder_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH);
			char SimResFolder_fullpath[SimResFolder_fullpath_len];
			snprintf(SimResFolder_fullpath,SimResFolder_fullpath_len,"%s" HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH, StorageFilesPath);
			CREATE_COMPLETE_FOLDER_PATH(SimResFolder_fullpath);
		}
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		{
			int SimResFolder_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH);
			char SimResFolder_fullpath[SimResFolder_fullpath_len];
			snprintf(SimResFolder_fullpath,SimResFolder_fullpath_len,"%s" HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH, StorageFilesPath);
			CREATE_COMPLETE_FOLDER_PATH(SimResFolder_fullpath);
		}
	}







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
    //----  Main Operation  ----------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
		// Verschiedene Simulations-Parameter anpassen, um deren Einfluss zu zeigen:
		//  -> Legende: (X) Sinnlos (nach fundierter Überlegung), (I) Sinnhaft
		//       (I) Distance
		//       (I) MCS
		//       (X) # Clients, (Not relevant for lower Bound, since it 'only' introduces random collisions, which are avoided with using techniques such as PCF (Point Coordination Function) )
		//       (X) Transmission Power, (Should directly correlated with Distance)
			// Parameter am Kanal-Modell ändern -> Statistik: PERR über verschiedene SNR
				// Mehrere Pakete senden, um PERR Statistik zu generieren
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	char tracename[NS3_PCAP_TRACENAME_SIZEOF];
	int num_packets=10;
	uint32_t num_Wifi=2;
	int distance=1;
	int tx_pow=15;

	//First, use the "Standart MCS" and roll over the distance. Distance Unit is in "meters" and "double"
	for(distance=0;distance<=30;distance+=1){
		snprintf(tracename,NS3_PCAP_TRACENAME_SIZEOF,NS3_PCAP_TRACENAME_TEMPLATE,distance,0);
		//That function loops over the SNR and creates various appropriate channels. Inside it, the core_Sim is called.
		wifi_sim_PERR__coreSim_channel_wrap(tracename, num_packets,num_Wifi,distance,tx_pow);
	}
	//Then adjust for some "reasonable" PERR distance and loop over the MCS
	distance=4;//TODO: Fill in







    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Program-Termination Sequence  --------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
//	NS_LOG_UNCOND ("<-- WLAN PERR-Simulation terminated.\n");
	std::cout << std::flush;
	printfc(cyan,"<-- WLAN PERR-Simulation terminated.\n\n");
	std::cout << std::flush;

	return err;
}








//#######################################################################################################################################
//##########========================================================================================================================#####
//##########  Additional Functions  -------------------------------------------------------------------------------------------=====#####
//##########========================================================================================================================#####
//#######################################################################################################################################
//------   (Function Definitions according to the corresponding Prototype Declarations above)   ----//
//--------------------------------------------------------------------------------------------------//


wifi_sim_PERR__coreSim_channel_wrap__PROTOTYPE
{
	double SNR;

	//Some calculating Loop here, after MiKa figured Channel out.
	{
		SNR=15;//Please fill in Formula or Numerous Value...
		NS_LOG_UNCOND ("-> Simulation for SNR "<<SNR<<" dB.");
		YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
		YansWifiPhyHelper phy = YansWifiPhyHelper::Default ();
		phy.SetChannel (channel.Create ());

		{//Finishing Name for Trace-File
			char* searchSNR=tracename;
			while(1){
				if(*searchSNR=='['){
					if( (*(searchSNR+1)=='S') && (*(searchSNR+2)=='N') && (*(searchSNR+3)=='R') ){
						break;
					}
				}
				searchSNR++;
				if(searchSNR>tracename+NS3_PCAP_TRACENAME_SIZEOF){// Just to get sure. You know, for the sake of not creating out-of-bounds
					printfc(red,"ERROR: ");printf("Some Bug detected while creating the File-Name for the pcap-Trace-File. Thus exit.\n");
					exit(MAIN_ERR_BUG);
				}
			}
			snprintf(searchSNR,NS3_PCAP_TRACENAME_SNR_STRLEN+1,NS3_PCAP_TRACENAME_SNR,(int)SNR);
		}

		wifi_sim_PERR__coreSim(tracename, num_packs,nWifi,distance,tx_pow,&phy);
		NS_LOG_UNCOND ("<-> SNR "<<SNR<<" dB done. Results:");
		NS_LOG_UNCOND ("<- ");
		NS_LOG_UNCOND ("");
	}

	return 0;
}







wifi_sim_PERR__coreSim__PROTOTYPE
{
    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  ns-3 Setup  --------------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	NS_LOG_UNCOND ("  --> WLAN PERR Core-Simulation started.");
	#define phy (*phy_point)
	#define PACK_INTERVAL 0.1
		// -> In Seconds




    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Main Operation  ----------------------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	bool verbose = true;
//	bool tracing = true;

//	CommandLine cmd;
//	cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
//	cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
//	cmd.AddValue ("tracing", "Enable pcap tracing", tracing);
//
//	cmd.Parse (argc,argv);

	// The underlying restriction of 18 is due to the grid position
	// allocator's configuration; the grid layout will exceed the
	// bounding box if more than 18 nodes are provided.
	if (nWifi > 18)
	{
	  std::cout << "nWifi should be 18 or less; otherwise grid layout exceeds the bounding box" << std::endl;
	  return 1;
	}

	if (verbose)
	{
	  LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
	  LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
	}


	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nWifi);
	NodeContainer wifiApNode;
	wifiApNode.Create(1);

	WifiHelper wifi;
	wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

	WifiMacHelper mac;
	Ssid ssid = Ssid ("ns-3-ssid");
	mac.SetType ("ns3::StaWifiMac",
			   "Ssid", SsidValue (ssid),
			   "ActiveProbing", BooleanValue (false));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);

	mac.SetType ("ns3::ApWifiMac",
			   "Ssid", SsidValue (ssid));

	NetDeviceContainer apDevices;
	apDevices = wifi.Install (phy, mac, wifiApNode);

	MobilityHelper mobility;

	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
								 "MinX", DoubleValue (0.0),
								 "MinY", DoubleValue (0.0),
								 "DeltaX", DoubleValue (5.0),
								 "DeltaY", DoubleValue (10.0),
								 "GridWidth", UintegerValue (3),
								 "LayoutType", StringValue ("RowFirst"));

	/*mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
							 "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	*/
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (wifiStaNodes);

	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (wifiApNode);

	InternetStackHelper stack;
	stack.Install (wifiApNode);
	stack.Install (wifiStaNodes);

	Ipv4AddressHelper address;

	address.SetBase ("10.1.1.0", "255.255.255.0");
	Ipv4InterfaceContainer wifiAPInterface;
	address.Assign (staDevices);
	wifiAPInterface = address.Assign (apDevices);

	UdpEchoServerHelper echoServer (9);
//	PERRpcapMonitorHelper PERRMon(0);

	ApplicationContainer serverApps = echoServer.Install (wifiApNode.Get (0));
//	serverApps = PERRMon.Install (wifiApNode.Get (0));
	serverApps.Start (Seconds (1.0));
	serverApps.Stop (Seconds (PACK_INTERVAL * num_packs + 10.0));

	UdpEchoClientHelper echoClient (wifiAPInterface.GetAddress(0), 9);
	echoClient.SetAttribute ("MaxPackets", UintegerValue (num_packs));
	echoClient.SetAttribute ("Interval", TimeValue (Seconds (PACK_INTERVAL)));
	echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

	ApplicationContainer clientApps =  echoClient.Install (wifiStaNodes.Get (nWifi-1));
	clientApps.Start (Seconds (2.0));
	clientApps.Stop (Seconds (PACK_INTERVAL * num_packs + 10.0));

	//Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	Simulator::Stop (Seconds (PACK_INTERVAL * num_packs + 60.0));

	//Create Full-Path for pcap&ascii Trace-File(s)
	int pcapTraceFile1_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH)-1 + strlen(tracename) +3 +1;//contains: +strlen("_ap"), +1 for '\0'
	char pcapTraceFile1_fullpath[pcapTraceFile1_fullpath_len];
	snprintf(pcapTraceFile1_fullpath,pcapTraceFile1_fullpath_len,"%s" HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH "%s" "_ap", StorageFilesPath, tracename);
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	int pcapTraceFile2_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH)-1 + strlen(tracename) +4 +1;//contains: +strlen("_sta"), +1 for '\0'
	char pcapTraceFile2_fullpath[pcapTraceFile2_fullpath_len];
	snprintf(pcapTraceFile2_fullpath,pcapTraceFile2_fullpath_len,"%s" HUAWEI_WTSN_FILES_PCAPTRACE_SUBPATH "%s" "_sta", StorageFilesPath, tracename);
	//
	int asciiTraceFile1_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH)-1 + strlen(tracename) +9 +3 +1;//contains: +strlen("_Mobility") +strlen(".tr"), +1 for '\0'
	char asciiTraceFile1_fullpath[asciiTraceFile1_fullpath_len];
	snprintf(asciiTraceFile1_fullpath,asciiTraceFile1_fullpath_len,"%s" HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH "%s_Mobility.tr", StorageFilesPath, tracename);
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	int asciiTraceFile2_fullpath_len = StorageFilesPath_length + sizeof(HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH)-1 + strlen(tracename) +8 +3 +1;//contains: +strlen("_WiFiPhy") +strlen(".tr"), +1 for '\0'
	char asciiTraceFile2_fullpath[asciiTraceFile2_fullpath_len];
	snprintf(asciiTraceFile2_fullpath,asciiTraceFile2_fullpath_len,"%s" HUAWEI_WTSN_FILES_ASCIITRACE_SUBPATH "%s_WiFiPhy.tr", StorageFilesPath, tracename);
	//
	std::cout << std::flush;
	printf("Creating pcap-Trace-File:\n\t%s\n",pcapTraceFile1_fullpath);
	printf("Creating pcap-Trace-File:\n\t%s\n",pcapTraceFile2_fullpath);
	printf("Creating ASCII-Trace-File:\n\t%s\n",asciiTraceFile1_fullpath);
	printf("Creating ASCII-Trace-File:\n\t%s\n",asciiTraceFile2_fullpath);
	std::cout << std::flush;

//	if (tracing == true)
	{
	  phy.EnablePcap (pcapTraceFile1_fullpath, apDevices.Get (0));
	  phy.EnablePcap (pcapTraceFile2_fullpath, staDevices.Get (0));
	  AsciiTraceHelper ascii;
//	  mobility.EnableAsciiAll (ascii.CreateFileStream (asciiTraceFile1_fullpath));//MobilityHelper, no content
	  phy.EnableAsciiAll (ascii.CreateFileStream (asciiTraceFile2_fullpath));//WiFiPhyHelper
	}

	Simulator::Run ();
	Simulator::Destroy ();


    //==================================================================================================//
    //--------------------------------------------------------------------------------------------------//
    //----  Program-Termination Sequence  --------------------------------------------------------------//
    //--------------------------------------------------------------------------------------------------//
    //==================================================================================================//
	NS_LOG_UNCOND ("  <-- WLAN PERR Core-Simulation terminated.");

	#undef PACK_INTERVAL
	#undef phy
	return 0;
}








//--------------------------------------------------------------------------------------------------//
//---------   (Clear all the used #defines)   ------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
#undef wifi_sim_PERR__coreSim__FUNCNAME
#undef wifi_sim_PERR__coreSim__PAREMETERS
#undef wifi_sim_PERR__coreSim__SIGNATURE
#undef wifi_sim_PERR__coreSim__PROTOTYPE
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#undef wifi_sim_PERR__coreSim_channel_wrap__FUNCNAME
#undef wifi_sim_PERR__coreSim_channel_wrap__PAREMETERS
#undef wifi_sim_PERR__coreSim_channel_wrap__SIGNATURE
#undef wifi_sim_PERR__coreSim_channel_wrap__PROTOTYPE









#undef NO__WIFI_SIM_PERR_CC__FUNCTIONS
