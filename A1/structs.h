/**
* Tyler Thompson
* WMU CS4540 - A1
* Sept 11, 2019
*/
#ifndef STRUCTS_H
#define STRUCTS_H
typedef struct cityStruct { unsigned int zip; char * town; } city;
typedef struct zipTownsStruct {
	int * towns;		// indexs to main array cities sorted by town name
	city * * zips;	// pointers to main array cities sorted by zip
	city * cities;	// main array of cities in order from file not sorted
} zipTowns;

#endif
