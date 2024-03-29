/**
 * @project IFJ17
 * @file strings.c
 * @author Jan Bartosek (xbarto92)
 * @brief Operations with string structure.
 */
#include "strings.h"
#include <malloc.h>
#include <string.h>
#include "garbage_collector.h"

/**
 * @copydoc strInit
 */
int strInit(string *s) {
	if ((s->str = (char *) gcmalloc(STR_LEN_INC)) == NULL) {
		return STR_ERROR;
	}
	s->str[0] = '\0';
	s->length = 0;
	s->allocSize = STR_LEN_INC;
	return STR_SUCCESS;
}

/**
 * @copydoc strFree
 */
void strFree(string *s) {
	gcfree(s->str);
}

/**
 * @copydoc strClear
 */
void strClear(string *s) {
	strFree(s);
	strInit(s);
}

/**
 * @copydoc strAddChar
 */
int strAddChar(string *s1, char c) {
	if (s1->length + 1 >= s1->allocSize) {
		if ((s1->str = (char *) gcrealloc(s1->str, s1->length + STR_LEN_INC)) == NULL) {
			return STR_ERROR;
		}
		s1->allocSize = s1->length + STR_LEN_INC;
	}
	s1->str[s1->length] = c;
	s1->length++;
	s1->str[s1->length] = '\0';
	return STR_SUCCESS;
}

/**
 * @copydoc strSizeUp
 */
int strSizeUp(string *s1) {
	if ((s1->str = (char *) gcrealloc(s1->str, s1->length * 2)) == NULL) {
		return STR_ERROR;
	}
	s1->allocSize = s1->length * 2;
	return STR_SUCCESS;
}