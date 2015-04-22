#include <stdio.h>
#include <string.h>

int main() {
	char str[] = "Hello, World";
	char *tok = strtok(str, ",");
	
	printf("Before: %s\n", str);
	tok[0] = 'B';
	printf("After: %s\n", str);

	return 0;
}
