#include "readfile.h"

void readFile(zipTowns arrs, FILE * infile, int * length, int size) {
    int i;
    char town_buf[256][256]; // using buffer instead of reading directly into array. room for improvment.

    for(i=0; fscanf(infile, "%u\t%s", &arrs.cities[i].zip, town_buf[i]) != EOF && i<size; i++) {
            arrs.cities[i].town = town_buf[i];
            //printf("\nLine: %u Zip: %d Town: %s\n", i, arrs.cities[i].zip, arrs.cities[i].town);

            insert_zip_sorted(arrs.zips, &(arrs.cities[i]), i);
            insert_town_sorted(arrs.towns, arrs.cities, i);
    }
    *length = i;
    print_zipTowns(arrs, i);
}

void insert_zip_sorted(city * * zips, city * cur_city, int length) {
    int i;
    for(i=length-1; i >= 0 && (*zips[i]).zip > (*cur_city).zip; i--) zips[i+1] = zips[i];
    zips[i+1] = cur_city;
}

void insert_town_sorted(int * towns, city * cities, int index) {
    int i;
    for(i=index-1; i >= 0 && strcmp(cities[towns[i]].town, cities[index].town) > 1; i--) towns[i+1] = towns[i];
    towns[i+1] = index;
}

void print_zipTowns(zipTowns arrs, int size) {
    int i;
    printf("\n\tCities Array\t\t\t\tZips Array\t\tTowns Arrays\nIndex\t|Address\tZip\tTown\t\t|Address\tZip\t|Town\t\tCities Array Index\n");
	for(i=0; i<size; i++) printf("%d\t|%p\t%u\t%-10s\t|%p\t%u\t|%-10s\t%d\n", i, (void *)(&(arrs.cities[i])), arrs.cities[i].zip, arrs.cities[i].town, (void *)(arrs.zips[i]), (*arrs.zips[i]).zip, arrs.cities[arrs.towns[i]].town, arrs.towns[i]);
    printf("\n");
}
