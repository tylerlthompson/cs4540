#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "readfile.h"
#include "interactive.h"

void getArrs(zipTowns * arrs, int size) {  // mallocs arrays of size elements
	arrs->towns = malloc(sizeof(int) * size);
	arrs->zips = malloc(sizeof(city *) * size);
	arrs->cities = malloc(sizeof(city) * size);
	// two more lines
	// towns[0] is an int
	// zips[0] is a (citie *)
	// city[0] is a citie
}

extern int getArgsInfoOpenFile(int argc, char * argv[], FILE * * infile, int * size); // 0 ok, !0 error

int main(int argc, char * argv[]) {
	zipTowns arrs; // all the arrays in one struct
	int length = 0;		// current count of items in arrays 
	
	FILE * infile = NULL;

	int ret, size, j = 0;
	
	if (getArgsInfoOpenFile(argc, argv, &infile, &size))
		printf("error in command line arguments\n");
	else {
		getArrs(&arrs, size);
		
		readFile(arrs, infile, &length, size);
		fclose(infile);
		doInteractive(arrs, length);
	} // end else no error in command line
	printf("press any key: ");
	getc(stdin);
	return 0;
}

int getArgsInfoOpenFile(int argc, char * argv[], FILE ** infile, int * size) // 0 ok else !0 error
{
	int retval = 0;

	if(argc != 3) { // test for correct arguments number 3: exename, filename, size
		retval = 1;
	}
	else { // attempt to open file
		*infile = fopen(argv[1], "r");
		*size = atoi(argv[2]);

		if(!*infile) { // handle path not found
			printf("\nFailed to open file: %s Check that your path is correct.\n", argv[1]);
			retval = 1;
		} 
	}
	// return file and size in parameters or error
	return retval;
}
