//
// Created by danie on 3/31/2022.
//

#include <iostream>
#include "utility.h"

int randomInt(int bUpper, int bLower){
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
