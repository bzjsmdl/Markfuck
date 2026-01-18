#ifndef MARKFUCK_RUN_H
#define MARKFUCK_RUN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum Error{
    UnkownArgments,
    CannotOpenFile,
    AboveOverFlowError,
    BelowOverFlowError,
    CannotMoveLeftPointer,
    CannotMoveRightPointer,
    StackOverFlow,
    UnkownLoop,
    FileNotEnd

};


int run(char* buffer, unsigned int lengh, int isDebug);
void push(int Data);
void pop(int* pointer);
void Error(int error);

#endif