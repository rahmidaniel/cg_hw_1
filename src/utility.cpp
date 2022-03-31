//
// Created by danie on 3/31/2022.
//

#include <iostream>
#include "utility.h"

int randomInt(int bLower, int bUpper){
    std::random_device device;
    std::mt19937 rng(device());
    std::uniform_int_distribution<std::mt19937::result_type> distribution(bLower, bUpper);
    return (int)distribution(rng);
}
int fastExpo(int base, int exp){
    if (exp == 0) return 1;
    if (exp == 1) return base;
    int oneHalf = fastExpo(base,exp / 2);
    return oneHalf * oneHalf * fastExpo(base,exp % 2);
}

atom::atom() {
    pos = vec2(randomInt(-10,10), randomInt(-10,10));
    radius = randomInt(1, 6);

    int intensity = randomInt(); // TODO: ask if separate for charge and mass
    mass = 1.008f * intensity;
    qCharge = -1.602f * fastExpo(10, 19) * intensity;

    // calculate color intensity
    float alphaIntensity = 255 * ((float)intensity/RAND_MAX);
    if(qCharge > 0) color = vec4(255,0,0, alphaIntensity); // red
    else color = vec4(0,0,255, alphaIntensity); // blue
}
