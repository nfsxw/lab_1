#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
	#define CLEAR_SCREEN system("cls")
#else
	#define CLEAR_SCREEN system("clear")
#endif

void print_menu(void)
{
	printf("\n-------------LAB_1-------------\n");
	printf("1.Create matrix");
	printf("2.Delete matrix")
	printf("3.My matrices");
	printf("4.Add matrices");
	printf("5.Multiply matrices");
	printf("6.Multiply a matrix by a scalar");
	printf("7.Add linear combination to a matrix row");
	printf("8.Exit");

	return;
}

#endif



