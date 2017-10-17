/**
 * @file error_codes.c
 * @author Tomas Odehnal (xodehn08)
 */

void print_err_msg(ERROR_CODES err_code, const char *msg, ...) {
	fprintf(stderr, "ERROR (%d): ");
	va_list args;
	va_start(args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
	fprintf(stderr, "\n");
}