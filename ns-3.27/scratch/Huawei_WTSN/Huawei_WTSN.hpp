#ifndef HUAWEI_WTSN_H
#define HUAWEI_WTSN_H

/*
 * Authored by
 * Dennis Krummacker (03.06.14-)
 */






// Pretty much most of this is just old leftover
// I think it could be dropped... maybe... hm, let me say likely... I hope...
//---------------------------------------------------------------------------------------------
//		#define CFG_FILE_PATH_FILE_NAME "DenKrement.cfg"
//		//This becomes preceded onto the StorageFilesPath string variable
//		#define CFG_FILE_PATH_PREFIX "config/"
//		//So the complete File-Path is concatenated as
//		//		StorageFilesPath + CFG_FILE_PATH_PREFIX + CFG_FILE_PATH_FILE_NAME
//		//				StorageFilesPath has a preceding '/'
//		//				One should specify the CFG_FILE_PATH_PREFIX including a preceding '/'
//
//		#define CFG_FILE_PATH_PREFIX_ADAPTTXPOWER "adapttxpower_"
//		#define LOG_FILE_PATH_PREFIX_ADAPTTXPOWER "log_adapttxpower_"


#ifndef ETH_ALEN
	#define ETH_ALEN 6
#endif


/* libnl 1.x compatibility code */
/*
#if !defined(CONFIG_LIBNL20) && !defined(CONFIG_LIBNL30)
#define nl_sock nl_handle
#endif
*/










#endif /* HUAWEI_WTSN_H */
