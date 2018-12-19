#include <stdio.h>

char *remove_exe_suffix(char *str) {

	int n = 0;
	while (*str != '\0') {
		str++;
		if (str[n] == '.' && str[n + 1] == 'e' && str[n + 2] == 'x' && str[n + 3] == 'e') {
			str[n] = '\0';
			break;
		}
		n++;
	}

	if (n != 0) {
		return str;
	}
	else {
		return NULL;
	}
}