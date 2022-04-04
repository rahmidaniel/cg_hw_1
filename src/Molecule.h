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

    mat4 MVP;
    vertex center;
    atomNode nodes; // Tree of nodes
    std::vector<vertex> bonds;

    // Physics
    // src = https://www.youtube.com/watch?v=Z2XN8NrvhoQ
    // ALSO SHIFTS ATOM POSITIONS - makes atom center the origin
    void massCenter();
    // Generate atoms and bonds
    void generateAtomTree(atomNode& node, int& atomNumCopy);
public:
    int atomNum; // Between 2-8
    std::vector<Atom*> atoms;

    Molecule();
    // Resets properties
    void init();
    // OpenGL
    void create();
    void draw();
    void setMVP(mat4 mvp){ MVP = mvp;}
    // Physics
    void react2Molecule(const Molecule& molecule);
};


#endif //CG_HF1_MOLECULE_H
