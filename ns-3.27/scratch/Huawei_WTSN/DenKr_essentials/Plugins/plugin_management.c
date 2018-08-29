/*
 * plugin_management.c
 *
 *  Created on: 26.07.2017
 *      Author: Dennis Krummacker
 */





#define DENKR_ESSENTIALS__PLUGINS__PLUGIN_MANAGEMENT_C
#define NO_DENKR_ESSENTIALS__PLUGINS__PLUGIN_MANAGEMENT_C_FUNCTIONS




//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//--------  Preamble, Inclusions  ------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//
//==================================================================================================//
// At first include this ----------------------------------------------------------
//---------------------------------------------------------------------------------
//#include <global/global_settings.h>
// Then better start with this  ---------------------------------------------------
//---------------------------------------------------------------------------------
//#include "DenKrement.h"
//==================================================================================================//
//==================================================================================================//
//==================================================================================================//
//Just to nicely keep order:  -----------------------------------------------------
//   First include the System / Third-Party Headers  ------------------------------
//   Format: Use <NAME> for them  -------------------------------------------------
//---------------------------------------------------------------------------------
//#include <dlfcn.h>
//#include <stdint.h>
//#include <errno.h>
#include <stdlib.h>
//==================================================================================================//
//==================================================================================================//
//==================================================================================================//
//Then include own Headers  -------------------------------------------------------
//   Format: Use "NAME" for them  -------------------------------------------------
//---------------------------------------------------------------------------------
#include "../PreC/DenKr_PreC.h"
//#include "../DenKr_errno.h"
#include "../auxiliary.h"
//#include "function_creator.h"
//#include "getRealTime.h"
//#include "Program_Files/P_Files_Path.h"
#include "DenKr_essentials/Plugins/plugin_management.h"
//==================================================================================================//
//==================================================================================================//
//////////////////////////////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------//
//==================================================================================================//



static void PluginHandler_init(PluginHandler** plughan) {
	*plughan = malloc(sizeof(**plughan));
	int temp_mem_size=DenKr_plugin_role__MAX*sizeof(*((*plughan)->handles));
	(*plughan)->handles=malloc(temp_mem_size);
	memset((*plughan)->handles,0,temp_mem_size);
	(*plughan)->generic_c=0;
	(*plughan)->generic=NULL;
}


static void PluginHandler_free(PluginHandler** plughan) {
	int i;

	if((*plughan)->handles){
		free((*plughan)->handles);
	}

	struct PluginHandleGeneric* tempp=(*plughan)->generic;
	struct PluginHandleGeneric* tempp_n=NULL;
	for(i=0;i<(*plughan)->generic_c;i++){
		tempp_n=tempp->next;
		free(tempp);
		tempp=tempp_n;
	}

	free(*plughan);
	*plughan=NULL;
}





DENKR_PLUGIN_MANAGER_INIT_SIGNATURE {
	*plugman = malloc(sizeof(**plugman));
	int temp_mem_size=DenKr_plugin_role__MAX*sizeof(*((*plugman)->predef));
	(*plugman)->predef=malloc(temp_mem_size);
	memset((*plugman)->predef,0,temp_mem_size);
	(*plugman)->generic_c=0;
	(*plugman)->generic=NULL;
	PluginHandler_init(&((*plugman)->PluginHandler));
}


DENKR_PLUGIN_MANAGER_FREE_SIGNATURE {
	PluginHandler_free(&((*plugman)->PluginHandler));
	if((*plugman)->predef){
		free((*plugman)->predef);
	}

	struct PluginRoleGeneric* tempp=(*plugman)->generic;
	struct PluginRoleGeneric* tempp_n=NULL;
	while(tempp){
		tempp_n=tempp->next;
		free(tempp);
		tempp=tempp_n;
	}

	free(*plugman);
	*plugman=NULL;
}




DENKR_PLUGIN_MANAGER_REG_ROLE_PREDEF
{
	FLAG_SET((plugman->predef)[role].flags,DENKR_PLUGINS_ROLE_FLAG__ROLE_DEFINED);
	(plugman->predef)[role].work_type=work_type;
	(plugman->predef)[role].hook=hook;
}




DENKR_PLUGIN_MANAGER_REG_ROLE_GENERIC
{

}








#undef DENKR_ESSENTIALS__PLUGINS__PLUGIN_MANAGEMENT_C
#undef NO_DENKR_ESSENTIALS__PLUGINS__PLUGIN_MANAGEMENT_C_FUNCTIONS
