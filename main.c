#include "run.h"

int main(int argc, const char **argv){
    int isdg = 0, error = 0;
    const char* str;
    if (argc == 3){
        if (strequ(argv[2], "-d") || strequ(argv[2], "--debug")) isdg = 1;
        else {
            str = argv[2];
            error = UnkownArgments;
            goto err;
        }
    }
    FILE* src = fopen(argv[1], "r");
    if (src == NULL){
        str = argv[1];
        error = CannotOpenFile;
        goto err;
    }
    
    unsigned long long int nfo = ftell(src);
    fseek(src, 0, SEEK_END);
    unsigned long long int lengh = ftell(src);
    fseek(src, 0, SEEK_SET);
    if (isdg) printf("[Message]<The file's lengh: %d>\n", lengh);
    
    char* text = (char*)malloc(lengh + 1);
    fread(text, 1, lengh, src);
    text[lengh] = '\0';
    fclose(src);

    error = run(text, lengh, isdg);
    if (error != NoError) goto err;

    return 0;
    err:
        Error(error, str);
        return 1;
}