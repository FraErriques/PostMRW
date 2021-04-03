#ifndef RWTXTFILE_H_INCLUDED
#define RWTXTFILE_H_INCLUDED

#include <iostream>
#include <list>
#include <string>
#include <fstream>

using namespace std;

int RWtxtfile_demo_();
bool writeFileByWords( double *data, int hm);
bool readFileByWords();
bool readFileByLines();
int readFileByChars();

#endif // RWTXTFILE_H_INCLUDED
