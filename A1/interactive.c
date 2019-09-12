#include "interactive.h"

void doInteractive(zipTowns arrs, int length) {
    unsigned int input, search_zip;
    int bin_search;
    char search_town[64];

    // prompt user for command
    printf("1.\tQuit\n2.\tSearch for zip\n3.\tSearch for town\nCommand: ");
    scanf("%u", &input);

    // process user command 
    switch (input) {
        case 1: // quit
            printf("Quitting...\n");
            break;
        case 2: // search for zip
            // get zip from user
            printf("Enter Zip to Search For: ");
            scanf("%u", &search_zip);

            // search the zips array for the specified zip
            bin_search = binary_search_zip(arrs.zips, length, search_zip);

            // handle the search results 
            if(bin_search == -1) printf("Zip not found\n");
            else printf("Zip found at index: %d\n", bin_search);

            break;
        case 3: // search for town
            // get town from user
            printf("Enter Town to Search For: ");
            scanf("%s", search_town);

            // search the towns array for the specified town
            bin_search = binary_search_town(arrs.cities, arrs.towns, search_town, length);

            // handle the search results
            if (bin_search == -1) printf("Town not found\n");
            else printf("Town found at index: %d\n", bin_search);
            
            break;
        default:
            printf("Unknown input. Quitting...\n");
            break;
    }
}

int binary_search_zip(city * * zips,  unsigned int zip, int length) {
    int found = -1, top = 0, btm = length, mid;
    while (found == -1 && top <= btm) {
        mid = (btm + top) / 2;
        if ((*zips[mid]).zip == zip) {
            found = mid;
        }
        else if (zip < (*zips[mid]).zip) {
            btm = mid - 1;
        }
        else {
            top = mid + 1;
        }
        
    }
    return found;
}

int binary_search_town(city * cities, int * towns, char * town, int length) {
    int found = -1, top = 0, btm = length, mid, str_cmp;
    while (found == -1 && top <= btm) {
        mid = (btm + top) / 2;
        str_cmp = strcmp(town, cities[towns[mid]].town);
        printf("Key: %s Value: %s Str_cmp: %d\n", town, cities[towns[mid]].town, str_cmp);
        if (str_cmp == 0) {
            found = mid;
        }
        else if (str_cmp < 0) {
            btm = mid - 1;
        }
        else {
            top = mid + 1;
        }
        
    }
    return found;
}
