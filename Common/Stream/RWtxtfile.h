#ifndef RWTXTFILE_H_INCLUDED
#define RWTXTFILE_H_INCLUDED


#include <list>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

int RWtxtfile_demo_();
bool writeFileByWords( double *data, int hm);
bool writeVectorFieldR2Affine( double *ApplicationPoint, double *FreeBound, int hm);
bool readFileByWords();
bool readFileByLines();
int readFileByChars();
void tryReadForewards();
void tryReadBackwards();// platform dependent :  NB. works on Linux but not on Windows.

#endif // RWTXTFILE_H_INCLUDED
