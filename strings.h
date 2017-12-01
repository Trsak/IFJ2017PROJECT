/**
 * @project IFJ17
 * @file strings.h
 * @author Jan Bartosek (xbarto92)
 * @brief Defines struct string and operations with it.
 */

#ifndef IFJ_STRINGS_H
#define IFJ_STRINGS_H

typedef struct {
	char *str;        // string that ends with /0 char
	int length;        // string length
	int allocSize;    // size of allocated memory
} string;

#define STR_LEN_INC 8 // primary size of allocation in bytes

#define STR_ERROR   1
#define STR_SUCCESS 0

/**
 * @param s - this is a pointer for a string structure to operate with.
 *
 * Initialise the structure and sets it's default values.
 */
int strInit(string *s);

/**
 * @param s - this is a pointer for a string structure to operate with.
 *
 * Destroys the whole structure
 */
void strFree(string *s);

/**
 * @param s - this is a pointer for a string structure to operate with.
 *
 * Sets the structure to it's default values as it comes after initialisation.
 */
void strClear(string *s);

/**
 * @param s - this is a pointer for a string structure to operate with.
 * @param c - character to be added to the structure.
 * @return - 1 if any problem occurred, 0 if not.
 *
 * Adds a char 'c' on the end of the char *str and resize the string length if needed.
 */
int strAddChar(string *s1, char c);

/**
 * @param s - this is a pointer for a string structure to operate with.
 * @return - 1 if any problem occurred, 0 if not.
 *
 * Resize the string length twice. This function is called when the string is filled with hexadecimal number and converted to decimal. Otherwise it might cause a problem.
 */
int strSizeUp(string *s1);

#endif //UNTITLED_STRING_H