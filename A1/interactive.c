#include "interactive.h"

void doInteractive(zipTowns arrs, int length) {
    unsigned int input;
    printf("1.\tQuit\n2.\tSearch for zip\n3.\tSearch for town\nCommand: ");
    scanf("%u", &input);
    switch (input) {
        case 1: // quit
            printf("Quitting...\n");
            break;
        case 2: // search for zip

            break;
        case 3: // search for town
        
            break;
        default:
            printf("Unknown input. Quitting...\n");
            break;
    }
}
