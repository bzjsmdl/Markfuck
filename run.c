#include "run.h"
int LoopStart = 0;
int run(char* buffer, unsigned int lengh, int isdg){
    int LoopCount = 0;
    int isLoop = 0;
    int Stack[1024] = {0};
    int StackOffset = 0;

    int8_t* a = malloc(30000);
    for (size_t j = 0; j < 30000; j++){
        a[j] = 0;
    }
    
    int i = 0, aidx = 0, s = 0;
    while (i < lengh){
        if (!s && buffer[i] == ':') {
            s = 1;
            if (isdg) {
                printf("[DEBUG]<program start>\n");
                printf("[DEBUG]<i = %d, nowchr = %c>\n", i, buffer[i]);
            }
        }
        if (s && buffer[i] == ';') {
            if (isdg) {
                printf("[DEBUG]<program end>\n");
                printf("[DEBUG]<i = %d, nowchr = %c>\n", i, buffer[i]);
            }
            break;
        }
        
        if (s && buffer[i] == '+') {
            int gover = a[aidx];
            if (gover++ > 127) {
                if (isdg) printf("[DEBUG]<gover = %d>\n", gover);
                return AboveOverFlowError;
            }
            a[aidx]++;
        }
        if (s && buffer[i] == '-') {
            int lover = a[aidx];
            if (lover-- < -128) {
                if (isdg) printf("[DEBUG]<lover = %d>\n", lover);
                return BelowOverFlowError;
            } 
            a[aidx]--;
        }
        if (s && buffer[i] == '{') {
            if (aidx == 0) return CannotMoveLeftPointer;
            aidx--;
        }
        if (s && buffer[i] == '}') {
            if (aidx > 30000) return CannotMoveRightPointer;
            aidx++;
        }
        if (s && buffer[i] == 'r') {
            printf("%d", a[aidx]);
            if (isdg) printf("[DEBUG]<Read: %d>\n", a[aidx]);
        }
        if (s && buffer[i] == '.') {
            printf("%c", a[aidx]);
            if (isdg) printf("[DEBUG]<Print: %c>\n", a[aidx]);
        }
        if (s && buffer[i] == '[') {
            
            LoopCount = a[aidx];
            isLoop = 1;

            //Stack[StackOffset] = i;
            //StackOffset++;
            LoopStart = i;
            if (isdg){
                printf("[DEBUG]<Start Character: %c>\n", buffer[LoopStart]);
                printf("[DEBUG]<Start: LoopCount = %d>\n", LoopCount);
                printf("[DEBUG]<Start: i = %d>\n", i + 1);
            }
        }
        if (s && buffer[i] == ']') {
            int LC = LoopCount;
            LC--;
            if (!isLoop || LoopCount < 0) return UnkownLoop;
            if (LoopCount <= 0 || LC-- <= 0) {
                i++;
                continue;
            }
            else {
                if (isdg) {
                    printf("[DEBUG]<End: i = %d>\n", i);
                    printf("[DEBUG]<End Character: %c>\n", buffer[i - 1]);
                }
                LoopCount--;
                if (isdg) printf("[DEBUG]<LoopCount = %d>\n", LoopCount);
                //i = Stack[StackOffset];
                //StackOffset--;
                i = LoopStart;
            }
        }
        if (buffer[i] == '\0') return FileNotEnd;
        i++;
    }
    return 0;
}
void Error(int error){
    if (error = AboveOverFlowError) printf("Error: Above OverFlow!");
    else if (error = CannotMoveLeftPointer) printf("Error: Cannot Move Left Pointer!");
    else if (error = BelowOverFlowError) printf("Error: Below OverFlow!");
    else if (error = StackOverFlow) printf("Error: Program(VM) Stack OverFlow!");
    else if (error = CannotMoveRightPointer) printf("Error: Cannot Move Right Pointer!");
    else if (error = UnkownLoop) printf("Error: Unkown Loop!");
    else if (error = FileNotEnd) printf("Error: The file doesn't write \";\". So, the program maybe not exit.");
}