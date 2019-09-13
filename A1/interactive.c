/**
* Tyler Thompson
* WMU CS4540 - A1
* Sept 11, 2019
*/
#include "interactive.h"

void doInteractive(zipTowns arrs, int length) {
    unsigned int search_zip;
    int bin_search;
    char input_buf[32];

    // prompt user for command
    printf("1.\tQuit\n2.\tSearch for zip\n3.\tSearch for town\nCommand: ");
    scanf(" %s", input_buf);

    // process user command 
    switch (input_buf[0]) {
        case '1': // quit
            printf("Quitting...\n");
            break;
        case '2': // search for zip
            // get zip from user
            printf("Enter Zip to Search For: ");
            scanf(" %s%*c", input_buf);
            search_zip = atoi(input_buf);

            // search the zips array for the specified zip
            bin_search = binary_search_zip(arrs.zips, search_zip, length);

            // handle the search results 
            if(bin_search == -1) printf("Zip not found.\n");
            else printf("Zip found at index %d of Zips array.\n", bin_search);

            break;
        case '3': // search for town
            // get town from user
            printf("Enter Town to Search For: ");
            scanf(" %s%*c", input_buf);

            // search the towns array for the specified town
            bin_search = binary_search_town(arrs.cities, arrs.towns, input_buf, length);

            // handle the search results
            if (bin_search == -1) printf("Town not found.\n");
            else printf("Town found at index %d of Towns array.\n", bin_search);
            
            break;
        default:
            printf("Unknown input. Quitting...\n");
            break;
    }

    // deconstruct arrays
    free(arrs.cities);
    free(arrs.towns);
    free(arrs.zips);
}

/**
 * Do a binary search for a zip in an array of pointers to zips that is already sorted.
 */
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

/**
 * Do a binary search for a town in an array of town indexs that indicate their index in the cities array. 
 * Assumes the towns index array is alrady sorted by town.
 */
int binary_search_town(city * cities, int * towns, char * town, int length) {
    int found = -1, top = 0, btm = length, mid, str_cmp;
    while (found == -1 && top <= btm) {
        mid = (btm + top) / 2;
        str_cmp = strcmp(town, cities[towns[mid]].town);
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
