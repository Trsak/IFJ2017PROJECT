/**
 * @file error_codes.h
 * @author Petr Sopf (xsopfp00)
 * @author Tomas Odehnal (xodehn08)
 * @brief Constants for error codes and function for printing error message.
 */
#ifndef IFJ_ERROR_CODES_H
#define IFJ_ERROR_CODES_H

#include <stdarg.h>
#include <stdio.h>
#include "garbage_collector.h"

/**
 *
 */
enum ERROR_CODES {
	ERROR_SCANNER = 1, /* Lexical analysis error */
	ERROR_SYNTAX = 2, /* Syntax error */
	ERROR_PROG_SEM = 3, /* Semantic error in program (undefined function/variable,...) */
	ERROR_TYPE_SEM = 4, /* Semantic error of type compatibility in arithmetic, string, relational,... expressions */
	ERROR_OTHER_SEM = 6, /* Other semantic errors */
	ERROR_INTERNAL = 99 /* Internal errors (opening files, allocating memory,...) */
};

/**
 * @brief Printf error message with error code (number) and exit program.
 * @param err_code Code (number) of error.
 * @param msg String literal to print.
 * @param ... Any number of parameters, same functionality as printf.
 */
void printErrAndExit (enum ERROR_CODES errCode, const char *msg, ...);

#endif //IFJ_ERROR_CODES_H
