//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_UTILITY_H
#define CG_HF1_UTILITY_H

#include <random>
#include <list>
#include "framework.h"
#include "debugmalloc.h"

/* Generate random number with mt19937
 * Will generate between bounds as: [bLower, bUpper]
 * src = https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
 */
int randomInt(int bLower = 1, int bUpper = RAND_MAX);

/* Fast exponentiation
 * src = https://medium.com/techzap/fast-exponentiation-for-competitive-programming-c-2639362698f2
 */
int fastExpo(int base, int exp);

struct atom {
    float mass;     //Hydrogen -> Atomic mass: 1.008 u  * <n>
    float qCharge;  //Electron -> -1.602 x 10^19 C.      * <n>
    vec4 color; // RGBA - alpha is intensity (Charge/RAND_Max)

    vec2 pos;
    int radius;

    atom();
};

struct atomNode{
    atom self;
    int n; // number of edges (adj)
    std::vector<atomNode> adj;
    std::vector<vec3> edges;
};


#endif //CG_HF1_UTILITY_H
