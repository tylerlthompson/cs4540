/**
* Tyler Thompson
* WMU CS4540 - A1
* Sept 11, 2019
*/
#ifndef READFILE_H
#define READFILE_H

#include <stdio.h>
#include <string.h>
#include "structs.h"

extern void readFile(zipTowns arrs, FILE * infile, int * length);

extern void insert_zip_sorted(city * * zips, city * cur_city, int length);
extern void insert_town_sorted(int * towns, city * cities, int index);
extern void print_zipTowns(zipTowns arrs, int size);

#endif // last line of file
