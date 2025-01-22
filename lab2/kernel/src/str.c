#include "memalloc.h"
#include "str.h"

//with null-terminator -> "hello" return 6
size_t utils_strlen(const char *s) {
    size_t i = 0;
	while (s[i]) i++;
	return i+1;
}

char *utils_strcpy(char* dst, const char *src) {
	char *save = dst;
	while((*dst++ = *src++));
	return save;
}

char *utils_strdup(const char *src) {
	size_t len = utils_strlen(src);
	char *dst = malloc(len);
    if (dst == NULL) { // Check if the memory has been successfully allocated
        return NULL;
    }
    utils_strcpy(dst, src); // Copy the string
    return dst;
}