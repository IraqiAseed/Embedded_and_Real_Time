#ifndef TESTS_H_
#define TESTS_H_

#include "../include/dynamicVector.h"
#include <stdio.h>
#include <stdlib.h>

int RunVectorTests();
void PrintResult(const char *testName, int condition);
int PrintInt(void *element, size_t index, void *context);
int SumInt(void *element, size_t index, void *context);
int StopAtIndex2(void *element, size_t index, void *context);
void DestroyInt(void *element);


#endif // TESTS_H_