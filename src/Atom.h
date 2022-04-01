//
// Created by danie on 3/31/2022.
//

#ifndef CG_HF1_ATOM_H
#define CG_HF1_ATOM_H


#include "utility.h"

// src = https://stackoverflow.com/questions/14486291/how-to-draw-line-in-opengl
class Atom {
    unsigned int vao, vbo;	// vertex array object, vertex buffer object
    std::vector<vertex> vertices;
public:
    vertex center; // encodes color and position
    float radius;
    float mass;     //Hydrogen -> Atomic mass: 1.008 u  * <n>
    float qCharge;  //Electron -> -1.602 x 10^19 C.      * <n>

    void init();
    void calculateVertices(); // fills 'vertices'

    Atom();
    void create();
    void draw();
};

struct atomNode{
    Atom self = Atom();
    int n = 0; // number of edges (adj)
    std::vector<atomNode> adj;
    std::vector<vec2> edges;
};


#endif //CG_HF1_ATOM_H
