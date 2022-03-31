//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_MOLECULE_H
#define CG_HF1_MOLECULE_H

#include "utility.h"

class Molecule {
    float vel;
    float torque;
    atomNode atoms; // Tree of atoms

public:
    int atomNum; // Between 2-8
    Molecule();
    void calculateVertices(float* res);
    void generateAtomTree(atomNode& node, int& atomNumCopy);

    std::vector<float> points;
};


#endif //CG_HF1_MOLECULE_H
