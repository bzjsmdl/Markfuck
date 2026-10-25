#include "run.h"

#define ZF 0
#define OF 1
#define CF 2
#define SF 3

#define push(n) {\
	size_t step = sizeof(n);\
	memcpy(stack + stack_ptr, &n, step);\
	stack_ptr += step;\
}

#define pop(n) {\
	size_t step = sizeof(n);\
	stack_ptr -= step;\
	memcpy(&n, stack + stack_ptr, step);\
}

int run(char* command, unsigned int length, int isdg){
	int error = NoError;

	size_t pc = 0;
	size_t data_ptr = 0;
	uint8_t flag = 0;
	size_t stack_ptr = 0;
	size_t iv_idx = 0;

	size_t msize = MB(256);

	int8_t* memory = malloc(msize);
	int8_t* stack = memory + MB(128);
	int8_t* interrupt_vector = stack + MB(64);
	memset(memory, 0, msize);
	int start = 0;

	while (pc < length) {
		if (!start && command[pc] != ':') goto next;
		switch (command[pc]) {
			case '\0': error = FileNotEnd; goto end;
			case ';': if (isdg) { printf("[DEBUG]<program end>\n"); } goto end;
			case ',': scanf("%c", memory + data_ptr); break;
			case '<': scanf("%d", memory + data_ptr); break;
			case '>': printf("%d", memory[data_ptr]); break;
			case '.': printf("%c", memory[data_ptr]); break;
			case '#': memory[data_ptr] = command[memory[data_ptr]]; break;
			case '~':
				if (memory[data_ptr] != 0)  {
					memory[data_ptr] = 0;
				}
				else {
					#ifdef __WIN32__ 
						system("cls"); 
					#else 
						system("clear");
					#endif
				}
			break;
			case '&':
				if (stack_ptr == msize - MB(64)) {
					stack_ptr = 0;
					if (isdg) {
						printf("[DEBUG]stack_ptr Overflow!\n", stack_ptr);
						printf("[DEBUG] Continue to run\n");
					}
				}
				push((memory[data_ptr]));
				if (isdg) printf("[DEBUG]<stack_ptr = %lu>\n", stack_ptr);
			break;
			case '*':
				if (stack_ptr == 0) {
					stack_ptr = msize - MB(64);
					if (isdg) {
						printf("[DEBUG]stack_ptr Overflow!\n", stack_ptr);
						printf("[DEBUG] Continue to run\n");
					}
				}
				pop((memory[data_ptr]));
				if (isdg) printf("[DEBUG]<stack_ptr = %lu>\n", stack_ptr);
			break;
			case '?': {
				size_t pc_else = pc += 2, pc_then = pc + 1;
				size_t bidx = memory[data_ptr] % 8;
				if (getb(flag, bidx) == 1) pc = pc_then;
				else pc = pc_else;
				continue;
			} break;
			case '!': {
				size_t pc_else = pc + 2, pc_then = pc + 1;
				size_t bidx = memory[data_ptr] % 8;
				if (getb(flag, bidx) == 0) pc = pc_then;
				else pc = pc_else;
				continue;
			} break;
			case ':':
				start = 1;
				if (isdg) {
					printf("[DEBUG]<program start>\n");
					printf("[DEBUG]<pc = %lu>\n", pc);
				}
			break;
			case '+':
				int gover = memory[data_ptr];
				if (gover == 127) {
					if (isdg) {
						setb(flag, OF, 1);
						printf("[DEBUG]<gover = %d>\n", gover);
						printf("[DEBUG] Continue to run\n");
					}
				}
				memory[data_ptr]++;
				if (memory[data_ptr] == 0) setb(flag, ZF, 1);
				if ((memory[data_ptr] & 0b10000000) != 0) setb(flag, SF, 1);
			break;
			case '-':
				int lover = memory[data_ptr];
				if (lover == -128) {
					if (isdg) {
						setb(flag, CF, 1);
						printf("[DEBUG]<lover = %d>\n", lover);
						printf("[DEBUG] Continue to run\n");
					}
				} 
				memory[data_ptr]--;
				if (memory[data_ptr] == 0) setb(flag, ZF, 1);
				if ((memory[data_ptr] & 0b10000000) != 0) setb(flag, SF, 1);
			break;
			case '{':
				if (data_ptr == 0) {
					data_ptr = msize;
					if (isdg) {
						printf("[DEBUG]data_ptr Overflow!\n", data_ptr);
						printf("[DEBUG] Continue to run\n");
					}
				}
				else {
					data_ptr--;
					if (isdg) printf("[DEBUG]<data_ptr = %lu>\n", data_ptr);
				}
			break;
			case '}':
				if (data_ptr == msize) {
					data_ptr = 0;
					if (isdg) {
						printf("[DEBUG] data_ptr Overflow!\n", data_ptr);
						printf("[DEBUG] Continue to run\n");
					}
				}
				else {
					data_ptr++;
				}
			break;
			case '@': {
				size_t tmp_pc = pc + 1, step = sizeof(tmp_pc);
				memcpy((interrupt_vector + iv_idx * step), &tmp_pc, step);
				iv_idx++;
			} break;
			case '$': {
				size_t step = sizeof(pc), idx = 0, tmp_pc = pc + 1;
				memcpy(&idx, memory + data_ptr, step);
				memcpy(&pc, (interrupt_vector + idx * step), step);
				push(tmp_pc);
				continue;
			} break;
			case '%': {
				memcpy(&pc, memory + data_ptr, sizeof(pc));
				continue;
			} break;
			default: break;
		}
		if (isdg) {
			printf("===========================\n[DEBUG]<data_ptr = %lu>\n", data_ptr);
			printf("[DEBUG]<pc = %lu>\n", pc);
			printf("[DEBUG]<stack_ptr = %lu>\n", stack_ptr);
			printf("Stack: ");
			for (size_t i = 0; i < stack_ptr; i++) printf("%.2X ", stack[stack_ptr]);
			printf("\n");
		}
		next: pc++;
		
	}
	end:
		free(memory);
		return error;
}

void Error(int error, const char* str){
	switch (error) {
		case NoError: return;
		case UnkownArgments: printf("Error: Unkown Argments %s", str); break;
		case CannotOpenFile: printf("Error: Cannot open file \"%s\"\n", str); break;
		case CannotMoveLeftPointer: printf("Error: Cannot Move Left Pointer!"); break;
		case StackOverFlow: printf("Error: Stack of VM OverFlow!"); break;
		case CannotMoveRightPointer: printf("Error: Cannot Move Right Pointer!"); break;
		case UnkownLoop: printf("Error: Unkown Loop!"); break;
		case FileNotEnd: printf("Error: The file doesn't write \";\". So, the program maybe not exit."); break;
		default: printf("Unkown Error.");
	}
	printf("\n");
}