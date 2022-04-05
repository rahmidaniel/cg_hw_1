//
// Created by danie on 3/30/2022.
//

#include "Molecule.h"

Molecule::Molecule() {
    init();
}

void Molecule::init() {
    atomNum = randomInt(8, 2); // TODO: set to 2-3
    // will subtract from this to generate nodes
    int atomNumCopy = atomNum - 1; // -1 is for the root

    bonds.clear();
    atoms.clear();
    nodes = atomNode(); // reset
    generateAtomTree(nodes, atomNumCopy);
    // set origin and mass center
    massCenter();
}

void Molecule::generateAtomTree(atomNode& node, int& atomNumCopy){
    if(atomNumCopy < 1) { // Not enough nodes left to generate more
        this->atoms.push_back(&node.self);
        return;
    }

    // Generating children
    int children = randomInt(atomNumCopy);
    atomNumCopy -= children; // subtracting from TOTAL nodes

    node.n = children;
    node.adj = std::vector<atomNode>(node.n);

    // Recursive call on the children to generate the rest
    for(int i=0; i < children; i++) {
        //TODO: check on me later, color and all
        bonds.push_back({node.self.center.pos, vec4(1,1,1,1)}); // p1
        bonds.push_back({node.adj[i].self.center.pos, vec4(1,1,1,1)}); // p2

        // push to atoms bonds
        node.self.bonds.push_back(&node.adj[i].self.center.pos);

        generateAtomTree(node.adj[i], atomNumCopy);
    }

    // Push self after children are generated
    this->atoms.push_back(&node.self);
}

void Molecule::create() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //The bonds as lines
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * bonds.size(), &bonds[0], GL_DYNAMIC_DRAW);

    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    // Map attribute array 0 to the vertex data of the interleaved vbo
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) offsetof(vertex, pos));
    // Map attribute array 1 to the color data of the interleaved vbo
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (void *) offsetof(vertex, color));

    // Creating all atoms
    for(auto atom: atoms) atom->create();
}

void Molecule::draw() {

//    int location = glGetUniformLocation(gpuProgramID, "MVP");	// Get the GPU location of uniform variable MVP
//    glUniformMatrix4fv(location, 1, GL_TRUE, &MVP[0][0]);	// Load a 4x4 row-major float matrix to the specified location
    update();
    calculateBonds();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * bonds.size(), &bonds[0], GL_DYNAMIC_DRAW);
    //glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, bonds.size());
    for(auto atom: atoms) atom->draw();
}

void Molecule::massCenter() {
    float mSum = 1;
    for(auto atom: atoms) {
        mSum += atom->mass;
        center.pos.x += atom->center.pos.x * atom->mass;
        center.pos.y += atom->center.pos.y * atom->mass;
    }
    center.pos = center.pos / mSum;

//    // Translating all atoms around the origin
//    for(auto atom: atoms) atom->center.pos = atom->center.pos - center.pos;
//    // - || - all bonds // TODO: bonds bad pos USE MAT4 TRANSLATION
//    for(auto bond: bonds) bond.pos = bond.pos - center.pos;

//    MVP = { 1, 0, 0, center.pos.x,    // MVP matrix,
//            0, 1, 0, center.pos.y,    // row-major!
//            0, 0, 1, 0,
//            0, 0, 0, 1 };
    MVP = TranslateMatrix(center.pos);
    // transforming atoms to origin
    for(auto atom: atoms) {
//        vec4 res = vec4(atom->center.pos.x, atom->center.pos.y, 0, 1) * MVP;
//        atom->center.pos = vec2(res.x, res.y);
        atom->center.pos = atom->center.pos + center.pos;
    }
//    for(auto bond: bonds) {
//        vec4 res = vec4(bond.pos.x, bond.pos.y, 0, 1) * MVP;
//        bond.pos = bond.pos + center.pos * 2;
//    }
}

void Molecule::react2Molecule(const Molecule& molecule) {
    vel = 0;
    vec2 F; // direction of the molecule
    for(auto atom: atoms){
        // calculate sum force from all target atoms, vec direction
        vec2 f;
        // adding all discrete charges
        for(auto targetAtom: molecule.atoms){
            vec2 R = normalize(atom->center.pos - targetAtom->center.pos);
            float len = length(R);
            R = normalize(R);
            f = f + targetAtom->qCharge * ( R / (len * len));
        }
        // final multiplication with coulombs constant
        f = f * atom->qCharge * coulombConst;

        // this vector has to be split into torque and momentum
        // TODO: use drag here
        // use vector projection
        vec2 fm = f * normalize(atom->center.pos);
        // use normal vector for projection
        vec2 ff = f * normalize(vec2(atom->center.pos.y, - atom->center.pos.x));
        F = F + fm;
    }
    //if(length(vel) != 0) F = F - 0.32 * vel; // TODO: drag, 0.32 is mat dependent, test if 'if' matters
    //F = F - 0.32 * vel;
    vel = F;
}

void Molecule::update() {
    for(auto atom : atoms){
        atom->center.pos = atom->center.pos + vel;
    }
}

void Molecule::calculateBonds() {
    // refresh the bonds vector with fresh atom data
    bonds.clear();
    for(auto atom : atoms){
        for(auto bond: atom->bonds){
            bonds.push_back({atom->center.pos, vec4(1,1,1,1)}); // starting point of bond
            bonds.push_back({*bond, vec4(1,1,1,1)}); // endpoint of bond
        }
    }
}

Molecule::~Molecule() {
    if(vbo != 0 && vao != 0){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}

