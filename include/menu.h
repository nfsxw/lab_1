#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#define CLEAR_SCREEN system("cls")
#else
	#define CLEAR_SCREEN system("clear")
#endif

void menu(void);
void print_menu(void);

#endif



