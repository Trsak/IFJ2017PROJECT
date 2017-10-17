/**
 * @file error_codes.h
 * @author Petr Sopf (xsopfp00)
 * @author Tomas Odehnal (xodehn08)
 * @brief Constants for error codes and function for printing error message.
 */
#ifndef IFJ_ERROR_CODES_H
#define IFJ_ERROR_CODES_H

#include <stdarg.h>

/**
 *
 */
enum ERROR_CODES {
	ERROR_FILE = 5
};

/**
 * @brief Printf error message with error code (number).
 * @param err_code Code (number) of error.
 * @param msg String literal to print.
 * @param ... Any number of parameters, same functionality as printf.
 */
void print_err_msg(ERROR_CODES err_code, const char *msg, ...);

#endif //IFJ_ERROR_CODES_H
