/*usage ./a.out < file > output.bf or typing into terminal the chars directly

Made to be simple.
*/

#include <stdio.h>
#define MAXCHARS 100000

int main()
{
    int chars[MAXCHARS], c, i, j;
    for (i = 0; (c = getchar()) != EOF && i < MAXCHARS; i++)
        chars[i] = c;
    chars[i + 1] = 0;
    for (i = 0; chars[i]; i++) {
        for (j = 0; j < chars[i]; j++) 
            putchar('+');
		putchar('.');
        putchar('>');
    }
    return 0;
}
