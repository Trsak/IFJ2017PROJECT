/**
 * @file error_codes.c
 * @author Tomas Odehnal (xodehn08)
 */

#include "error_codes.h"

void print_err_msg(enum ERROR_CODES err_code, const char *msg, ...) {
	fprintf(stderr, "ERROR (%d): ", err_code);
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	fprintf(stderr, "\n");
}