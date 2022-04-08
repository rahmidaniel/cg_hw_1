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
    transMat = TranslateMatrix(vec3());
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

    for(auto atom: atoms) {
        atom->center.pos = atom->center.pos + center.pos;
    }
}

void Molecule::react2Molecule(const Molecule& molecule, float dt) {
    for(auto atom: atoms){
        // calculate sum force from all target atoms, vec direction
        vec2 F;
        // adding all discrete charges
        for(auto targetAtom: molecule.atoms){
            vec2 R = normalize(atom->center.pos - targetAtom->center.pos);
            float len = length(R);
            R = normalize(R);
            F = F + targetAtom->qCharge * (R / (len * len));
        }
        // final multiplication with coulombs constant
        F = F * atom->qCharge * coulombConst;

        // this vector has to be split into torque and momentum
        // vector projection
        vec2 Fm = F * normalize(atom->center.pos);
        // use normal vector for projection
        vec2 Fr = F * normalize(vec2(atom->center.pos.y, - atom->center.pos.x)); //
        //vec2 w = Fr / length(atom->center.pos); // w = Fr / radius

        // ROTATION
        float phi = acosf(length(Fr) / length(center.pos));
        M += atom->center.pos.x * Fr.x - atom->center.pos.y * Fr.y; // (z component)M = Fx * dx - dy * Fy
        w = w + M / phi * dt;
        //a = M / (atom->mass * atom->center.pos);
        a = a + w * dt;

        // MOVEMENT
        vel = vel + Fm / atom->mass * dt;
        center.pos = center.pos + vel * dt;
    }
}

void Molecule::update() {
    // Calculate translations
//    float phi = acosf(length(w) / length(center.pos));
//    transMat = transMat * RotationMatrix(phi, vec3(0,0,1));
    //transMat.rows[3] = TranslateMatrix(vel).rows[3];

    for(auto atom : atoms){
        // Transform with mtx
        vec4 res = vec4(atom->center.pos.x, atom->center.pos.y,0,1) * transMat;
        atom->center.pos = vec2(res.x, res.y);
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

