/**
 * @project IFJ17
 * @file optimalization.c
 * @author Petr Sopf (xsopfp00)
 * @brief Functions used for optimalization
 */

#include <math.h>

int floatToInt(double f) {
    double a = f + 0.5;

    if ((a == ceil(a)) && (a == floor(a))) {
        int b = (int) a;
        if (b % 2 == 0) {
            return (int) ceil(f);
        }

        return (int) floor(f);
    }

    return (int) round(f);
}