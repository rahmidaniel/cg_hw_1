//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_MOLECULE_H
#define CG_HF1_MOLECULE_H

#include "utility.h"
#include "Atom.h"

class Molecule {
    unsigned int vbo, vao;

    float vel = 0;
    float torque = 0;
    atomNode nodes = atomNode(); // Tree of nodes
    std::vector<vertex> bonds;

public:
    int atomNum; // Between 2-8
    std::vector<Atom*> atoms;

    Molecule();
    void generateAtomTree(atomNode& node, int& atomNumCopy);
    void create();
    void draw();
};


#endif //CG_HF1_MOLECULE_H
