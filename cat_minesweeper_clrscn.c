#include <stdio.h>

#define SCREEN 80

void clrscn() {

	int clr;

	for( clr=0; clr<SCREEN; clr++ )
		putchar('\n');
}