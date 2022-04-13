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
    float rotationAngle;
    float theta; // Moment of inertia

    // DEBUG MASS
    Atom m;

    mat4 transMat;
    vertex center;
    atomNode nodes; // Tree of nodes
    std::vector<vertex> bonds;

    // Physics
    // src = https://www.youtube.com/watch?v=Z2XN8NrvhoQ
    // ALSO SHIFTS ATOM POSITIONS
    void massCenter();
    // Generate atoms and bonds
    void generateAtomTree(atomNode& node, int& atomNumCopy);

    // Prufer tree generation
    // src = http://www-math.ucdenver.edu/%7Ewcherowi/courses/m4408/gtaln7.html
    void generateTree();
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
    // Physics
    void react2Molecule(const Molecule& molecule, float dt);
    void update();

    ~Molecule();
};


#endif //CG_HF1_MOLECULE_H
