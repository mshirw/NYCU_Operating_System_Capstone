#ifndef	_STR_H
#define	_STR_H

#include <stddef.h>

size_t utils_strlen(const char *s);
char *utils_strcpy(char* dst, const char *src);
char *utils_strdup(const char *src);
void memset(void *dest, uint8 value, uint32 len);
void memcpy(void *dest, void *src, uint32 len);

#endif