//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_UTILITY_H
#define CG_HF1_UTILITY_H

#include <random>
#include <list>
#include "framework.h"
#include "debugmalloc.h"

// Important constants
static int worldSize = 1000; // for atom positions
static int stepNum = 15; // for circle definition (triangle fan count)
static float twicePi = 2 * M_PI; // for circle vertex calc

struct vertex {
    vec2 pos;
    vec4 color; // RGBA - alpha is intensity (Charge/RAND_Max) for atom
};

/* Generate random number with mt19937
 * Will generate between bounds as: [bLower, bUpper]
 * src = https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
 */
int randomInt(int bLower = 1, int bUpper = RAND_MAX);

/* Fast exponentiation
 * src = https://medium.com/techzap/fast-exponentiation-for-competitive-programming-c-2639362698f2
 */
int fastExpo(int base, int exp);

#endif //CG_HF1_UTILITY_H
