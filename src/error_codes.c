/**
 * @file error_codes.c
 * @author Tomas Odehnal (xodehn08)
 */

#include "error_codes.h"

void printErrMsg(enum ERROR_CODES errCode, const char *msg, ...) {
	fprintf(stderr, "ERROR (%d): ", errCode);
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	fprintf(stderr, "\n");
}