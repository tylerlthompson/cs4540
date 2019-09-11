#include "readfile.h"

void readFile(zipTowns arrs, FILE * infile, int * length, int size) {
    int i, j;
    char town_buf[256][256]; // using buffer instead of reading directly into array. room for improvment.

    for(i=0; fscanf(infile, "%u\t%s", &arrs.cities[i].zip, town_buf[i]) != EOF && i<size; i++) {
            arrs.cities[i].town = town_buf[i];
            //printf("\nLine: %u Zip: %d Town: %s\n", i, arrs.cities[i].zip, arrs.cities[i].town);

            insert_zip_sorted(arrs.zips, &(arrs.cities[i]), i);
            insert_town_sorted(arrs.towns, arrs.cities, i);
    }
    *length = i;
}

void insert_zip_sorted(city * * zips, city * cur_city, int length) {
    int i = length - 1;
    while (i >= 0 && (*zips[i]).zip > (*cur_city).zip) {
        zips[i+1] = zips[i];
        i--;
    }
    zips[i+1] = cur_city;
}

void insert_town_sorted(int * towns, city * cities, int index) {
    int i = index - 1;
    while (i >= 0 && strcmp(cities[towns[i]].town, cities[index].town) > 1) {
        towns[i+1] = towns[i];
        i--;
    }
    towns[i+1] = index;
}