//
// Created by danie on 3/30/2022.
//

#include <deque>
#include "Molecule.h"

Molecule::Molecule() {
    init();
}

void Molecule::init() {
    atomNum = randomInt(8, 2);
    // will subtract from this to generate nodes
    int atomNumCopy = atomNum - 1; // -1 is for the root

    bonds.clear();
    atoms.clear();
    transMat = TranslateMatrix(vec3());

    vel = 0;

    nodes = atomNode(); // reset
    generateAtomTree(nodes, atomNumCopy);
    //generateTree();
    // set origin and mass center
    massCenter();

    // Sum of charges in molecule should be 0
    float sumCharge = 0;
    for (int i = 0; i < atoms.size() - 1; ++i) {
        sumCharge += atoms[i]->qCharge;
    }
    // -7 + x = 0 -> x = 7 hehe
    atoms.at(atoms.size()-1)->qCharge = -sumCharge;
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
        // push to atoms bonds
        node.self.bonds.push_back(&node.adj[i].self.center.pos);

        generateAtomTree(node.adj[i], atomNumCopy);
    }

    // Push self after children are generated
    this->atoms.push_back(&node.self);
}

void Molecule::create() {
    ::create(vao, vbo);
    // Creating all atoms
    for(auto atom: atoms) atom->create();
}

void Molecule::draw() {
    update();
    calculateBonds();

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * bonds.size(), &bonds[0], GL_DYNAMIC_DRAW);
    // Draw each bond as a line strip
    for (int i = 0; i < atomNum - 1; ++i) { // tree -> n - 1 edges
        glDrawArrays(GL_LINE_STRIP, (lineTessellation + 1) * i, (lineTessellation + 1));
    }

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

        // Moment of inertia
        float r = length(atom->center.pos);
        theta += atom->mass * r * r;
    }
    if(DEBUG_MASS){
        m.center = center;
        m.center.color = vec4(0,1,0,1);
        atoms.push_back(&m);
    }
}

void Molecule::react2Molecule(const Molecule& molecule, float dt) {

    rotationAngle = 0; // angle of rotation
    float w = 0; // angular velocity
    float M = 0; // torque

    // calculate sum force from all target atoms, vec direction, rotation angle
    for(auto atom: atoms){
        vec2 F;

        // adding all discrete charges
        for(auto targetAtom: molecule.atoms){
            // values should already be normalized
//            vec2 R = normalize(atom->center.pos - targetAtom->center.pos);
//            float len = length(R);
//            R = normalize(R);
            vec2 R = targetAtom->center.pos - atom->center.pos;
            float len = length(R);
            F = F + targetAtom->qCharge * (R / (len * len));
            //F = F + atom->qCharge * targetAtom->qCharge * R / coulomb;
        }
        // final multiplication with coulombs constant
        F = F * atom->qCharge * coulombConst;

        // Apply drag
        F = F - dragCoefficient * vel;

        // Momentum force
        vec2 Fm = F * atom->center.pos;
        // Rotational force
        vec2 Fr = F * vec2(atom->center.pos.y, - atom->center.pos.x);

        // ROTATION
        M += atom->center.pos.x * Fr.x - atom->center.pos.y * Fr.y; // (z component)M = Fx * dx - dy * Fy
        w = w + M / theta * dt;
        rotationAngle = rotationAngle + w; // * dt

        // MOVEMENT
        vel = vel + Fm / atom->mass * dt + w * atom->center.pos * dt;
    }
}

void Molecule::update() {
    // Calculate translations
    transMat = transMat * RotationMatrix(rotationAngle, vec3(center.pos.x, center.pos.y, 1));
    transMat.rows[3] = TranslateMatrix(vel).rows[3];

    for(auto atom : atoms){
        // Transform with mtx
        vec4 res = vec4(atom->center.pos.x, atom->center.pos.y,0,1) * transMat;
        atom->center.pos = vec2(res.x, res.y);
    }

    // Transform the center
    vec4 res = vec4(center.pos.x, center.pos.y,0,1) * transMat;
    center.pos = vec2(res.x, res.y);
}

void Molecule::calculateBonds() {
    // refresh the bonds vector with fresh atom data
    bonds.clear();
    for(auto atom : atoms){
        for(auto bond: atom->bonds){
            std::vector<vertex> li = getTessellation(atom->center.pos, *bond ,vec4(1,1,1,1));
            bonds.insert(bonds.end(), li.begin(), li.end());
        }
    }
}

Molecule::~Molecule() {
    if(vbo != 0 && vao != 0){
        glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
    }
}

//void Molecule::generateTree() {
//    atoms = std::vector<Atom>(atomNum);
//
//    // Prufer sequence
//    std::deque<int> P(atomNum - 2);
//    for (int i = 0; i < atomNum - 2; ++i) P[i] = randomInt(atomNum-1, 0);
//
//    // Ordered list
//    std::deque<int> L(atomNum);
//    for (int i = 0; i < atomNum; ++i) L[i] = i+1;
//
//    for (int i = 0; i < atomNum - 2; ++i) {
//        int k = -1;
//        for (int j = 0; j < atomNum; ++j) {
//            if(std::find(P.begin(), P.end(), L[j]) == P.end()){
//                k = j;
//                L.at(j) = -1;
//                break;
//            }
//        }
//        if(k != -1){
//            int j = P.at(0); // first number in P
//            // add edge here
//            atoms[k].bonds.push_back(&atoms[j].center.pos);
//
//            // removing values used
//            P.pop_front();
//            L.shrink_to_fit();
//            P.shrink_to_fit();
//        }
//    }
//    std::vector<int> f(2);
//    for (int i = 0; i < atomNum; ++i) {
//        if(L[i] != -1) f.push_back(L[i]);
//    }
//    atoms[f[0]].bonds.push_back(&atoms[f[1]].center.pos);
//
//}

