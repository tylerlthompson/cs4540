/**
* Tyler Thompson
* WMU CS4540 - A1
* Sept 11, 2019
*/
#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

extern void doInteractive(zipTowns arrs, int length);

extern int binary_search_zip(city * * zips, unsigned int zip, int length);
extern int binary_search_town(city * cities, int * towns, char * town, int length);

#endif // last line of file
