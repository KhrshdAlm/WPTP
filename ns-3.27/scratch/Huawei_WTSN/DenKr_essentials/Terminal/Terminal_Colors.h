/*
 * Terminal_Colors.h
 *
 *  Created on: 26.09.2017
 *      Author: Dennis Krummacker
 */

#ifndef DENKR_ESSENTIALS__TERMINAL__TERMINAL_COLORS_H_
#define DENKR_ESSENTIALS__TERMINAL__TERMINAL_COLORS_H_



#define DENKR__COLOR_ANSI__ESC_Base           "\033"
//256-color mode:
#define DENKR__COLOR_ANSI__escape_fg          DENKR__COLOR_ANSI__ESC_Base"[38;5;"
#define DENKR__COLOR_ANSI__escape_bg          DENKR__COLOR_ANSI__ESC_Base"[48;5;"
//----------------------------------------------
#define DENKR__COLOR_ANSI__reset              "\x1b[0m"
//----------------------------------------------
//----------------------------------------------



#define DENKR__COLOR_ANSI__THREAD_BASIC       DENKR__COLOR_ANSI__escape_fg"56m"
#define DENKR__COLOR_ANSI__DL_LIBS            DENKR__COLOR_ANSI__escape_fg"14m"
	#define DENKR__COLOR_ANSI__PLUGINS            DENKR__COLOR_ANSI__DL_LIBS

//--------------------------------------------------------------------------

#define ANSI_COLOR_DENKR__COLOR_ANSI__THREAD_BASIC       DENKR__COLOR_ANSI__THREAD_BASIC
#define ANSI_COLOR_DENKR__COLOR_ANSI__DL_LIBS            DENKR__COLOR_ANSI__DL_LIBS
#define ANSI_COLOR_DENKR__COLOR_ANSI__PLUGINS            DENKR__COLOR_ANSI__PLUGINS




#endif /* DENKR_ESSENTIALS__TERMINAL__TERMINAL_COLORS_H_ */
