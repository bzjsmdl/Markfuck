#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "run.h"

char* buffer;
unsigned int lengh;

int main(int argc, char const *argv[]){
    int isdg = 0;
    int error = 0;
    if (argc == 3){
        if (strcmp(argv[2], "-d") == 0) isdg = 1;
        else {
            error = UnkownArgments;
            printf("Error: Unkown Argments %s", argv[2]);
            return 1;
        }
    }
    FILE* src = fopen(argv[1], "r");
    if (src == NULL){
        error = CannotOpenFile;
        printf("Error: Cannot open file \"%s\"\n", argv[1]);
        return 1;
    }
    
    fseek(src, 0, SEEK_END);
    lengh = ftell(src);
    fseek(src, 0, SEEK_SET);
    if (isdg) printf("[Message]<The file's lengh: %d>\n", lengh);
    
    buffer = (char*)malloc(lengh + 1);
    fread(buffer, 1, lengh, src);
    buffer[lengh] = '\0';
    fclose(src);

    if (isdg) printf("[Message]<The File: %s>\n\n\n\n\n", buffer);
    error = run(buffer, lengh, isdg);
    if (error != 0){
        Error(error);
        return 1;
    }
    return 0;
}