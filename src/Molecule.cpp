//
// Created by danie on 3/30/2022.
//

#include "Molecule.h"

Molecule::Molecule() {
    atomNum = randomInt(2, 8);
    int atomNumCopy = atomNum;
    generateAtomTree(atoms, atomNumCopy);
}

//void Molecule::calculateVertices(float* res) {
//    res = new float[atomNum * 2];
//    for(int i=0; i < atomNum; i++) {
//        res[i] = nodes[i].pos.x;
//        res[i+1] = nodes[i].pos.y;
//    }
//}

// TODO: gives +1 node everytime
void Molecule::generateAtomTree(atomNode& node, int& atomNumCopy){
    // init
    node.self = atom();
    this->points.push_back(node.self.pos.x);
    this->points.push_back(node.self.pos.y);

    if(atomNumCopy < 1) return;

    int children = randomInt(1, atomNumCopy);
    atomNumCopy -= children; // subtracting from TOTAL nodes

    node.n = children;
    node.adj = std::vector<atomNode>(node.n);
    node.edges = std::vector<vec3>(node.n);

    // recursive call on the children to generate the rest
    for(int i=0; i < children; i++) generateAtomTree(node.adj[i], atomNumCopy);
}
