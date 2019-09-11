#ifndef READFILE_H
#define READFILE_H
#include "structs.h"

extern void readFile(zipTowns arrs, FILE * infile, int * length);
extern void insert_zip_sorted(city * * * zips, city town, int index);
extern void insert_town_sorted(int * towns, int index);

void readFile(zipTowns arrs, FILE * infile, int * length) {
    printf("Reading File...\n");
    int i;
    char town_buf[256]; // using buffer instead of reading directly into array. room for improvment.
    for(i=0; fscanf(infile, "%u\t%s", &arrs.cities[i].zip, town_buf) != EOF; i++) {
            arrs.cities[i].town = town_buf;
            printf("Line: %u Zip: %d Town: %s\n", i, arrs.cities[i].zip, arrs.cities[i].town);

            insert_zip_sorted(&arrs.zips, arrs.cities[i], i);
            
            
    }
    //*length = i;
}

void insert_zip_sorted(city * * * zips, city cur_city, int index) {
    (*zips)[index] = &cur_city;
    // int i = index - 1;
    
    // if(!index) {
    //     zips[index] = &cur_city;
    //     printf("ZERO CurZip: %u LastZip: %u Index: %d I: %d\n", cur_city.zip, (*zips[0]).zip, index, i);
    // }
    // else if (cur_city.zip > (*zips[i]).zip) {
    //     printf("GREATER CurZip: %u LastZip: %u Index: %d I: %d\n", cur_city.zip, (*zips[i]).zip, index, i);
    //     zips[index] = zips[i];
    //     insert_zip_sorted(zips, cur_city, i);
    // }
    // else {
    //     printf("ELSE CurZip: %u LastZip: %u Index: %d I: %d\n", cur_city.zip, (*zips[i]).zip, index, i);
    //     zips[index] = &cur_city;
    // }
}

void insert_town_sorted(int * towns, int index) {
    
}

#endif // last line of file
