#pragma once
#include "Functions.h"

int decodedFileLenght = 0;
unsigned short alphabetSize = 0;
unsigned short nbits;
string text = "";

HeaderNo * tempRaiz;
Header * totalHeader[256];
Header * header;
PriorityQueue simpleFrontier;
list<Definition *> Dictionary;