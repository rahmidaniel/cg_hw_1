//
// Created by danie on 3/30/2022.
//

#ifndef CG_HF1_MOLECULE_H
#define CG_HF1_MOLECULE_H

#include "utility.h"
#include "Atom.h"

class Molecule {
    unsigned int vbo, vao;

    vec2 vel;
    float M;
    vec2 w;
    vec2 a;

    mat4 transMat;
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
    // Calculates bond vectors, pushes them into 'bonds'
    void calculateBonds();
    // OpenGL
    void create();
    void draw();
    void update();
    // Physics
    void react2Molecule(const Molecule& molecule, float dt);

    ~Molecule();
};


#endif //CG_HF1_MOLECULE_H
