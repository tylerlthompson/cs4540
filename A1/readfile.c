/**
* Tyler Thompson
* WMU CS4540 - A1
* Sept 11, 2019
*/
#include "readfile.h"

void readFile(zipTowns arrs, FILE * infile, int * length) {
    int i;
    char town_buf[128][64]; // using buffer instead of reading directly into array. room for improvment.

    for (i=0; fscanf(infile, "%u\t%s", &arrs.cities[i].zip, town_buf[i]) != EOF && i<*length; i++) {
            arrs.cities[i].town = town_buf[i]; // read town out of buffer
            insert_zip_sorted(arrs.zips, &(arrs.cities[i]), i); // insert pointer to zips array
            insert_town_sorted(arrs.towns, arrs.cities, i); // insert index to towns array
    }
    *length = i; // update length to be the length of the file
    print_zipTowns(arrs, *length);
}

/**
 * Inserts a pointer to a city into an already sorted list of pointers to cities. 
 */
void insert_zip_sorted(city * * zips, city * cur_city, int length) {
    int i;
    for (i=length-1; i >= 0 && (*zips[i]).zip > (*cur_city).zip; i--) {
        zips[i+1] = zips[i];
    }
    zips[i+1] = cur_city;
}

/**
 * Insert a town index into a list of town indexs, where the index is the index of the town in the cities array.
 */
void insert_town_sorted(int * towns, city * cities, int index) {
    int i;
    for (i=index-1; i >= 0 && strcmp(cities[towns[i]].town, cities[index].town) > 0; i--) {
        towns[i+1] = towns[i];
    }
    towns[i+1] = index;
}

/**
 * Print a formatted list of all three arrays stored in a zipTowns struct
 */
void print_zipTowns(zipTowns arrs, int size) {
    int i;
    printf("\n\tCities Array\t\t\t\tZips Array\t\tTowns Arrays\nIndex\t|Address\tZip\tTown\t\t|Address\tZip\t|Town\t\tCities Array Index\n");
	for (i=0; i<size; i++) {
        printf("%d\t|%p\t%u\t%-10s\t|%p\t%u\t|%-10s\t%d\n", i, (void *)(&(arrs.cities[i])), arrs.cities[i].zip, arrs.cities[i].town, (void *)(arrs.zips[i]), (*arrs.zips[i]).zip, arrs.cities[arrs.towns[i]].town, arrs.towns[i]);
    }
    printf("\n");
}
