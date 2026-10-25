#ifndef MARKFUCK_RUN_H
#define MARKFUCK_RUN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define strequ(s, S) (strcmp(s, S) == 0)

#define KB(n) (size_t)(1024 * n)
#define MB(n) (size_t)(KB(1024) * n)
#define GB(n) (size_t)(MB(1024) * n)

// set bit
#define setb(b, i, n) (b |= (n << i))
// get bit
#define getb(b, i) ((b & (1 << i)) != 0)

enum ErrorCode {
    NoError,
    UnkownArgments,
    CannotOpenFile,
    CannotMoveLeftPointer,
    CannotMoveRightPointer,
    StackOverFlow,
    UnkownLoop,
    FileNotEnd
};


int run(char* buffer, unsigned int lengh, int isDebug);
void push(int Data);
void pop(int* pointer);
void Error(int error, const char* str);

#endif