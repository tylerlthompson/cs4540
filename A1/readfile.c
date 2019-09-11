#include "readfile.h"

void readFile(zipTowns arrs, FILE * infile, int * length) {
    printf("Reading File...\n");
    int i;
    int j;
    int k;
    char town_buf[256][256]; // using buffer instead of reading directly into array. room for improvment.
    for(i=0; fscanf(infile, "%u\t%s", &arrs.cities[i].zip, town_buf[i]) != EOF; i++) {
            arrs.cities[i].town = town_buf[i];
            printf("\nLine: %u Zip: %d Town: %s\n", i, arrs.cities[i].zip, arrs.cities[i].town);

            //insert_zip_sorted(arrs.zips, arrs.cities[i], i);
            //arrs.zips[i] = &(arrs.cities[i]);
            
            j = i - 1;
            //if (j>=0) printf("Will Compare %u with %u\n", (*arrs.zips)[j].zip, arrs.cities[i].zip);
            while (j >= 0 && (*arrs.zips)[j].zip > arrs.cities[i].zip) {
                //printf("%u greater than %u JIndex: %d\n", (*arrs.zips)[j].zip, arrs.cities[i].zip, j);
                printf("Moving %u to index %d\n", (*arrs.zips)[j].zip, (j+1));
                //arrs.zips[j+1] = arrs.zips[j];
                memmove(&arrs.zips[j+1], &arrs.zips[j], i*sizeof(void*));
                //arrs.zips[j+1] = &(arrs.cities[i-j]);

                j--;
            }
            
            arrs.zips[j+1] = &(arrs.cities[i]);


            printf("Placed %u at index %d\n", arrs.cities[i].zip, j+1);
            printf("\nCurrent zip list:\n");
            for(k=0; k<=i; k++) {
                printf("Zip: %u Index: %d\n", (*arrs.zips)[k].zip, k);
            }

    }
    //*length = i;
}

void insert_zip_sorted(city * * zips, city cur_city, int length) {
    //(*zips)[index] = cur_city;
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
    int j = length - 1;
    while (j >= 0 && (*zips[j]).zip > cur_city.zip) {
        zips[j + 1] = zips[j];
        j--;
    }
    zips[j+1] = &cur_city;
}

void insert_town_sorted(int * towns, int index) {
    
}