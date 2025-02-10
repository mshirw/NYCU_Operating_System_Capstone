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

void memset(void *dest, uint8 value, uint32 len)
{
	uint32 *tar = dest;

	for(uint32 i = 0; i < len; i++)
		tar[i] = ((value) | (value << 8) | (value << 16) | (value << 24));
		
}

void memcpy(void *dest, void *src, uint32 len)
{
	uint32 *tar = dest;
	uint32 *source = src;

	for(uint32 i = 0; i < len; i++)
		tar[i] = source[i];
}