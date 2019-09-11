#ifndef READFILE_H
#define READFILE_H
#include "structs.h"
#include <stdio.h>
#include <string.h>

extern void readFile(zipTowns arrs, FILE * infile, int * length, int size);

extern void insert_zip_sorted(city * * zips, city * cur_city, int length);
extern void insert_town_sorted(int * towns, int index);

#endif // last line of file
