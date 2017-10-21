/**
 * @file parser.h
 * @author Roman Bartl (xbartl06)
 * @brief  TODO
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include "scanner.h"
#include "lexems.h"
#include "derivation_tree.c"
#include "error_codes.c"


bool program();

bool functions();

bool main_body();

bool main_body_it();

bool statement();

bool function_follow(int token);

bool function_first(int token);

bool function_header();

bool function_it();

bool function_it_first(int token);

bool function_as_first(int token);

bool function_as();

bool function_next();

bool function_end();

bool declare_params();

bool declare_params_next();

bool declare_params_follow(int token);

bool data_type();

bool statement();

bool main_body();

bool main_body_it();

bool end();