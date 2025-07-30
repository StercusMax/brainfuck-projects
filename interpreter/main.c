#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>


#define ARRAYLENGTH 30000
#define STACKSIZE 10000

char * code = NULL;

int verifyloops();
void execute();

volatile int interrupt = 0;

void intHandler(int dummy) {
    interrupt = 1;
}

int main(int argc, char **argv)
{
    if (argc == 1) {perror("Need a file to interpret\n"); return 0;} 
    else if (argc > 2) {printf("You need to use ./bfi file\n"); return 0;}
    
    signal(SIGINT, intHandler);
    size_t length;
    FILE * f = fopen(argv[1], "r");
    if (!f) {fprintf(stderr, "Couldn't open file %s\n", argv[1]); return 0;}

    fseek(f, 0, SEEK_END);
    length = ftell (f);
    fseek(f, 0, SEEK_SET);
    code = malloc(length + 1);
    if (!code) {fclose(f); perror("Error when allocating memory to get the text of the file\n"); return 0;}
    fread(code, 1, length, f);
    fclose(f);
    code[length] = '\0';
    if (verifyloops() == -1) {free(code); perror("Not closed/opened loops\n"); return 0;};
    execute();
    
    free(code);
    return 0;
}

int verifyloops()
{
    int i, openedloop = 0;
    for (i = 0; code[i]; i++) {
        if (code[i] == '[')
            openedloop++;
        else if (code[i] == ']') {
        	if (!openedloop)
        		return -1;
        	else
        		openedloop--;	
        }
    }
    if (openedloop)
        return -1;
    return 0;
}

void execute() //need to check verifyloop() first
{
    char data[ARRAYLENGTH] = { }, *pdata = data;
    int stack[STACKSIZE], *pstack = stack;
    int c, i, loop;
    for (i = 0; code[i]; i++) {
    	if (interrupt)
    		return;
        switch (code[i])
        {
        case '+': (*pdata)++; break;
        case '-': (*pdata)--; break;
        case '>': pdata++; break;
        case '<': pdata--; break;
        case '.': putchar(*pdata); break;
        case ',': c = getchar(); if (c != EOF) *pdata = c; break;
        case '[':
            if (!(*pdata)) {
                for (loop = 1, i++; loop; i++)
                    if (code[i] == '[') loop++;
                    else if (code[i] == ']') loop--;
                i--;
            }
            else
                *pstack++ = i;
            break;
        case ']':
            if (*pdata) i = *(pstack - 1);
            else pstack--;
            break;
        }
    }
}
